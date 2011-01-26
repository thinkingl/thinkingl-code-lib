var allDvrInfomation = new Array();//存储所有的dvr信息
    
var DvrStore;

var allDvrNum = 0;
var correctNum = 0;
var notStartNum = 0;
var failed2dvrNum = 0;
var failed2cmuNum = 0;
var connecting2cmuNum=0;
var g_dvrFactoryTypeStore;

var selFilterType = new Ext.data.SimpleStore({
	fields:['index','name'],
	data:[[0,'全部DVR'],[1,'正常连接'],[2,'连接DVR失败'],[3,'连接平台失败'],[4,'未启动']]
});

var com;
function goToDvrlist(deleteCall , addCall){
 	ResourceLoader.loadTemplate("dvrList",function(html){
        $(".rightTab").html(html);
      //添加按键响应        
        $('#btn_stopDvr').click(stopDvr);
        $('#btn_startDvr').click(startDvr);  
        $('#btn_addDvr').click(addNewDvr);
        $('#btn_editDvr').click(editDvr); 
        $('#btn_deleteDvr').click(deleteDvr);   
        $('#btn_detailDvr').click(detailDvr); 
		$('#btn_advanceDvr').click(advanceDvrEdit);
        
        $('#btn_updateDvr').click(updateDvrStatus);     
     
		com = new Ext.form.ComboBox({
			store:selFilterType,
			mode:'local',
			triggerAction:'all',
			valueField:'index',
			displayField:'name',
			applyTo:'dvrlist_status',
			editable:false,
			listeners:{
				'select':function(combox){
					if(combox.getRawValue() == "全部DVR"){
						DvrStore.clearFilter();
					}else if(combox.getRawValue() == "正常连接"){  
						DvrStore.filter('statusOr','4',false,false);
					}else if(combox.getRawValue() == "连接DVR失败"){  
						DvrStore.filter('statusOr','0',false,false);
					}else if(combox.getRawValue() == "连接平台失败"){ 
						DvrStore.filter('statusOr','3',false,false);
					}else if(combox.getRawValue() == "未启动"){ 
						DvrStore.filter('statusOr','-1',false,false);
					}  
				}
			}
		});
		com.setValue(0);//显示全部dvr
		
		getFactoryType();//获取厂商类型
		updateDvrList();    //创建DVR列表
        isInDvrList=true;   
		
        $('body').everyTime(6000 , "updateDvrStatus" , updateDvrStatus);//设置一个心跳计时器，定期向服务器发送心跳  
        button_hover_function(); 
		
		licenseExpireWin();
    }); 
};

function licenseExpireWin()
{
	CMonMsg.setId(G100_COMMAND.COMMAND_GETVERSION);
	CMonMsg.setMsgNode("version","version"); 	
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
	{
	   if(isErrorCommand(json)===false)
			return; 
		var remainingDays = parseInt(json.kedacomxmldata.content.expire);
		if(remainingDays >= 0 && remainingDays < 7)
		{
			var str = "license还剩" + remainingDays + "天过期";
			myAlert(null,str,240,0);
		}
	},false);
}


function popupAdvWin(name, puid, configStr)
{
	Ext.QuickTips.init(); 
	var advWin = new Ext.Window({
		frame:true,
		title:name,
		width:450,
		height:500,
		modal:true,
		layout:'form',
		items:[{
			xtype:'textarea',
			hideLabel:true,
			id:'conf',
			autoScroll:true,
			anchor: '100% -5'
		}],
		buttons:[{
			text:'确定',
			handler:function(){
				CMonMsg.setId(G100_COMMAND.COMMAND_SETDVRCONFIG);
				CMonMsg.setMsgNode('puid',puid);
				CMonMsg.setMsgNode('file',advWin.findById('conf').getValue());
				ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
					if(isErrorCommand(json)===false)
                 		return;	
					var value = json.kedacomxmldata.content.status;
					if(value == 1)
					{
						advWin.close();
             	        myAlert(null,"修改配置成功!",200,0);
					}
					else
					{
						myAlert(null,"修改配置失败!",200,0);
					}
				},false);//end loadXmlData over
			}//end handler
		},{
			text:'取消',
			handler:function(){
				advWin.close();
			}//end handler
		}]
	});	
	advWin.findById('conf').setValue(configStr);
	
	advWin.show();
}

/*
	高级dvr编辑，直接修改文件
*/
function advanceDvrEdit()
{

	var sm = grid.getSelectionModel();
	if(sm.hasSelection())
	{
		var sel = sm.getSelected();
		//获取配置文件内容
		CMonMsg.setId(G100_COMMAND.COMMAND_GETDVRCONFIG);
		CMonMsg.setMsgNode('puid',sel.data.puid);
		ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
			if(isErrorCommand(json)===false)
                 return;
			popupAdvWin(sel.data.name, sel.data.puid, json.kedacomxmldata.content.file);
		});
	}
	else
	{
		myAlert(null,"没有选中任何DVR",180,0);
        return;
	}
}

//更新
function  updateDvrStatus(){
    if(isInDvrList==false){
        $('body').stopTime("updateDvrStatus");  
        return;  
    }
    CMonMsg.setId(G100_COMMAND.COMMAND_GETALTERED);
	CMonMsg.setMsgNode("range","1"); //１为DVR，　2为mtu	
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
	{
       if(isErrorCommand(json)===false)
            return; 
       if(json.kedacomxmldata.content.devnum == "0")
            return;
       if(json.kedacomxmldata.content.devnum == "1"){
            for(var i = 0 ; i < DvrStore.data.length ; i++){
                var record = DvrStore.getAt(i);
                var puid = record.get("puid");
                if(puid == json.kedacomxmldata.content.dev.puid){
                    record.set("status" , json.kedacomxmldata.content.dev.status);
                    record.set("statusOr" , json.kedacomxmldata.content.dev.statusOr);                    
                    record.commit();
                }                   
            }
       }else{
            for(var k = 0; k<json.kedacomxmldata.content.dev.length; k++){
                for(var i = 0 ; i < DvrStore.data.length ; i++){
                    var record = DvrStore.getAt(i);
                    var puid = record.get("puid");
                    if(puid == json.kedacomxmldata.content.dev[k].puid){
                        record.set("status" , json.kedacomxmldata.content.dev[k].status);
                        record.set("statusOr" , json.kedacomxmldata.content.dev[k].statusOr);                    
                        record.commit();
                    } //if over       
                } //for over
             }//for over
       }//else over
       updateTheNum();     
    },false);//loadXmlData　over
}


function updateTheNum(){
    correctNum = 0;
    failed2dvrNum = 0;
    failed2cmuNum = 0 ;
    notStartNum = 0 ;
    for(var i = 0 ; i < DvrStore.data.length ; i++){
       var record = DvrStore.getAt(i);
       var value = record.get("statusOr"); 
       if(value == "-1"){
                notStartNum++;                
       }else if(value == "0"){
                failed2dvrNum++;             
       }else if(value == "2"){
                connecting2cmuNum++;             
       }else if(value == "3"){
                failed2cmuNum++;            
       }else if(value == "4"){
                correctNum++;           
       }                         
    }
    $("#dvrlist_normal").html(correctNum);           
    $("#dvrlist_faild2dvr").html(failed2dvrNum);
    $("#dvrlist_faild2cmu").html(failed2cmuNum);   
    $("#dvrlist_noStart").html(notStartNum);   
}
//连接服务器得到所有的DVR信息
function updateDvrList(){
        CMonMsg.setId(G100_COMMAND.COMMAND_GETDVRS);
    	CMonMsg.setMsgNode("range","1"); 
    	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
    	{
	       if(isErrorCommand(json)===false)
	        return; 
              
          allDvrNum = 0;
          correctNum = 0;
          notStartNum = 0;
          failed2dvrNum = 0;
          failed2cmuNum = 0;
          connecting2cmuNum=0; 
          
          allDvrNum = json.kedacomxmldata.content.dvrnum;  
          if(allDvrNum == "0"){
            
          }else if(allDvrNum == "1"){
                   var value = json.kedacomxmldata.content.dev.statusOr; 
                   if(value == "-1"){
                            notStartNum++;                
                   }else if(value == "0"){
                            failed2dvrNum++;             
                   }else if(value == "2"){
                            connecting2cmuNum++;             
                   }else if(value == "3"){
                            failed2cmuNum++;            
                   }else if(value == "4"){
                            correctNum++;           
                   }                                                    
          }else{
              for(i=0;i<allDvrNum;i++){ 
                   var value = json.kedacomxmldata.content.dev[i].statusOr; 
                   if(value == "-1"){
                            notStartNum++;                
                   }else if(value == "0"){
                            failed2dvrNum++;             
                   }else if(value == "2"){
                            connecting2cmuNum++;             
                   }else if(value == "3"){
                            failed2cmuNum++;            
                   }else if(value == "4"){
                            correctNum++;           
                   }                                       
               } 
           }
           
           allDvrInfomation = json.kedacomxmldata.content;
		   
           addDvrList(allDvrInfomation);
           $("#dvrlist_total").html(allDvrNum);
           $("#dvrlist_normal").html(correctNum);           
           $("#dvrlist_faild2dvr").html(failed2dvrNum);
           $("#dvrlist_faild2cmu").html(failed2cmuNum);   
           $("#dvrlist_noStart").html(notStartNum);                     
    	},false);
};

/*
	告警状态上报解析
*/
function alarmRender(val)
{
	var valStr = val;
	if(val == 0)
	{
		valStr = "告警停止"
	}
	else
	{
		valStr = "告警上报";
	}
	return valStr;
}

function addDvrList(json){
    $("#dvrlist").html('');
    var sm = new Ext.grid.CheckboxSelectionModel(); 
	var cm = new Ext.grid.ColumnModel([ 
		new Ext.grid.RowNumberer(), 
		sm, 
		{header:'名称',dataIndex:'name',sortable:true,width:50}, 
		{header:'设备IP',dataIndex:'ip',width:60}, 
		{header:'设备入网ID',dataIndex:'puid',sortable:true,width:220}, 
		{header:'当前状态',dataIndex:'status',sortable:true},
		{header:'告警',dataIndex:'alarm',renderer:alarmRender}
	]); 

	DvrStore = new Ext.data.Store({ 
		proxy: new Ext.data.PagingMemoryProxy(json),     
		reader: new Ext.data.JsonReader({
			id:'puid',
			root:'dev'
		}, [ 
			{name: 'name'}, 
			{name: 'ip'}, 
			{name: 'puid'}, 
			{name: 'alarm'},
			{name: 'status'},
			{name: 'statusOr'}
		]) 

	}); 
	DvrStore.load(); 

	grid = new Ext.grid.GridPanel({ 
		renderTo: 'dvrlist', 
		store: DvrStore, 
		height: 400,
		width : 605,
		cm: cm, 
		sm: sm 
	}); 
	com.setValue(0);
};

//添加按键响应
function addNewDvr(){
   Ext.QuickTips.init(); 
   var win = new Ext.Window({
        frame: true,
        title:'添加DVR',            
        width:350,
        modal : true,
        layout: 'form',
        items:[{
            xtype:'textfield',
            fieldLabel:'名称',
            id:'dvr_name',
            width:220,            
            allowBlank: false,
			maxLength:24,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '24'}
        },{
            xtype:'textfield',
            fieldLabel:'DVR入网ID',
            id:'dvr_puid',
            width:220,
            allowBlank: false,
			maxLength:32,
			minLength:32,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '32'}
        },{
            xtype:'combo',
            fieldLabel:'制造厂商',
            id:'dvr_manufactory',
            mode:'local',
            store:g_dvrFactoryTypeStore,
            displayField:'name',
			editable:false,
            width:220,
            triggerAction: 'all',               
            allowBlank: false,
            listeners:{
                select: function(f,r,i){
                    if(i==0){
                        win.findById('dvr_port').setValue(8000);
                    }else if(i==1){
                        win.findById('dvr_port').setValue(37777);                    
                    }else{
                        win.findById('dvr_port').setValue("");                    
                    }
                }
            }
        },{
            xtype:'textfield',
            fieldLabel:'设备IP',
            id:'dvr_ip',
            width:220,            
            allowBlank: false,
			maxLength:15,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '15'}
        },{
            xtype:'numberfield',
            fieldLabel:'设备端口',
            id:'dvr_port',
            width:220,
            allowBlank: false,
            type: 'int',
			maxValue:65535
        },{
            xtype:'textfield',
            fieldLabel:'DVR用户名',
            id:'dvr_user',
            width:220,            
            allowBlank: false,
			maxLength:24,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '24'}
        },{
            xtype:'textfield',
            fieldLabel:'DVR密码',
            id:'dvr_password',
            width:220,             
            allowBlank: false,
			maxLength:24,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '24'}
        },{
			xtype:'checkbox',
			fieldLable:'',
			labelSeparator:'',
			boxLabel:'自动获取DVR通道',
			id:'dvr_autochnl',
			checked:true,
			listeners:{
				check:function(obj,status){
					if(status)
					{
						//选中状态
						win.findById('dvr_chnlnum').disable();
					}
					else
					{
						//未选中状态
						win.findById('dvr_chnlnum').enable();
					}
				}
			}
		},{
			xtype:'textfield',
			fieldLabel:'DVR通道数',
			id:'dvr_chnlnum',
			width:220,
			disabled:true
		},{
			xtype:'checkbox',
			fieldLable:'',
			id:'dvr_alarmNotify',
		    boxLabel:'告警上报',
			labelSeparator:''
		}],
        buttons:[{
            text:'确定',
            handler:function(){

            //验证：
                var dvrName = win.findById('dvr_name').getValue();          
                var dvrPuid = win.findById('dvr_puid').getValue();
                
                if(dvrName == ''){
                        myAlert(null,"名称不能为空！" , 180 ,0);   
                        return;                 
                }
                
                for(var i =0 ; i< allDvrNum ; i++){
                    var dvrNameLocal;
                    var dvrPuidLocal;
                    if(allDvrNum == 1){
                        dvrNameLocal = allDvrInfomation.dev.name;
                        dvrPuidLocal = allDvrInfomation.dev.puid;
                    }else{
                        dvrNameLocal = allDvrInfomation.dev[i].name ;
                        dvrPuidLocal = allDvrInfomation.dev[i].puid ;
                    }
                    if(dvrNameLocal == dvrName){
                        myAlert(null,"名称有重复，请重新输入！" , 230 ,0);
                        return;                   
                    }
					
                    if(dvrPuidLocal == dvrPuid){
                        myAlert(null,"DVR入网ID有重复，请重新输入！！",280 , 0);
                        return;                   
                    } 
                }
				if(!UiUtil.checkField('dvr_name','DVR名称'))
					return;
					
				var puidValue = win.findById('dvr_puid').getValue();
				if(!UiUtil.checkPstvNumber('dvr_puid','DVR入网ID'))
					return;
				if(puidValue.length != 32)
				{
					myAlert(null,"DVR入网ID号少于32位数字",250,0);
					return;
				}
				
				if(!UiUtil.checkEmpty('dvr_manufactory','制造厂商'))
					return;
								
				if(!UiUtil.checkIp('dvr_ip','设备IP'))
					return;
					
				if(!UiUtil.checkPstvNumber('dvr_port','设备端口'))
					return;
				if(parseInt($('#dvr_port').val()) > 65535)
				{
					myAlert(null,"超过最大端口数65535!",220,0);
					return;
				}
				
				
				if(!UiUtil.checkField('dvr_user','DVR用户名'))
					return;
					
				if(!UiUtil.checkField('dvr_password','DVR密码'))
					return;
				
				if(!win.findById('dvr_chnlnum').disabled)
				{
					if(!UiUtil.checkPstvNumber('dvr_chnlnum','DVR通道数'))
						return;
				}
            //验证over
				CMonMsg.setId(G100_COMMAND.COMMAND_CHANGEDVR);
				CMonMsg.setMsgNode("configfile","mdcfg_"+win.findById('dvr_name').getValue()+".ini");   
				CMonMsg.setMsgNode("puid",win.findById('dvr_puid').getValue());            
				CMonMsg.setMsgNode("dvrip",win.findById('dvr_ip').getValue());
				CMonMsg.setMsgNode("dvrport",win.findById('dvr_port').getValue());            
				CMonMsg.setMsgNode("dvrname",win.findById('dvr_user').getValue());
				CMonMsg.setMsgNode("dvrpass",win.findById('dvr_password').getValue());
				CMonMsg.setMsgNode("dvrmanufactory",char2character(win.findById('dvr_manufactory').getValue(),true));
				CMonMsg.setMsgNode("channelnum",win.findById('dvr_chnlnum').getValue());
				CMonMsg.setMsgNode("autogetchnnum",Number(win.findById('dvr_autochnl').getValue()));
				CMonMsg.setMsgNode("dvralarm",Number(win.findById('dvr_alarmNotify').getValue()));
				ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
					if(isErrorCommand(json)===false)
					   return;
					var value = json.kedacomxmldata.content.status;
					if (value == 1){
					   win.close();
					   myAlert(null,"添加新设备成功!" , 200, 0);
					   
					   setTimeout(updateDvrList,2*1000);
					}else if(value == 2){
					   myAlert(null,"添加新设备失败，请重新添加！",300,0);
					}        
			   },false);//loadXmlData over
             }//function over
        },{
            text:'取消',
            handler:function(){
                win.close();
            }
        }
        ]        
    });
    win.show();   
};//添加按键响应 over

//编辑一个dvr信息
function editDvr(){
        var sm=grid.getSelectionModel();      
        if(sm.hasSelection()){
            var sel = sm.getSelected();
            CMonMsg.setId(G100_COMMAND.COMMAND_GETDVRDETAIL);       
            CMonMsg.setMsgNode("puid",sel.data.puid);    
           	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
        	    if(isErrorCommand(json)===false)
        	       return;
                editNewDvr(json.kedacomxmldata.content.name,
                           json.kedacomxmldata.content.puid, 
                           json.kedacomxmldata.content.dvrip, 
                           json.kedacomxmldata.content.dvrport, 
                           json.kedacomxmldata.content.dvrname,
                           json.kedacomxmldata.content.dvrpass, 
                           json.kedacomxmldata.content.dvrmanufactory,
                           json.kedacomxmldata.content.configfile,
						   json.kedacomxmldata.content.dvralarm,
						   json.kedacomxmldata.content.autogetchnnum,
						   json.kedacomxmldata.content.channelnum
                           );
           },false);//loadXmlData over
        }else{
            myAlert(null,"没有选中任何DVR",180,0);
            return;
        }
};//编辑一个dvr信息 over
//添加按键响应
function editNewDvr(dvrName,dvrPuid,dvrIp,dvrPort,dvrUser,dvrPassword,dvrManufactory,configFile,dvralarm,autogetchnnum,chnlnum){
   Ext.QuickTips.init(); 
   var win = new Ext.Window({
        frame: true,
        title:'修改DVR',            
        width:350,
        modal : true,
        layout: 'form',
        items:[{
            xtype:'textfield',
            fieldLabel:'名称',
            id:'dvr_name',
            width:220,            
            allowBlank: false,
            disabled:true
        },{
            xtype:'textfield',
            fieldLabel:'设备入网ID',
            id:'dvr_puid',
            width:220,
            allowBlank: false,
			maxLength:32,
			minLength:32,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '32'}
        },{
            xtype:'combo',
            fieldLabel:'制造厂商',
            id:'dvr_manufactory',
            mode:'local',
            store:g_dvrFactoryTypeStore,
            displayField:'name',
            width:220,
            allowBlank: false,
            triggerAction: 'all',  
			editable:false,
            listeners:{
                select: function(f,r,i){
                    if(i==0){
                        win.findById('dvr_port').setValue(8000);
                    }else if(i==1){
                        win.findById('dvr_port').setValue(37777);                    
                    }else{
                        win.findById('dvr_port').setValue(12855);                    
                    }
                
                }
            }            
        },{
            xtype:'textfield',
            fieldLabel:'DVR地址',
            id:'dvr_ip',
            width:220,
            allowBlank: false,
			maxLength:15,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '15'}
        },{
            xtype:'numberfield',
            fieldLabel:'DVR端口号',
            id:'dvr_port',
            width:220,
            allowBlank: false,
			maxValue:65535
        },{
            xtype:'textfield',
            fieldLabel:'登陆用户名',
            id:'dvr_user',
            width:220,
            allowBlank: false,
			maxLength:24,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '24'}
        },{
            xtype:'textfield',
            fieldLabel:'登陆密码',
            id:'dvr_password',
            width:220,
            allowBlank: false,
			maxLength:24,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '24'}
        },{
			xtype:'checkbox',
			fieldLable:'',
			labelSeparator:'',
			boxLabel:'自动获取DVR通道',
			id:'dvr_autochnl',
			checked:true,
			listeners:{
				check:function(obj,status){
					if(status)
					{
						//选中状态
						win.findById('dvr_chnlnum').disable();
					}
					else
					{
						//未选中状态
						win.findById('dvr_chnlnum').enable();
					}
				}
			}
		},{
			xtype:'textfield',
			fieldLabel:'DVR通道数',
			id:'dvr_chnlnum',
			width:220,
			disabled:true
		},{
			xtype:'checkbox',
			fieldLable:'',
			id:'dvr_alarmNotify',
		    boxLabel:'告警上报',
			labelSeparator:''
		}],
        buttons:[{
            text:'确定',
            handler:function(){
                            //验证：
                            var dvrName = win.findById('dvr_name').getValue();          
                            var dvrPuid = win.findById('dvr_puid').getValue();
                            for(var i =0 ; i< allDvrInfomation.dev.length ; i++){
                                var dvrNameLocal;
                                var dvrPuidLocal;
                                if(allDvrNum == 1){
                                    dvrNameLocal = allDvrInfomation.dev.name;
                                    dvrPuidLocal = allDvrInfomation.dev.puid;
                                }else{
                                    dvrNameLocal = allDvrInfomation.dev[i].name ;
                                    dvrPuidLocal = allDvrInfomation.dev[i].puid ;
                                }

                                if(dvrPuidLocal == dvrPuid){
                                    if(dvrNameLocal != dvrName){
                                        myAlert(null,"DVR入网ID有重复！",200 , 0);
                                        return;                   
                                    }                                              
                                } 
                            }
						
						var puidValue = win.findById('dvr_puid').getValue();
						if(!UiUtil.checkPstvNumber('dvr_puid','DVR入网ID'))
							return;
						if(puidValue.length != 32)
						{
							myAlert(null,"DVR入网ID号少于32位数字",250,0);
							return;
						}
					
						if(!UiUtil.checkEmpty('dvr_manufactory','制造厂商'))
							return;
										
						if(!UiUtil.checkIp('dvr_ip','设备IP'))
							return;
							
						if(!UiUtil.checkPstvNumber('dvr_port','设备端口'))
							return;
						if(parseInt($('#dvr_port').val()) > 65535)
						{
							myAlert(null,"超过最大端口数65535!",220,0);
							return;
						}
					
						if(!UiUtil.checkField('dvr_user','DVR用户名'))
							return;
							
						if(!UiUtil.checkField('dvr_password','DVR密码'))
							return;
						
						if(!win.findById('dvr_chnlnum').disabled)
						{
							if(!UiUtil.checkPstvNumber('dvr_chnlnum','DVR通道数'))
								return;
						}
                        //验证over
                        CMonMsg.setId(G100_COMMAND.COMMAND_CHANGEDVR);
                        CMonMsg.setMsgNode("configfile",configFile);   
                        CMonMsg.setMsgNode("puid",win.findById('dvr_puid').getValue());            
                        CMonMsg.setMsgNode("dvrip",win.findById('dvr_ip').getValue());
                        CMonMsg.setMsgNode("dvrport",win.findById('dvr_port').getValue());            
                        CMonMsg.setMsgNode("dvrname",win.findById('dvr_user').getValue());
                        CMonMsg.setMsgNode("dvrpass",win.findById('dvr_password').getValue());
                        CMonMsg.setMsgNode("dvrmanufactory",char2character(win.findById('dvr_manufactory').getValue(),true));
                        CMonMsg.setMsgNode("channelnum",win.findById('dvr_chnlnum').getValue());
						CMonMsg.setMsgNode("autogetchnnum",Number(win.findById('dvr_autochnl').getValue()));
						CMonMsg.setMsgNode("dvralarm",Number(win.findById('dvr_alarmNotify').getValue()));
                       	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
                    	    if(isErrorCommand(json)===false)
                    	       return;
                 	        var value = json.kedacomxmldata.content.status;
                 	        if (value == 1){
                 	           win.close();
             	               myAlert(null,"修改设备成功!",200,0);
             	               setTimeout(updateDvrList,2*1000);
                            }else if(value == 2){
                               myAlert(null,"修改设备失败，请重新操作！",280,0);
                            }        
                       },false);//loadXmlData over
                }//function over
        },{
            text:'取消',
            handler:function(){
                win.close();
            }
        }
        ]        
    });
    win.findById('dvr_name').setValue(dvrName);   
    win.findById('dvr_puid').setValue(dvrPuid);
    win.findById('dvr_ip').setValue(dvrIp);   
    win.findById('dvr_port').setValue(dvrPort);   
    win.findById('dvr_user').setValue(dvrUser);   
    win.findById('dvr_password').setValue(dvrPassword);    
    win.findById('dvr_manufactory').setValue(char2character(dvrManufactory,false)); 
	win.findById('dvr_alarmNotify').setValue(dvralarm);
	win.findById('dvr_chnlnum').setValue(chnlnum);
	win.findById('dvr_autochnl').setValue(autogetchnnum);
    win.show();   
};//添加按键响应 over
function char2character(manufactory,bReturnIsChar){
   /* var charHK = "haikang";
    var charDH = "dahua";
    var charBSR = "bluesky";
    var chHK = "海康(haikang)";
    var chDH = "大华(dahua)";
    var chBSR="蓝色星际(bluesky)";
    if(bReturnIsChar === true){
        if(manufactory == chHK)
            return charHK;
        if(manufactory == chDH)
            return charDH;        
        if(manufactory == chBSR)
            return charBSR;    
    }else{
        if(manufactory == charHK)
            return chHK;
        if(manufactory == charDH)
            return chDH;        
        if(manufactory == charBSR)
            return chBSR;      
    } */
	var strRet = "undefined";
	if(!bReturnIsChar)
	{
		//包含汉字
		g_dvrFactoryTypeStore.each(function(rec){
			if(rec.data.name.indexOf(manufactory) != -1)
			{
				strRet = rec.data.name;
				return false;
			}
		});//end each
	}
	else
	{
		//将中文汉字去掉
		var nLeft = manufactory.indexOf("(");//左括号的位置
		var nRight = manufactory.indexOf(")");//右括号的位置
		strRet = manufactory.substr(nLeft+1,nRight-nLeft-1);
	}
	return strRet;
};

function deleteDvr(){
    var sm = grid.getSelectionModel();
    var store = grid.getStore();
    var view = grid.getView();
    if(!sm.hasSelection()){
        myAlert(null,"没选中任何DVR",180,0);
        return;   
    }
    
    Ext.Msg.show({
        title:"提示",
        msg:"是否删除选中的设备！",
        icon:Ext.MessageBox.INFO, 
		width:220,
        buttons:{
                    yes:true,
                    no:true
        },
        fn:function(btn){
            if(btn =="yes"){
                CMonMsg.setId(G100_COMMAND.COMMAND_DELETEDVR);
                for(var i = 0; i < view.getRows().length; i ++){
                     if(sm.isSelected(i)){
                        var record = store.getAt(i);
                        var puid = record.get("puid");
                        CMonMsg.setMsgNode("puid",puid);           
                     }
                } 
            	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
                    if(isErrorCommand(json)===false)
                       return;
                    var value = json.kedacomxmldata.content.status;
                    myAlert(null,"成功删除了"+ value+"个DVR设备!",220,0);
                    updateDvrList();    
                },false);//loadXmlData over                  
            }
            else
            {
                return;
            }
        
        }
    });  
};

function detailDvr(){
        var sm=grid.getSelectionModel();      
        if(sm.hasSelection()){
            var sel = sm.getSelected();
            CMonMsg.setId(5);       
            CMonMsg.setMsgNode("puid",sel.data.puid);    
           	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
        	    if(isErrorCommand(json)===false)
        	       return;
                var  detailForADvr="<p style='margin:10px;color:#205699;'><label style='padding-left:48px;padding-right:10px;'>名称：</label><span>"+json.kedacomxmldata.content.name+"</span></p>"+
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:13px;padding-right=10px;'>设备入网ID：</span><span>"+json.kedacomxmldata.content.puid+"</span></p>"+
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:38px;padding-right=10px;'>设备IP：</span><span>"+json.kedacomxmldata.content.dvrip+"</span></p>"+                                                
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:12px;padding-right=10px;'>设备端口号：</span><span>"+json.kedacomxmldata.content.dvrport+"</span></p>"+                        
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:12px;padding-right=10px;'>设备用户名：</span><span>"+json.kedacomxmldata.content.dvrname+"</span></p>"+                        
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:0px;padding-right=10px;'>设备通道数量：</span><span>"+json.kedacomxmldata.content.channelnum+"</span></p>"+                        
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:24px;padding-right=10px;'>设备厂商：</span><span>"+json.kedacomxmldata.content.dvrmanufactory+"</span></p>"+                                             
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:12px;padding-right=10px;'>调试端口号：</span><span>"+json.kedacomxmldata.content.telnet+"</span></p>"+                        
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:12px;padding-right=10px;'>上报端口号：</span><span>"+json.kedacomxmldata.content.startport+"</span></p>"+                        
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:24px;padding-right=10px;'>进程信息：</span><span>"+json.kedacomxmldata.content.processname+"("+
                                        json.kedacomxmldata.content.processid+")"+"</span></p>"+
						"<p style='margin:10px;color:#205699;'><span style='padding-left:24px;padding-right=10px;'>告警上报：</span><span>" + UiUtil.parseBoolFromNum(json.kedacomxmldata.content.dvralarm) + "</span></p>"+
						"<p style='margin:10px;color:#205699;'><span style='padding-left:0px;padding-right=10px;'>自动获取通道：</span><span>" + UiUtil.parseBoolFromNum(json.kedacomxmldata.content.autogetchnnum) +"</span></p>";
                var win = new Ext.Window({
                    frame: true,
                    title:'DVR详细信息',            
                    width:350,
                    modal : true,
                    html: detailForADvr,
                    buttons:[{
                        text:'关闭',
                        handler:function(){
                            win.close();
                        }
                    }]
                });
                win.show();
           },false);//loadXmlData over                       
        }else{
            myAlert(null,"没有选中任何DVR",180,0);
            return;
        }   
};

function stopDvr(){
    var sm = grid.getSelectionModel();
    var store = grid.getStore();
    var view = grid.getView();
	//view.getRows().length == 0;
    if(!sm.hasSelection()){
        myAlert(null,"没有选中任何DVR",180,0);
        return;   
    }
    Ext.Msg.show({
        title:"提示",
        msg:"是否停止选中的设备！",
		width:220,
        icon:Ext.MessageBox.INFO,
        buttons:{
                    yes:true,
                    no:true
        },
        fn:function(btn){
            if(btn =="yes"){
                CMonMsg.setId(G100_COMMAND.COMMAND_STOPDVR);
                for(var i = 0; i < view.getRows().length; i ++){
                     if(sm.isSelected(i)){
                        var record = store.getAt(i);
                        var puid = record.get("puid");
                        CMonMsg.setMsgNode("puid",puid);           
                     }
                } 
            	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
                    if(isErrorCommand(json)===false)
                       return;
                    var value = json.kedacomxmldata.content.status;
                    myAlert(null , "成功停止了"+ value+"个DVR设备!" , 250 , 0);
                    updateDvrList();    
                },false);//loadXmlData over                  
            }
            else
            {
                return;
            }
        
        }
    }); 
};

function startDvr(){
    var sm = grid.getSelectionModel();
    var store = grid.getStore();
    var view = grid.getView();
    if(!sm.hasSelection()){
        myAlert(null,"没有选中任何DVR",180,0);
        return;   
    }
    
    Ext.Msg.show({
        title:"提示",
        msg:"是否启动所选中的设备",
		width:220,
        icon:Ext.MessageBox.INFO,        
        buttons:{
                    yes:true,
                    no:true
        },
        fn:function(btn){
            if(btn =="yes"){
                CMonMsg.setId(G100_COMMAND.COMMAND_STARTDVR);
                for(var i = 0; i < view.getRows().length; i ++){
                     if(sm.isSelected(i)){
                        var record = store.getAt(i);
                        var puid = record.get("puid");
                        CMonMsg.setMsgNode("puid",puid);           
                     }
                } 
            	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
                    if(isErrorCommand(json)===false)
                       return;
                    var value = json.kedacomxmldata.content.status;
                    myAlert(null,"成功启动了"+ value+"个DVR设备!", 250 , 0);
                   // updateDvrList();    
                },false);//loadXmlData over                  
            }
            else
            {
                return;
            }
        
        }
    }); 
};

//获取厂家类型
function getFactoryType()
{
	CMonMsg.setId(G100_COMMAND.COMMAND_FACTORYTYPE);
	CMonMsg.setMsgNode("name","vendor");
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
		if(isErrorCommand(json)==false)
        	return;	
		g_dvrFactoryTypeStore = new Ext.data.Store({
			proxy: new Ext.data.MemoryProxy(json.kedacomxmldata.content),
			reader:new Ext.data.JsonReader({root:'factory'},[{name:'name'}])
		});
		g_dvrFactoryTypeStore.load();
	},true);
}
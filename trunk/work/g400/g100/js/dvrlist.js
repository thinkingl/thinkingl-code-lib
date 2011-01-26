var allDvrInfomation = new Array();//�洢���е�dvr��Ϣ
    
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
	data:[[0,'ȫ��DVR'],[1,'��������'],[2,'����DVRʧ��'],[3,'����ƽ̨ʧ��'],[4,'δ����']]
});

var com;
function goToDvrlist(deleteCall , addCall){
 	ResourceLoader.loadTemplate("dvrList",function(html){
        $(".rightTab").html(html);
      //��Ӱ�����Ӧ        
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
					if(combox.getRawValue() == "ȫ��DVR"){
						DvrStore.clearFilter();
					}else if(combox.getRawValue() == "��������"){  
						DvrStore.filter('statusOr','4',false,false);
					}else if(combox.getRawValue() == "����DVRʧ��"){  
						DvrStore.filter('statusOr','0',false,false);
					}else if(combox.getRawValue() == "����ƽ̨ʧ��"){ 
						DvrStore.filter('statusOr','3',false,false);
					}else if(combox.getRawValue() == "δ����"){ 
						DvrStore.filter('statusOr','-1',false,false);
					}  
				}
			}
		});
		com.setValue(0);//��ʾȫ��dvr
		
		getFactoryType();//��ȡ��������
		updateDvrList();    //����DVR�б�
        isInDvrList=true;   
		
        $('body').everyTime(6000 , "updateDvrStatus" , updateDvrStatus);//����һ��������ʱ�����������������������  
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
			var str = "license��ʣ" + remainingDays + "�����";
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
			text:'ȷ��',
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
             	        myAlert(null,"�޸����óɹ�!",200,0);
					}
					else
					{
						myAlert(null,"�޸�����ʧ��!",200,0);
					}
				},false);//end loadXmlData over
			}//end handler
		},{
			text:'ȡ��',
			handler:function(){
				advWin.close();
			}//end handler
		}]
	});	
	advWin.findById('conf').setValue(configStr);
	
	advWin.show();
}

/*
	�߼�dvr�༭��ֱ���޸��ļ�
*/
function advanceDvrEdit()
{

	var sm = grid.getSelectionModel();
	if(sm.hasSelection())
	{
		var sel = sm.getSelected();
		//��ȡ�����ļ�����
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
		myAlert(null,"û��ѡ���κ�DVR",180,0);
        return;
	}
}

//����
function  updateDvrStatus(){
    if(isInDvrList==false){
        $('body').stopTime("updateDvrStatus");  
        return;  
    }
    CMonMsg.setId(G100_COMMAND.COMMAND_GETALTERED);
	CMonMsg.setMsgNode("range","1"); //��ΪDVR����2Ϊmtu	
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
    },false);//loadXmlData��over
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
//���ӷ������õ����е�DVR��Ϣ
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
	�澯״̬�ϱ�����
*/
function alarmRender(val)
{
	var valStr = val;
	if(val == 0)
	{
		valStr = "�澯ֹͣ"
	}
	else
	{
		valStr = "�澯�ϱ�";
	}
	return valStr;
}

function addDvrList(json){
    $("#dvrlist").html('');
    var sm = new Ext.grid.CheckboxSelectionModel(); 
	var cm = new Ext.grid.ColumnModel([ 
		new Ext.grid.RowNumberer(), 
		sm, 
		{header:'����',dataIndex:'name',sortable:true,width:50}, 
		{header:'�豸IP',dataIndex:'ip',width:60}, 
		{header:'�豸����ID',dataIndex:'puid',sortable:true,width:220}, 
		{header:'��ǰ״̬',dataIndex:'status',sortable:true},
		{header:'�澯',dataIndex:'alarm',renderer:alarmRender}
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

//��Ӱ�����Ӧ
function addNewDvr(){
   Ext.QuickTips.init(); 
   var win = new Ext.Window({
        frame: true,
        title:'���DVR',            
        width:350,
        modal : true,
        layout: 'form',
        items:[{
            xtype:'textfield',
            fieldLabel:'����',
            id:'dvr_name',
            width:220,            
            allowBlank: false,
			maxLength:24,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '24'}
        },{
            xtype:'textfield',
            fieldLabel:'DVR����ID',
            id:'dvr_puid',
            width:220,
            allowBlank: false,
			maxLength:32,
			minLength:32,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '32'}
        },{
            xtype:'combo',
            fieldLabel:'���쳧��',
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
            fieldLabel:'�豸IP',
            id:'dvr_ip',
            width:220,            
            allowBlank: false,
			maxLength:15,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '15'}
        },{
            xtype:'numberfield',
            fieldLabel:'�豸�˿�',
            id:'dvr_port',
            width:220,
            allowBlank: false,
            type: 'int',
			maxValue:65535
        },{
            xtype:'textfield',
            fieldLabel:'DVR�û���',
            id:'dvr_user',
            width:220,            
            allowBlank: false,
			maxLength:24,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '24'}
        },{
            xtype:'textfield',
            fieldLabel:'DVR����',
            id:'dvr_password',
            width:220,             
            allowBlank: false,
			maxLength:24,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '24'}
        },{
			xtype:'checkbox',
			fieldLable:'',
			labelSeparator:'',
			boxLabel:'�Զ���ȡDVRͨ��',
			id:'dvr_autochnl',
			checked:true,
			listeners:{
				check:function(obj,status){
					if(status)
					{
						//ѡ��״̬
						win.findById('dvr_chnlnum').disable();
					}
					else
					{
						//δѡ��״̬
						win.findById('dvr_chnlnum').enable();
					}
				}
			}
		},{
			xtype:'textfield',
			fieldLabel:'DVRͨ����',
			id:'dvr_chnlnum',
			width:220,
			disabled:true
		},{
			xtype:'checkbox',
			fieldLable:'',
			id:'dvr_alarmNotify',
		    boxLabel:'�澯�ϱ�',
			labelSeparator:''
		}],
        buttons:[{
            text:'ȷ��',
            handler:function(){

            //��֤��
                var dvrName = win.findById('dvr_name').getValue();          
                var dvrPuid = win.findById('dvr_puid').getValue();
                
                if(dvrName == ''){
                        myAlert(null,"���Ʋ���Ϊ�գ�" , 180 ,0);   
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
                        myAlert(null,"�������ظ������������룡" , 230 ,0);
                        return;                   
                    }
					
                    if(dvrPuidLocal == dvrPuid){
                        myAlert(null,"DVR����ID���ظ������������룡��",280 , 0);
                        return;                   
                    } 
                }
				if(!UiUtil.checkField('dvr_name','DVR����'))
					return;
					
				var puidValue = win.findById('dvr_puid').getValue();
				if(!UiUtil.checkPstvNumber('dvr_puid','DVR����ID'))
					return;
				if(puidValue.length != 32)
				{
					myAlert(null,"DVR����ID������32λ����",250,0);
					return;
				}
				
				if(!UiUtil.checkEmpty('dvr_manufactory','���쳧��'))
					return;
								
				if(!UiUtil.checkIp('dvr_ip','�豸IP'))
					return;
					
				if(!UiUtil.checkPstvNumber('dvr_port','�豸�˿�'))
					return;
				if(parseInt($('#dvr_port').val()) > 65535)
				{
					myAlert(null,"�������˿���65535!",220,0);
					return;
				}
				
				
				if(!UiUtil.checkField('dvr_user','DVR�û���'))
					return;
					
				if(!UiUtil.checkField('dvr_password','DVR����'))
					return;
				
				if(!win.findById('dvr_chnlnum').disabled)
				{
					if(!UiUtil.checkPstvNumber('dvr_chnlnum','DVRͨ����'))
						return;
				}
            //��֤over
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
					   myAlert(null,"������豸�ɹ�!" , 200, 0);
					   
					   setTimeout(updateDvrList,2*1000);
					}else if(value == 2){
					   myAlert(null,"������豸ʧ�ܣ���������ӣ�",300,0);
					}        
			   },false);//loadXmlData over
             }//function over
        },{
            text:'ȡ��',
            handler:function(){
                win.close();
            }
        }
        ]        
    });
    win.show();   
};//��Ӱ�����Ӧ over

//�༭һ��dvr��Ϣ
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
            myAlert(null,"û��ѡ���κ�DVR",180,0);
            return;
        }
};//�༭һ��dvr��Ϣ over
//��Ӱ�����Ӧ
function editNewDvr(dvrName,dvrPuid,dvrIp,dvrPort,dvrUser,dvrPassword,dvrManufactory,configFile,dvralarm,autogetchnnum,chnlnum){
   Ext.QuickTips.init(); 
   var win = new Ext.Window({
        frame: true,
        title:'�޸�DVR',            
        width:350,
        modal : true,
        layout: 'form',
        items:[{
            xtype:'textfield',
            fieldLabel:'����',
            id:'dvr_name',
            width:220,            
            allowBlank: false,
            disabled:true
        },{
            xtype:'textfield',
            fieldLabel:'�豸����ID',
            id:'dvr_puid',
            width:220,
            allowBlank: false,
			maxLength:32,
			minLength:32,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '32'}
        },{
            xtype:'combo',
            fieldLabel:'���쳧��',
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
            fieldLabel:'DVR��ַ',
            id:'dvr_ip',
            width:220,
            allowBlank: false,
			maxLength:15,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '15'}
        },{
            xtype:'numberfield',
            fieldLabel:'DVR�˿ں�',
            id:'dvr_port',
            width:220,
            allowBlank: false,
			maxValue:65535
        },{
            xtype:'textfield',
            fieldLabel:'��½�û���',
            id:'dvr_user',
            width:220,
            allowBlank: false,
			maxLength:24,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '24'}
        },{
            xtype:'textfield',
            fieldLabel:'��½����',
            id:'dvr_password',
            width:220,
            allowBlank: false,
			maxLength:24,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '24'}
        },{
			xtype:'checkbox',
			fieldLable:'',
			labelSeparator:'',
			boxLabel:'�Զ���ȡDVRͨ��',
			id:'dvr_autochnl',
			checked:true,
			listeners:{
				check:function(obj,status){
					if(status)
					{
						//ѡ��״̬
						win.findById('dvr_chnlnum').disable();
					}
					else
					{
						//δѡ��״̬
						win.findById('dvr_chnlnum').enable();
					}
				}
			}
		},{
			xtype:'textfield',
			fieldLabel:'DVRͨ����',
			id:'dvr_chnlnum',
			width:220,
			disabled:true
		},{
			xtype:'checkbox',
			fieldLable:'',
			id:'dvr_alarmNotify',
		    boxLabel:'�澯�ϱ�',
			labelSeparator:''
		}],
        buttons:[{
            text:'ȷ��',
            handler:function(){
                            //��֤��
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
                                        myAlert(null,"DVR����ID���ظ���",200 , 0);
                                        return;                   
                                    }                                              
                                } 
                            }
						
						var puidValue = win.findById('dvr_puid').getValue();
						if(!UiUtil.checkPstvNumber('dvr_puid','DVR����ID'))
							return;
						if(puidValue.length != 32)
						{
							myAlert(null,"DVR����ID������32λ����",250,0);
							return;
						}
					
						if(!UiUtil.checkEmpty('dvr_manufactory','���쳧��'))
							return;
										
						if(!UiUtil.checkIp('dvr_ip','�豸IP'))
							return;
							
						if(!UiUtil.checkPstvNumber('dvr_port','�豸�˿�'))
							return;
						if(parseInt($('#dvr_port').val()) > 65535)
						{
							myAlert(null,"�������˿���65535!",220,0);
							return;
						}
					
						if(!UiUtil.checkField('dvr_user','DVR�û���'))
							return;
							
						if(!UiUtil.checkField('dvr_password','DVR����'))
							return;
						
						if(!win.findById('dvr_chnlnum').disabled)
						{
							if(!UiUtil.checkPstvNumber('dvr_chnlnum','DVRͨ����'))
								return;
						}
                        //��֤over
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
             	               myAlert(null,"�޸��豸�ɹ�!",200,0);
             	               setTimeout(updateDvrList,2*1000);
                            }else if(value == 2){
                               myAlert(null,"�޸��豸ʧ�ܣ������²�����",280,0);
                            }        
                       },false);//loadXmlData over
                }//function over
        },{
            text:'ȡ��',
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
};//��Ӱ�����Ӧ over
function char2character(manufactory,bReturnIsChar){
   /* var charHK = "haikang";
    var charDH = "dahua";
    var charBSR = "bluesky";
    var chHK = "����(haikang)";
    var chDH = "��(dahua)";
    var chBSR="��ɫ�Ǽ�(bluesky)";
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
		//��������
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
		//�����ĺ���ȥ��
		var nLeft = manufactory.indexOf("(");//�����ŵ�λ��
		var nRight = manufactory.indexOf(")");//�����ŵ�λ��
		strRet = manufactory.substr(nLeft+1,nRight-nLeft-1);
	}
	return strRet;
};

function deleteDvr(){
    var sm = grid.getSelectionModel();
    var store = grid.getStore();
    var view = grid.getView();
    if(!sm.hasSelection()){
        myAlert(null,"ûѡ���κ�DVR",180,0);
        return;   
    }
    
    Ext.Msg.show({
        title:"��ʾ",
        msg:"�Ƿ�ɾ��ѡ�е��豸��",
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
                    myAlert(null,"�ɹ�ɾ����"+ value+"��DVR�豸!",220,0);
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
                var  detailForADvr="<p style='margin:10px;color:#205699;'><label style='padding-left:48px;padding-right:10px;'>���ƣ�</label><span>"+json.kedacomxmldata.content.name+"</span></p>"+
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:13px;padding-right=10px;'>�豸����ID��</span><span>"+json.kedacomxmldata.content.puid+"</span></p>"+
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:38px;padding-right=10px;'>�豸IP��</span><span>"+json.kedacomxmldata.content.dvrip+"</span></p>"+                                                
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:12px;padding-right=10px;'>�豸�˿ںţ�</span><span>"+json.kedacomxmldata.content.dvrport+"</span></p>"+                        
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:12px;padding-right=10px;'>�豸�û�����</span><span>"+json.kedacomxmldata.content.dvrname+"</span></p>"+                        
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:0px;padding-right=10px;'>�豸ͨ��������</span><span>"+json.kedacomxmldata.content.channelnum+"</span></p>"+                        
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:24px;padding-right=10px;'>�豸���̣�</span><span>"+json.kedacomxmldata.content.dvrmanufactory+"</span></p>"+                                             
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:12px;padding-right=10px;'>���Զ˿ںţ�</span><span>"+json.kedacomxmldata.content.telnet+"</span></p>"+                        
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:12px;padding-right=10px;'>�ϱ��˿ںţ�</span><span>"+json.kedacomxmldata.content.startport+"</span></p>"+                        
                        "<p style='margin:10px;color:#205699;'><span style='padding-left:24px;padding-right=10px;'>������Ϣ��</span><span>"+json.kedacomxmldata.content.processname+"("+
                                        json.kedacomxmldata.content.processid+")"+"</span></p>"+
						"<p style='margin:10px;color:#205699;'><span style='padding-left:24px;padding-right=10px;'>�澯�ϱ���</span><span>" + UiUtil.parseBoolFromNum(json.kedacomxmldata.content.dvralarm) + "</span></p>"+
						"<p style='margin:10px;color:#205699;'><span style='padding-left:0px;padding-right=10px;'>�Զ���ȡͨ����</span><span>" + UiUtil.parseBoolFromNum(json.kedacomxmldata.content.autogetchnnum) +"</span></p>";
                var win = new Ext.Window({
                    frame: true,
                    title:'DVR��ϸ��Ϣ',            
                    width:350,
                    modal : true,
                    html: detailForADvr,
                    buttons:[{
                        text:'�ر�',
                        handler:function(){
                            win.close();
                        }
                    }]
                });
                win.show();
           },false);//loadXmlData over                       
        }else{
            myAlert(null,"û��ѡ���κ�DVR",180,0);
            return;
        }   
};

function stopDvr(){
    var sm = grid.getSelectionModel();
    var store = grid.getStore();
    var view = grid.getView();
	//view.getRows().length == 0;
    if(!sm.hasSelection()){
        myAlert(null,"û��ѡ���κ�DVR",180,0);
        return;   
    }
    Ext.Msg.show({
        title:"��ʾ",
        msg:"�Ƿ�ֹͣѡ�е��豸��",
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
                    myAlert(null , "�ɹ�ֹͣ��"+ value+"��DVR�豸!" , 250 , 0);
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
        myAlert(null,"û��ѡ���κ�DVR",180,0);
        return;   
    }
    
    Ext.Msg.show({
        title:"��ʾ",
        msg:"�Ƿ�������ѡ�е��豸",
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
                    myAlert(null,"�ɹ�������"+ value+"��DVR�豸!", 250 , 0);
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

//��ȡ��������
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
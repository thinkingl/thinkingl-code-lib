var data;
var g_mtulist;
var g_mtuStore;
var g_mtuGrid;

var formates= new Ext.data.SimpleStore({
	fields:['id','formate'],
	data:[['106','H264'],['97','MPEG4']]
});   
var genres = new Ext.data.SimpleStore({
	fields:['id','genre'],
	data:[['1','QCIF'],['2','CIF'],['3','2CIF'],['3','D1']]
}); 
var rates = new Ext.data.SimpleStore({
	fields:['id','rate'],
	data:[['1','256'],['2','384'],['3','512'],['4','800'],['5','1024']]
});
   
function goToMTUDetails(){
 	ResourceLoader.loadTemplate("mtuDetails",function(html){
        $(".rightTab").html(html);
          
          $(".mainEdit").attr("disabled","true");      
            //��Ӱ�����Ӧ
           $('#rule_add').click(addNewRule);
           $('#rule_alert').click(editRule); 
           $('#rule_delete').click(deleteRules); 
           $('#rule_apply').click(applyRuleChange); 
           $('#rule_extport').click(exportRules);
		    
		   $('#idMtuEdit').click(editMtu);
		   $('#idMtuStop').click(stopMtu);
		   $('#idMtuStart').click(startMtu);
		   
          showMTURules();
		  showMTUList();
		  
		  mtuStatusTimer = true;
		  $('body').everyTime(6000 , "updateMTUStatus" , updateMTUStatus);
          button_hover_function();  
    });
};

/*
 * ����ת����������ļ�
 */
 function exportRules()
 {
	CMonMsg.setId(G100_COMMAND.COMMAND_EXPORTCMD);
	CMonMsg.setMsgNode("rules","rules");
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
		if(isErrorCommand(json)===false)
            return;
		var value = json.kedacomxmldata.content.status;
		if(value == 1)
		{
			downrule.click();
		}
		else
		{
			myAlert(null,"��������ʧ�ܣ�",180,0);	
		}
	},false);
 }
 
function form4_submit(){
    //�˴����Լ�����֤����
	var fileName = $("#myfile").val();
	if(fileName.lastIndexOf("rules.ini") == -1)
	{
		myAlert(null,"��ѡ����ȷ�Ĺ����ļ�",230,0);
		return;
	}
	
	ResourceLoader.uploadFile(document.form4,function(json){
		if(!json){
			return;
		}
		myAlert(null,"�ļ��ϴ��ɹ�������������ϵͳ��",280,0);
	});
};
//���ӷ������õ����е�DVR��Ϣ
function showMTURules(){
        CMonMsg.setId(G100_COMMAND.COMMAND_GETMTUDETAIL);
    	CMonMsg.setMsgNode("name","mturules"); 	
    	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
    	{
	       if(isErrorCommand(json)==false)
	        return; 

          /* $("#mtu_puid").html(json.kedacomxmldata.content.puid);
           $("#mtu_status").html(json.kedacomxmldata.content.status);           
           $("#mtu_maxtrans").html(json.kedacomxmldata.content.maxexchange);
           $("#mtu_curtrans").html(json.kedacomxmldata.content.curexchange);  
           $("#mtu_telnetport").html(json.kedacomxmldata.content.telnet);*/
		   
           data = json.kedacomxmldata.content.regular;
           addMtuRule(data);              
    	},false);
}; 
 
function showMTUList(){
		CMonMsg.setId(G100_COMMAND.COMMAND_GETMTU);
		CMonMsg.setMsgNode("name","mtulist");
		ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
			
			if(isErrorCommand(json)==false)
	        	return;	
				
			printf("*************get mtulist***********");
			g_mtulist = json.kedacomxmldata.content.mtulist;
			addMtuList();
		},false);

}

function formatDesc(val)
{
	var strRet = "";
	for(var count = 0; count < formates.getTotalCount(); count++)
	{
		var rec = formates.getAt(count);
		if(val == rec.data.id)
		{
			strRet = rec.data.formate;
		}
	}
	return strRet;
}

function addMtuRule(json){
//    alert(json.length);
    $("#mturules").html('');
    var sm = new Ext.grid.CheckboxSelectionModel(); 
	var cm = new Ext.grid.ColumnModel([ 
		new Ext.grid.RowNumberer(), 
		sm, 
		{header:'puid',dataIndex:'rulepuid',width:250}, 
		{header:'��ʽ',dataIndex:'ruleformat',renderer:formatDesc}, 
		{header:'����',dataIndex:'rulerate'}, 
		{header:'�ֱ���',dataIndex:'rulesize'} 
	]); 


	store = new Ext.data.Store({ 
//                     proxy: new Ext.data.MemoryProxy(json), 
		  proxy: new Ext.data.PagingMemoryProxy(json),                       
//                   proxy: new Ext.data.PagingMemoryProxy(json),
		reader: new Ext.data.JsonReader({
			id:'rulepuid',
			root:'rule'
		}, [ 
			{name: 'rulepuid'}, 
			{name: 'ruleformat'}, 
			{name: 'rulerate'}, 
			{name: 'rulesize'} 
		]) 

	}); 
	store.load(); 

	grid = new Ext.grid.GridPanel({ 
		renderTo: 'mturules', 
		store: store,
		height: 270,
		width : 605,
		stripeRows:true,
		
//��ҳ��toobarȥ��                    
//                    tbar:new Ext.PagingToolbar(
//                    {
//                    	items:[
//                         {
//                            xtype:'label',
//                            fieldLabel: 'Name',
//                            text:'DVR״̬:'     
//                        }                       
//                        ,{
//                            xtype:'textfield',
//                            fieldLabel: 'Name',  
//                            region:'west'   
//                        }],                                         
//                    	pageSize:10,
//                    	store:store


//                    }),
		cm: cm, 
		sm: sm 
	}); 
};
function addMtuList()
{
	//g_mtulist = {mtu:{puid:1111111111111,telnet:2502,status:1,maxexchange:8}};
	g_mtuStore = new Ext.data.Store({
		proxy: new Ext.data.MemoryProxy(g_mtulist),
		reader: new Ext.data.JsonReader({id:'puid',root:'mtu'},[
		 {name:'name'},
		 {name:'puid'},
		 {name:'telnet'},
		 {name:'status'},
		 {name:'maxexchange'}
		 ])
	});
	g_mtuStore.load();
	
	
	g_mtuGrid = new Ext.grid.GridPanel({
		renderTo:'mtulist',
		width : 605,
		height: 270,
		store:g_mtuStore,
		stripeRows:true,
		columns:[
			new Ext.grid.RowNumberer(),
			{header:"ID",dataIndex:'puid',width:250,align:"center"},
			{header:"���Զ˿�",dataIndex:'telnet',align:"center"},
			{header:"״̬",dataIndex:'status',align:"center"},
			{header:"���ת��·��",dataIndex:'maxexchange',align:"center"}
		],
		sm:new Ext.grid.RowSelectionModel({
				singleSelect:true
			})
	});
}


function addNewRule(){
  Ext.QuickTips.init();
  
   var win = new Ext.Window({
//        frame: true,
        title:'�½�ת�����',            
        width:350,
        modal : true,
        layout: 'form',
        items:[{
            xtype:'textfield',
            fieldLabel:'�豸����ID',
            id:'rule_puid',
            width:200,           
            allowBlank: false,
			maxLength:32,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '32'}
        },{
            xtype:'combo',
            fieldLabel:'��ʽ',
            id:'rule_format',
            mode:'local',
            store:formates,
            displayField:'formate',
            triggerAction: 'all',            
            width:120,
			editable:false,
            allowBlank: false,
			valueField:'id'
        },{
            xtype:'combo',
            fieldLabel:'����',
            id:'rule_rate',
            mode:'local',
            store:rates,
            displayField:'rate',
            triggerAction: 'all',   
			editable:false,
            width:120,
            allowBlank: false
        },{
            xtype:'combo',
            fieldLabel:'�ֱ���',
            id:'rule_cif',
            mode:'local',
            store:genres,
            displayField:'genre',
            width:120,
			editable:false,
            triggerAction: 'all',
            allowBlank: false
        }],
        buttons:[{
            text:'ȷ��',
            handler:function(){  
                 var newData = {
				 	rulepuid:win.findById('rule_puid').getValue(),
					ruleformat:win.findById('rule_format').getValue(),
					rulerate: win.findById('rule_rate').getValue(),
					rulesize: win.findById('rule_cif').getValue()
				    };
				    
//��֤       
                var dvrPuid = newData.rulepuid; 
              	for(var i = 0 ; i < store.data.length ; i++)  
                {
                    var record = store.getAt(i);
                    
                    var puid = record.get("rulepuid");
                    if(dvrPuid == puid){
                        myAlert(null,"DVR����ID���ظ���",200 , 0);
                        return;                   
                    }                      
                }  
				
				var ruleValue = win.findById('rule_puid').getValue();
				if(ruleValue != "default")
				{
					if(!UiUtil.checkPstvNumber('rule_puid','�豸����ID'))
					{
						return;	
					}
					
					if(ruleValue.length != 32)
					{
						myAlert(null,"�豸����ID������32λ����",240,0);
						return;
					}
				}
					
				if(!UiUtil.checkEmpty('rule_format','��ʽ'))
					return;
					
				if(!UiUtil.checkEmpty('rule_rate','����'))
					return;
					
				if(!UiUtil.checkEmpty('rule_cif','�ֱ���'))
					return;
//��֤����				    			    
				    
		      var newRec = new store.recordType(newData);
		      store.add(newRec);     
              store.commitChanges();  
              win.close();
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
};
function editRule(){
        var sm=grid.getSelectionModel();      
        if(sm.hasSelection()){
              var sel = sm.getSelected();           
            editNewRule(sel.data.rulepuid,sel.data.ruleformat,sel.data.rulerate,sel.data.rulesize,sel);             
                            
        }else{
            myAlert(null,"û��ѡ���κ�ת�����",220,0);
            return;
        }

};
function editNewRule(puid,format,rate,cif,sel){
  Ext.QuickTips.init();

   var win = new Ext.Window({
        frame: true,
        title:'�޸�ת�����',            
        width:350,
        modal : true,
        layout: 'form',
        items:[{
            xtype:'textfield',
            fieldLabel:'�豸����ID',
            id:'rule_puid',
            width:200,           
            allowBlank: false,
            disabled:true
        },{
            xtype:'combo',
            fieldLabel:'��ʽ',
            id:'rule_format',
            mode:'local',
            store:formates,
            displayField:'formate',
			editable:false,
            width:120,
            triggerAction: 'all',  
            allowBlank: false,
			valueField:'id'
        },{
            xtype:'combo',
            fieldLabel:'����',
            id:'rule_rate',
            mode:'local',
            store:rates,
            displayField:'rate',
			editable:false,
            width:120,
            triggerAction: 'all',  
            allowBlank: false
        },{
            xtype:'combo',
            fieldLabel:'�ֱ���',
            id:'rule_cif',
            mode:'local',
            store:genres,
            displayField:'genre',
			editable:false,
            width:120,
            triggerAction: 'all',  
            allowBlank: false
        }],
        buttons:[{
            text:'ȷ��',
            handler:function(){  
                 var newData = {
				 	rulepuid:win.findById('rule_puid').getValue(),
					ruleformat:win.findById('rule_format').getValue(),
					rulerate: win.findById('rule_rate').getValue(),
					rulesize: win.findById('rule_cif').getValue()
				    };
				    
//��֤       
                if(!UiUtil.checkEmpty('rule_format','��ʽ'))
					return;
					
				if(!UiUtil.checkEmpty('rule_rate','����'))
					return;
					
				if(!UiUtil.checkEmpty('rule_cif','�ֱ���'))
					return;
				
				var dvrPuid = newData.rulepuid;
              	for(var i = 0 ; i < store.data.length ; i++)  
                {
                    var record = store.getAt(i);
                    
                    var puid = record.get("rulepuid");
                    if(dvrPuid == puid){
                        sel.set('ruleformat',newData.ruleformat);
                        sel.set('rulerate',newData.rulerate);                        
                        sel.set('rulesize',newData.rulesize);                                                                                                                                           
                        sel.commit(); 
                        win.close();	           
                        return;                   
                    }                      
                }              

//��֤����			    
				 var newRec = new store.recordType(newData);
		         store.add(newRec);       
                 newRec.commit();

                }//function over
        },{
            text:'ȡ��',
            handler:function(){
                win.close();
            }
        }
        ]        
    });
    win.findById('rule_puid').setValue(puid);   
    win.findById('rule_rate').setValue(rate);
    win.findById('rule_format').setValue(format);   
    win.findById('rule_cif').setValue(cif);  
    win.show();   
};
function deleteRules(){
    var sm = grid.getSelectionModel();
    var store = grid.getStore();
    var view = grid.getView();
    if(!sm.hasSelection()){
        myAlert(null,"û��ѡ���κ�ת�����",220,0);
        return;   
    }
    
    Ext.Msg.show({
        title:"��ʾ",
        msg:"�Ƿ�ɾ��ѡ�й���",
		width:220,
        icon:Ext.MessageBox.INFO,
        buttons:{
                    yes:true,
                    no:true
        },
        fn:function(btn){
            if(btn =="yes"){
				var length = view.getRows().length;
				var arrayRecord = [];
                for(var i = 0; i < length; i ++){
					 if(sm.isSelected(i)){
                        var record = store.getAt(i);
						arrayRecord.push(record);
                     }
                } 
				store.remove(arrayRecord);
				store.commitChanges();
				arrayRecord = null;
            }
            else
            {
                return;
            }
        }
    }); 
};

//Ӧ�ð�ť��������Ӧ
function applyRuleChange(){
    var header = CMonMsg.getHeader();
    var send2SeverXml = header + "<kedacomxmldata>"
				   + "<command>16</command>"
				   + "<content><rulelist>";

    for(var i=0;i<store.data.length;i++){
        send2SeverXml = send2SeverXml + "<rule><rulepuid>" +store.getAt(i).get("rulepuid")+"</rulepuid>";
        send2SeverXml = send2SeverXml + "<ruleformat>" +store.getAt(i).get("ruleformat")+"</ruleformat>";
        send2SeverXml = send2SeverXml + "<rulerate>" +store.getAt(i).get("rulerate")+"</rulerate>";
        send2SeverXml = send2SeverXml + "<rulesize>" +store.getAt(i).get("rulesize")+"</rulesize></rule>";
    }	
    			   
	send2SeverXml = send2SeverXml +"</rulelist><rulenum>"+ store.data.length +"</rulenum></content>" + "</kedacomxmldata>";

   	ResourceLoader.loadXmlData(send2SeverXml,function(json){
        if(isErrorCommand(json)===false)
           return;
        var value = json.kedacomxmldata.content.status;
        if (value == 1){
           myAlert(null,"����ת�����ɹ���������G100������Ч",320,0);
        }else if(value == 2){
           myAlert(null,"Ӧ��ת�����ʧ��,�����²�����",280,0);
        }        
    },false);//loadXmlData over
};

function stopMtu()
{
	var sm = g_mtuGrid.getSelectionModel();
	if(!sm.hasSelection())
	{
		myAlert(null,"û��ѡ���κ�ת���������",250,0);
        return;
	}
	
	Ext.Msg.show({
        title:"��ʾ",
        msg:"�Ƿ�ֹͣMTU",
		width:200,
        icon:Ext.MessageBox.INFO,
        buttons:{
                    yes:true,
                    no:true
        },
        fn:function(btn){
            if(btn =="yes"){
				var sel = sm.getSelected();	
				CMonMsg.setId(G100_COMMAND.COMMAND_STOPMTU);
				CMonMsg.setMsgNode("puid",sel.data.puid);
				ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
					if(isErrorCommand(json)===false)
						return;
					var value = json.kedacomxmldata.content.status;
					if (value == 1)
					{
						myAlert(null,"ֹͣ�ɹ�",200,0);	
					}
					else
					{
						myAlert(null,"ֹͣʧ��",200,0);	
					}
				});
            }
            else
            {
                return;
            }
        }
    }); //end Ext.Msg.show
	
}

function startMtu()
{
	var sm = g_mtuGrid.getSelectionModel();
	if(!sm.hasSelection())
	{
		myAlert(null,"û��ѡ���κ�ת���������",250,0);
        return;
	}
	
	Ext.Msg.show({
        title:"��ʾ",
        msg:"�Ƿ�����MTU",
		width:200,
        icon:Ext.MessageBox.INFO,
        buttons:{
                    yes:true,
                    no:true
        },
        fn:function(btn){
            if(btn =="yes"){
				var sel = sm.getSelected();
				CMonMsg.setId(G100_COMMAND.COMMAND_STARTMTU);
				CMonMsg.setMsgNode("puid",sel.data.puid);
				ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
					if(isErrorCommand(json)===false)
						return;
					var value = json.kedacomxmldata.content.status;
					if (value == 1)
					{
						myAlert(null,"�����ɹ�",200,0);	
					}
					else
					{
						myAlert(null,"����ʧ��",200,0);	
					}
				});
            }
            else
            {
                return;
            }
        }
    }); //end Ext.Msg.show
}

function editMtu()
{
	Ext.QuickTips.init();
	var sm = g_mtuGrid.getSelectionModel();
	if(!sm.hasSelection())
	{
		myAlert(null,"ûѡ���κ�MTU",180,0);
		return;
	}
	
	var sel = sm.getSelected();
	var mtuWin =  new Ext.Window({
		frame: true,
        title:'�޸�MTU',           
        width:370,
        modal : true,
        layout: 'form',
		items:[{
			xtype:'textfield',
            fieldLabel:'�������',
            id:'mut_puid',
            width:220,
            allowBlank: false,
			maxLength:32,
			minLength:32,
			autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '32'}
		}],
		buttons:[{
			text:"ȷ��",
			handler:function(){
				
				var puidValue = mtuWin.findById('mut_puid').getValue();
				if(!UiUtil.checkPstvNumber('mut_puid','�������'))
					return;
				if(puidValue.length != 32)
				{
					myAlert(null,"�����������32λ����",250,0);
					return;
				}
				if(puidValue == sel.data.puid)
				{
					return;
				}
				CMonMsg.setId(G100_COMMAND.COMMAND_CHANGEMTU);
				CMonMsg.setMsgNode("name",sel.data.name);
				CMonMsg.setMsgNode("puid",mtuWin.findById('mut_puid').getValue());
				ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
					if(isErrorCommand(json)===false)
						return;
					var value = json.kedacomxmldata.content.status;
					if (value == 1)
					{
						sel.data.puid = mtuWin.findById('mut_puid').getValue();
						sel.commit();
						
						mtuWin.close();
						myAlert(null,"�޸ĳɹ�",200,0);	
					}
					else
					{
						myAlert(null,"�޸�ʧ��",200,0);	
					}
				});
			}
		},{
			text:"ȡ��",
			handler:function(){
				mtuWin.close();	
			}
		}]
	});
	mtuWin.findById('mut_puid').setValue(sel.data.puid);
	mtuWin.show();
}

function  updateMTUStatus(){
    if(mtuStatusTimer==false){
        $('body').stopTime("updateMTUStatus");  
        return;  
    }
	
    CMonMsg.setId(G100_COMMAND.COMMAND_GETALTERED);
	CMonMsg.setMsgNode("range","2"); //��ΪDVR����2Ϊmtu	
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
	{
       if(isErrorCommand(json)===false)
            return; 
       if(json.kedacomxmldata.content.devnum == "0")
            return;
       if(json.kedacomxmldata.content.devnum == "1"){
            for(var i = 0 ; i < g_mtuStore.getTotalCount(); i++){
                var record = g_mtuStore.getAt(i);
                var puid = record.get("puid");
                if(puid == json.kedacomxmldata.content.dev.puid){
                    record.set("status" , json.kedacomxmldata.content.dev.status);               
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
                        record.commit();
                    } //if over       
                } //for over
             }//for over
       }//else over  
    },false);//loadXmlData��over
}
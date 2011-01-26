

//�洢���о�����Ϣ
var storeMatrixs;
var storeCamType = new Ext.data.SimpleStore({
	fields:['id','type'],
	data:[[1,"SONY"],[2,"AD"],[3,"PELCO"],[4,"PIH"],[5,"PELCO_P"],[6,"PARCO"],[7,"AB"],[8,"YUNTAI"],[9,"����"],
		  [10,"�Ű�"],[11,"AD168"],[12,"������"],[13,"JN"],[14,"����650"],[15,"����850"],[16,"������"],[17,"PANASONIC-OLD"],[18,"PANASONIC-NEW"]]
});

var cameraStore;
var encoderStore;
var cameraGrid;
var encoderGrid;
//������������ʾ����
var showData = [];
var matrixId;

var curSelectedEncoderGuid = null;
var combo;
var cameraList = [];
var encoderList = [];

function addCamera()
{	
	Ext.QuickTips.init();
	var winCamera = new Ext.Window({
		layout:'form',
		autoHeight:true,
		width:310,
		title:'��������',
		resizable:false,
		modal:true,
		items:[
			   	{xtype:'textfield',fieldLabel:'����',id:'cameraName',allowBlank:false,width:150,maxLength:30,
					autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
			   	{xtype:'combo',fieldLabel:'����',id:'cameraType',allowBlank:false,width:120,store:storeCamType,
					valueField:'id',displayField:'type',mode:'local',editable:false,triggerAction:'all'},
				//{xtype:'combo',fieldLabel:'���ӵľ���',id:'cameraMatrix',allowBlank:false,width:120,store:storeMatrixs,mode:'local',
//					valueField:'index',displayField:'name',editable:false,triggerAction:'all'},
				{xtype:'numberfield',fieldLabel:'����˿�',id:'cameraPort',allowBlank:false,width:80,allowNegative:false,
					allowDecimals:false},
				{xtype:'numberfield',fieldLabel:'��ַ��',id:'cameraAddr',allowBlank:false,width:80,allowNegative:false,
					allowDecimals:false}
			  ],
		buttons:[
					{	text:'ȷ��',
						handler:function(){
							
							if(!UiUtil.checkField('cameraName','���������'))
								return;
							if(!UiUtil.checkEmpty('cameraType','���������'))
								return;
							if(!UiUtil.checkEmpty('cameraPort','����˿�'))
								return;
							if(!UiUtil.checkPstvNumber('cameraPort','����˿�'))
								return;
							if(!UiUtil.checkEmpty('cameraAddr','�������ַ��'))
								return;
							if(!UiUtil.checkNumber('cameraAddr','�������ַ��'))
								return;
								
							if(!isCanUseInport(matrixId,winCamera.findById('cameraPort').getValue()))
							{
								return;
							}
							
							if(!isCanUseCamAddr(matrixId,winCamera.findById('cameraAddr').getValue()))
							{
								return;	
							}
							CMonMsg.setId(G200_ET.BS_G200MS_ADD_CAMERA_REQ);							
							CMonMsg.setMsgNode("matrixid",matrixId);
							CMonMsg.setMsgNode("videoport",winCamera.findById('cameraPort').getValue());
							CMonMsg.setMsgNode("type",winCamera.findById('cameraType').getValue());
							CMonMsg.setMsgNode("name",winCamera.findById("cameraName").getValue());
							CMonMsg.setMsgNode("addr",winCamera.findById("cameraAddr").getValue());
							ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
								if(!json)
								{
									return;
								}
								if(json.kedacomxmldata.command != G200_ET.G200_BS_ADD_CAMERA_ACK)
								{
									myAlert("���ʧ��",ERROR_CODE[json.kedacomxmldata.content.status],250,0);
									return;
								}
								var newData = {
									guid:json.kedacomxmldata.content.guid,
									name:winCamera.findById('cameraName').getValue(),
									status:1,
									matrixid:matrixId,
									videoport:winCamera.findById('cameraPort').getValue(),
									type:winCamera.findById('cameraType').getValue(),
									addr:winCamera.findById('cameraAddr').getValue()
								};
								var newRec = new cameraStore.recordType(newData);
								cameraStore.add(newRec);
								cameraStore.commitChanges();
								
								winCamera.close();
								myAlert(null,"��ӳɹ�,����������ϵͳ!",250,0);
							});
						}
					},
					{	text:'ȡ��',
						handler:function(){
								winCamera.close();
						}
					}
				]
	});
	winCamera.show();
}

function deleteCamera()
{
	var sm = cameraGrid.getSelectionModel();
	if(sm.hasSelection())
	{
		Ext.Msg.confirm('��ʾ',"ɾ�����������",function(btn,text){
			if(btn == "yes"){
				var sel = sm.getSelected();
				CMonMsg.setId(G200_ET.BS_G200MS_DELETE_CAMERA_REQ);
				CMonMsg.setMsgNode("guid",sel.data.guid);
				ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
					if(!json)
					{
						return;
					}	
					if(json.kedacomxmldata.command != G200_ET.G200MS_BS_DELETE_CAMERA_ACK)
					{
						myAlert(null,"ɾ�������ʧ�ܣ�",0,0);
						return;
					}
					cameraStore.remove(sel);
					cameraStore.commitChanges();
					cameraGrid.getView().refresh();
					myAlert(null,"ɾ���ɹ�,����������ϵͳ!",250,0);
				});
			}
			else
			{
				
			}
		});
	}
	else
	{
		myAlert(null,"��ѡ����Ҫɾ�����������",230,0);	
	}
}

function updateCamera()
{
	var sm = cameraGrid.getSelectionModel();

	if(sm.hasSelection())
	{
		var sel = sm.getSelected();
		
		Ext.QuickTips.init();
		var winCamera = new Ext.Window({
			layout:'form',
			autoHeight:true,
			width:310,
			title:'�޸������',
			resizable:false,
			modal:true,
			items:[
				{xtype:'textfield',fieldLabel:'����',id:'cameraName',allowBlank:false,width:150,maxLength:30,
					autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
			   	{xtype:'combo',fieldLabel:'����',id:'cameraType',allowBlank:false,width:120,store:storeCamType,
					valueField:'id',displayField:'type',mode:'local',editable:false,triggerAction:'all'},
				/*{xtype:'combo',fieldLabel:'���ӵľ���',id:'cameraMatrix',allowBlank:false,width:120,store:storeMatrixs,mode:'local',
					valueField:'index',displayField:'name',editable:false,triggerAction:'all'},*/
				{xtype:'numberfield',fieldLabel:'����˿�',id:'cameraPort',allowBlank:false,width:80,allowNegative:false,
					allowDecimals:false},
				{xtype:'numberfield',fieldLabel:'��ַ��',id:'cameraAddr',allowBlank:false,width:80,allowNegative:false,
					allowDecimals:false}
			],
			buttons:[
						{
							text:'ȷ��',
							handler:function(){
								
								if(!UiUtil.checkField('cameraName','���������'))
									return;
								if(!UiUtil.checkEmpty('cameraType','���������'))
									return;
								if(!UiUtil.checkEmpty('cameraPort','����˿�'))
									return;
								if(!UiUtil.checkPstvNumber('cameraPort','����˿�'))
									return;
								if(!UiUtil.checkEmpty('cameraAddr','�������ַ��'))
									return;
								if(!UiUtil.checkNumber('cameraAddr','�������ַ��'))
									return;
								if(!isCanUseInport(matrixId,winCamera.findById('cameraPort').getValue(),sel.data.guid))
								{
									return;
								}
								
								if(!isCanUseCamAddr(matrixId,winCamera.findById('cameraAddr').getValue(),sel.data.guid))
								{
									return;	
								}
								//����Ϣ�����������޸�
								CMonMsg.setId(G200_ET.BS_G200MS_UPDATE_CAMERA_REQ);
								CMonMsg.setMsgNode("guid",sel.data.guid);
								CMonMsg.setMsgNode("name",winCamera.findById("cameraName").getValue());
								CMonMsg.setMsgNode("matrixid",matrixId);
								CMonMsg.setMsgNode("videoport",winCamera.findById('cameraPort').getValue());
								CMonMsg.setMsgNode("type",winCamera.findById('cameraType').getValue());								
								CMonMsg.setMsgNode("addr",winCamera.findById("cameraAddr").getValue());
								ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
									if(!json)
									{
										return;
									}
									if(json.kedacomxmldata.command != G200_ET.G200MS_BS_UPDATE_CAMERA_ACK)
									{
										myAlert(null,"�޸�ʧ�ܣ�",0,0);
										return;
									}
									
									//��������ӵ����ش洢��
									sel.set("name",winCamera.findById("cameraName").getValue());
									sel.set("type",winCamera.findById("cameraType").getValue());
									sel.set("matrixid",matrixId);
									sel.set("videoport",winCamera.findById("cameraPort").getValue());
									sel.set("addr",winCamera.findById("cameraAddr").getValue());
									sel.commit();
									
									//cameraStore.filter('matrixid',matrixId,false,false);
									winCamera.close();
									myAlert(null,"�޸ĳɹ�,����������ϵͳ!",250,0);
								});
								
							}
						},
						{
							text:'ȡ��',
							handler:function(){
								winCamera.close();
							}
						}
					]
		});
	
		winCamera.findById("cameraName").setValue(sel.data.name);
		//winCamera.findById("cameraMatrix").setValue(sel.data.matrixid);
		winCamera.findById("cameraPort").setValue(sel.data.videoport);
		winCamera.findById("cameraType").setValue(sel.data.type);
		winCamera.findById("cameraAddr").setValue(sel.data.addr);
		winCamera.show();
	}
	else
	{
		myAlert(null,"��ѡ����Ҫ�޸ĵ��������",230,0);	
	}
}


function addEncoder()
{
	Ext.QuickTips.init();
	var winEncoder = new Ext.Window({
		layout:'form',
		autoHeight:true,
		width:310,
		title:'��ӱ�����',
		resizable:false,
		modal:true,
		items:[
			  	{xtype:'textfield',fieldLabel:'����',width:150,id:'encoderName',allowBlank:false,maxLength:30,
					autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
			  	{xtype:'numberfield',fieldLabel:'�˿���',width:80,id:'encoderPorts',allowBlank:false,
					allowNegative:false,allowDecimals:false,maxValue:64},
				//{xtype:'combo',fieldLabel:'���ӵľ���',width:120,id:'encoderMatrix',allowBlank:false,editable:false,triggerAction:'all',
//					store:storeMatrixs,mode:'local',valueField:'index',displayField:'name'},
				{xtype:'checkbox',fieldLabel:'',id:'encoderConn',boxLabel:'�������������',labelSeparator:''}
			  ],
		buttons:[
					{	text:'ȷ��',
						handler:function(){		
						
							if(!UiUtil.checkField('encoderName','����������'))
								return;
							if(!UiUtil.checkEmpty('encoderPorts','��ƵԴ�˿���'))
								return;
							if(!UiUtil.checkPstvNumber('encoderPorts','��ƵԴ�˿���'))
								return;
							
							if(parseInt($('encoderPorts').value) > 64)
							{
								myAlert(null,"���������ƵԴ�˿���64!",250,0);
								return;
							}
							
							CMonMsg.setId(G200_ET.BS_G200MS_ADD_ENCODER_REQ);
							CMonMsg.setMsgNode("matrixid",matrixId);
							CMonMsg.setMsgNode("name",winEncoder.findById("encoderName").getValue());
							CMonMsg.setMsgNode("link",UiUtil.parseBool(winEncoder.findById('encoderConn').getValue()));
							CMonMsg.setMsgNode("portnum",winEncoder.findById('encoderPorts').getValue());
							ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
								if(!json)
								{
									return false;
								}
								if(json.kedacomxmldata.command != G200_ET.G200_BS_ADD_ENCODER_ACK)
								{
									myAlert("���ʧ��",ERROR_CODE[json.kedacomxmldata.content.status],250,0);
									return false;
								}
								var portNum = winEncoder.findById('encoderPorts').getValue();
								var newData;
								var newRec;
								for(var i = 1; i<= portNum; i++)
								{
									newData = {
										guid:json.kedacomxmldata.content.guid,
										name:winEncoder.findById('encoderName').getValue(),
										matrixid:matrixId,							
										link:UiUtil.parseBool(winEncoder.findById('encoderConn').getValue()),
										portnum:winEncoder.findById('encoderPorts').getValue(),
										encoderport:i,
										matrixport:0,
										status:0,
										serialstatus:0
									};
									newRec = new encoderStore.recordType(newData);
									encoderStore.add(newRec);
								}
								encoderStore.commitChanges();
								encoderGrid.getView().focusRow(encoderStore.getCount() - 1);
								winEncoder.close();
								myAlert(null,"��ӳɹ�,����������ϵͳ!",250,0);
							});
						}
					},
					{	text:'ȡ��',
						handler:function(){
								winEncoder.close();
						}
					}
				]
	});
	winEncoder.show();
}
function updateEncoder()
{
	if(curSelectedEncoderGuid)
	{
		var recs = encoderStore.query('guid',curSelectedEncoderGuid);
		
		Ext.QuickTips.init();
		var winEncoder = new Ext.Window({
			layout:'form',
			autoHeight:true,
			width:360,
			title:'�޸ı�����',
			resizable:false,
			items:[
				{xtype:'textfield',fieldLabel:'���',id:'encoderGuid',readOnly:true,width:230},
				{xtype:'textfield',fieldLabel:'����',width:150,id:'encoderName',allowBlank:false}, 
			  	{xtype:'numberfield',fieldLabel:'�˿���',width:80,id:'encoderPorts',allowBlank:false,disabled:true,allowNegative:false},
				/*{xtype:'combo',fieldLabel:'���ӵľ���',width:120,id:'encoderMatrix',allowBlank:false,editable:false,triggerAction:'all',
					store:storeMatrixs,mode:'local',valueField:'index',displayField:'name'},*/
				{xtype:'checkbox',fieldLabel:'',id:'encoderConn',boxLabel:'�������������',labelSeparator:''}
			],
			modal:true,
			buttons:[
						{
							text:'ȷ��',
							handler:function(){								
								if(!UiUtil.checkField('encoderName','����������'))
									return;	
									
								//����Ϣ�����������޸�
								CMonMsg.setId(G200_ET.BS_G200MS_UPDATE_ENCODER_REQ);
								CMonMsg.setMsgNode("guid",curSelectedEncoderGuid);
								CMonMsg.setMsgNode("name",winEncoder.findById("encoderName").getValue());
								CMonMsg.setMsgNode("link",UiUtil.parseBool(winEncoder.findById('encoderConn').getValue()));
								CMonMsg.setMsgNode("matrixid",matrixId);
								ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
									if(!json)
									{
										return;
									}
									if(json.kedacomxmldata.command != G200_ET.G200MS_BS_UPDATE_ENCODER_ACK)
									{										
										myAlert(null,"�޸�ʧ�ܣ�",0,0);
										return;
									}
									recs.each(function(item){
										item.data.name = winEncoder.findById("encoderName").getValue();
										item.data.link = UiUtil.parseBool(winEncoder.findById('encoderConn').getValue());  
									},this);
									encoderStore.commitChanges();
									encoderStore.filter('matrixid',matrixId,false,false);
									winEncoder.close();
									myAlert(null,"�޸ĳɹ�,����������ϵͳ!",250,0);
								});
								
							}
						},
						{
							text:'ȡ��',
							handler:function(){
								winEncoder.close();
							}
						}
					]
		});
		winEncoder.findById('encoderGuid').setValue(recs.itemAt(0).data.guid);
		winEncoder.findById("encoderName").setValue(recs.itemAt(0).data.name);
		//winEncoder.findById("encoderMatrix").setValue(recs.itemAt(0).data.matrixid);
		winEncoder.findById("encoderConn").setValue(recs.itemAt(0).data.link);
		winEncoder.findById("encoderPorts").setValue(recs.itemAt(0).data.portnum);
		winEncoder.show();
	}
	else
	{
		myAlert(null,"��ѡ����Ҫ�޸ĵı�������",230,0);	
	}
}


function deleteEncoder()
{
	if(curSelectedEncoderGuid)
	{
		Ext.Msg.confirm('��ʾ',"ɾ��������",function(btn,text){
			if(btn == "yes"){
				CMonMsg.setId(G200_ET.BS_G200MS_DELETE_ENCODER_REQ);
				CMonMsg.setMsgNode("guid",curSelectedEncoderGuid);
				ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
					if(!json)
					{
						return;
					}	
					if(json.kedacomxmldata.command == G200_ET.G200MS_BS_DELETE_ENCODER_ACK)
					{
						//ɾ��һ������
						encoderStore.each(function(rec){
							if(rec.data.guid == curSelectedEncoderGuid)
							{
								encoderStore.remove(rec);
							}
						});
						encoderStore.commitChanges();
						curSelectedEncoderGuid = null;
						myAlert(null,"ɾ���ɹ�,����������ϵͳ!",250,0);
					}
					else
					{
						myAlert(null,"ɾ��ʧ�ܣ�",0,0);
					}																 
				});
			}
			else
			{
				
			}
		});
	}
	else
	{
		myAlert(null,"��ѡ����Ҫɾ���ı�������",230,0);
	}
}



function loadMatrixConf()
{
	showData = [];
	cameraStore = null;
	encoderStore = null;
	cameraGrid = null;
	encoderGrid = null;	
	ResourceLoader.loadTemplate("matrixconf");
	//��ʼ����ť����ʽ
	var arrBts = $('newCamera','deleteCamera','updateCamera','newEncoder','deleteEncoder','updateEncoder');
	arrBts.each(function(obj){
		obj.onmouseover = function(){
			this.className = "btIconOver";
		};
		obj.onmouseout = function(){
			this.className = "btIconNor";
		};
		obj.onmousedown = function(){
			this.className = "btIconDown";
		};
		obj.onmouseup = function(){
			this.className = "btIconOver";
		};
	});
	
	//���뵱ǰϵͳ���õľ���
	loadMatrixListPara();
}


function encoderInfo(val,object,pmEncoderStore)
{
	var showName = Ext.util.Format.ellipsis(pmEncoderStore.data.name,10,false);
	
	var info = val + "[" + showName + "](" + pmEncoderStore.data.portnum + "�˿�)";
	info = "<div style='float:left; width = 400px; margin-left:3px'>��������" + info + "</div>";
		
	if(pmEncoderStore.data.link == 0)
	{
		info += "<div style='float:left; width = 70px; margin-left:20px'>δ���Ӿ���</div>";
	}
	else
	{
		if(pmEncoderStore.data.serialstatus != 0)
		{
			info += "<div style='float:left; width = 70px; margin-left:20px; color:green'>���Ӿ���</div>";
		}
		else
		{
			info += "<div style='float:left; width = 70px; margin-left:20px;'>���Ӿ���</div>";
		}
		
	}
	
	if(pmEncoderStore.data.status == 0)
	{
		info += "<div style='float:left; width = 50px; margin-left:30px; color:red;'>����</div>";
	}
	else
	{
		info += "<div style='float:left; width = 50px; margin-left:30px; color:green;'>����</div>";
	}
	
	return info;
}


/*
	���������������Ķ˿�����
*/
function validateEdit(e)
{
	var data = e.record.data;
	var bRet = true;
	if(e.value == 0)
	{
		//ɾ������
		CMonMsg.setId(G200_ET.BS_G200MS_DELLINK_REQ);
		CMonMsg.setMsgNode("guid",data.guid);
		CMonMsg.setMsgNode("encoderport",data.encoderport - 1 );
		ResourceLoader.loadXmlData_Syn(CMonMsg.getMsg(),function(json){
			if(!json)
			{
				e.cancel = true;
				bRet = false;
				return false;
			}	
			if(json.kedacomxmldata.command == G200_ET.G200MS_BS_DELLINK_ACK)
			{
				myAlert(null,"ɾ�����ӳɹ�������������ϵͳ��",280,0);
				return true;
			}
			else
			{
				myAlert(null,"ɾ������ʧ�ܣ�",0,0);
				e.cancel = true;
				bRet = false;
				return false;
			}
		});
		return bRet;
	}
	else
	{
		//�������ӻ��޸�����
		if(!isCanUseOutport(matrixId,e.value))
		{
			e.cancel = true;
			return false;
		}
		
		CMonMsg.setId(G200_ET.BS_G200MS_ADDLINK_REQ);
		CMonMsg.setMsgNode("guid",data.guid);
		CMonMsg.setMsgNode("encoderport",data.encoderport - 1 );
		CMonMsg.setMsgNode("matrixoutport",e.value);
		ResourceLoader.loadXmlData_Syn(CMonMsg.getMsg(),function(json){
			if(!json)
			{
				e.cancel = true;
				bRet = false;
				return false;
			}	
			if(json.kedacomxmldata.command == G200_ET.G200MS_BS_ADDLINK_ACK)
			{
				myAlert(null,"���ӳɹ�������������ϵͳ��",280,0);
				return true;
			}
			else
			{
				myAlert(null,"����ʧ�ܣ�",0,0);
				e.cancel = true;
				bRet = false;
				return false;
			}
		});
		return bRet;
	}
}

/*
	���������˱���������Ϣ��ϳ���Ҫ����ʾ��ʽ
*/
function buildArrData(encoder)
{
	var totalNum = 0;
	/*if(encoder==undefined)
	{		
		alert('undefined');
		return;
	}*/
	if(encoder.length == undefined)
	{
		//ֻ��һ�����ݣ�����������
		for(var portIndex = 1; portIndex <= encoder.portnum; portIndex++)
		{
			showData.push({guid:encoder.guid,name:encoder.name,matrixid:encoder.matrixid,link:encoder.link,
			portnum:encoder.portnum,encoderport:portIndex,matrixport:0,status:0,serialstatus:0});
		}
		
		var storeConn = new Ext.data.Store({
			proxy:new Ext.data.MemoryProxy(encoder),
			reader:new Ext.data.JsonReader({id:'encoderport',root:'conn'},[{name:'encoderport',type:'int'},{name:'matrixport',type:'int'}])
		});
		storeConn.load();
		
		storeConn.each(function(rec){
			showData[totalNum + rec.data.encoderport].matrixport = rec.data.matrixport;		
		});
		storeConn.destroy();
	}
	else
	{		
		//��������
		for(var i = 0; i < encoder.length; i++)
		{
			//�˿���
			var portNum = parseInt(encoder[i].portnum);
			for(var j = 1; j <= portNum; j++)
			{
				showData.push({guid:encoder[i].guid,name:encoder[i].name,matrixid:encoder[i].matrixid,link:encoder[i].link,
					portnum:encoder[i].portnum,encoderport:j,matrixport:0,status:0,serialstatus:0});
			}
			
			storeConn = new Ext.data.Store({
				proxy:new Ext.data.MemoryProxy(encoder[i]),
				reader:new Ext.data.JsonReader({id:'encoderport',root:'conn'},[{name:'encoderport',type:'int'},{name:'matrixport',type:'int'}])
			});
			storeConn.load();
			storeConn.each(function(rec){
				showData[totalNum + rec.data.encoderport].matrixport = rec.data.matrixport;		
			});
			
			storeConn.destroy();
			totalNum += portNum;
		}
	}
}
function loadMatrixListPara()
{
	CMonMsg.setId(G200_ET.BS_G200MS_MATRIXSINFO_REQ);
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
		if(!json)
		{
			return false;
		}
		if(json.kedacomxmldata.command == G200_ET.G200MS_BS_MATRIXSINFO_NACK)
		{
			myAlert(null,"��������ʧ�ܣ�",0,0);
			return false;
		}
		storeMatrixs = new Ext.data.Store({
			proxy: new Ext.data.MemoryProxy(json.kedacomxmldata.content),
			reader:new Ext.data.JsonReader({id:'index',root:'matrix'},[
				{name:'index'},
				{name:'name'},
				{name:'inport',type:'int'},
				{name:'outport',type:'int'}
			])
		});
		storeMatrixs.load();
		
		combo = new Ext.form.ComboBox({
			store:storeMatrixs,
			mode:'local',
			triggerAction:'all',
			valueField:'index',
			displayField:'name',
			applyTo:'selList',
			editable:false
		});
		
		//��Ӽ����¼�
		combo.on("select",onChange);
		
		//ѡ���һ����й�����ʾ
		if(storeMatrixs.getCount() > 0)
		{
			var matrixItem = storeMatrixs.getAt(0);
			
			combo.setValue(matrixItem.data.index);
			onChange(combo);
			//���ö�ʱ����ȡ��������״̬
			timerEncoderStatus = window.setInterval(getEncoderStatus,10*1000);
		}
		else
		{
			getCameraList(0,0);
			getEncoderList(0,0);
			$('newEncoder').disabled = true;
			$('deleteEncoder').disabled = true;
			$('updateEncoder').disabled = true;
			$('encoderOperation').disabled = true;
			
			$('newCamera').disabled = true;
			$('deleteCamera').disabled = true;
			$('updateCamera').disabled = true;
			$('cameraOperation').disabled = true;
		}
	});	//end ResourceLoader
}


function onChange(comboBox)
{
	//�����豸��ʾ�����
	curSelectedEncoderGuid = null;
	
	//��ʾĳһ�����µ��豸
	matrixId = comboBox.getValue();
	cameraList = [];
	getCameraList(matrixId,0);	
	
	encoderList = [];
	showData = [];
	getEncoderList(matrixId,0);
	
	getEncoderStatus();
}

function showMatrixName(val)
{
	var matrixName = val;
	if(val == 0)
	{
		matrixName = "����";
	}
	storeMatrixs.each(function(rec){
		if(rec.data.index == val)
		{
			matrixName = rec.data.name;
			return false;
		}
	},this);
	return matrixName;
}
function showMatrixPort(val)
{
	var matrixPort = val;
	if(val == 0)
	{
		matrixPort = "��Ч";
	}
	return matrixPort;
}

function showCameraType(val)
{
	var typeName = val;
	storeCamType.each(function(rec){
		if(rec.data.id == val)
		{
			typeName = rec.data.type;
			return false;
		}
	},this);
	return typeName;
}

//var data = [];
function showEncoderSrcStatus(val)
{
	var EncoderSrcStatus = val;
	if(val == 0)
	{
		EncoderSrcStatus = "����";
	}
	return EncoderSrcStatus;
}

function isCanUseInport(matrixId,inport,camNo)
{
	//����˿ڲ��ܴ��ھ�����������˿ں�
	var recMatrix = storeMatrixs.getById(matrixId);
	if(recMatrix == undefined)
	{
		return false;
	}
	if(inport > recMatrix.data.inport)
	{
		myAlert(null,"���������������˿���("+ recMatrix.data.inport +")",250,0);
		return false;
	}
	
	var bRet = true;
	cameraStore.each(function(rec){
		if(rec.data.matrixid == matrixId && rec.data.videoport == inport && rec.data.guid != camNo)
		{
			bRet = false;
			myAlert(null,"�˿��ѱ�ռ�ã�",0,0);
			//�˳�ѭ��
			return false;
		}
	});
	
	return bRet;
}

function isCanUseOutport(matrixId,outport,encoderNo)
{
	//����˿ڲ��ܴ��ھ�����������˿ں�
	var recMatrix = storeMatrixs.getById(matrixId);
	if(recMatrix == undefined)
	{
		return false;
	}
	
	if(outport > recMatrix.data.outport)
	{
		myAlert(null,"�������������������("+ recMatrix.data.outport +")",250,0);
		return false;
	}
	
	var bRet = true;
	
/*	printf("matrixId = " + matrixId);
	printf("outport = " + outport);*/
	encoderStore.each(function(rec){
							   
		/*printf("rec.data.matrixid = " + rec.data.matrixid);
		printf("rec.data.matrixport = " + rec.data.matrixport);*/
		
		if(rec.data.matrixid == matrixId && rec.data.matrixport == outport)
		{
			bRet = false;
			myAlert(null,"�˿��ѱ�ռ�ã�",0,0);
			//�˳�ѭ��
			return false;
		}
	});
	
	return bRet;
}


function isCanUseCamAddr(matrixId,camAddr,camNo)
{
	//����˿ڲ��ܴ��ھ�����������˿ں�
	var recMatrix = storeMatrixs.getById(matrixId);
	if(recMatrix == undefined)
	{
		return false;
	}
	
	var bRet = true;
	cameraStore.each(function(rec){
		if(rec.data.matrixid == matrixId && rec.data.addr == camAddr && rec.data.guid != camNo)
		{
			bRet = false;
			myAlert(null,"��ַ���ѱ�ռ�ã�",0,0);
			//�˳�ѭ��
			return false;
		}
	});
	
	return bRet;
}


//��ȡ������������״̬
function getEncoderStatus()
{
	CMonMsg.setMsgNode("matrixid",matrixId);
	CMonMsg.setId(G200_ET.BS_G200MS_DEVICESTATUS_REQ);
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
			if(json.kedacomxmldata.command == G200_ET.G200MS_BS_DEVICESTATUS_ACK)
			{
					var encoderStatusList = json.kedacomxmldata.content.encoderlist.encoder;
					var encoderSendCmd = json.kedacomxmldata.content.serial;
					var encoderArrayList = [];
					if (encoderStatusList.length == undefined)
					{
						encoderArrayList.push(encoderStatusList);
					}
					else
					{
						encoderArrayList = $A(encoderStatusList);
					}
					
					//printf("**************ת������:" + encoderArrayList.length);
					encoderArrayList.each(function(encoder){						
							var recEncoders = encoderStore.query('guid',encoder.guid);
							recEncoders.each(function(rec){
								rec.data.status = encoder.status;
								rec.data.serialstatus = 0;
							});
					});
					
					//printf("**************������״̬��ֵ���");
					var recSerialEncoder =  encoderStore.query('guid',encoderSendCmd);
					recSerialEncoder.each(function(rec){
								rec.data.serialstatus = 1;
					});
					
					//printf("**************������ݸ���");
					encoderStore.commitChanges();
					encoderGrid.getView().refresh();
			}
	});
}

function getCameraList(matrixIndex,start)
{
	CMonMsg.setId(G200_ET.BS_G200MS_CAMERALIST_REQ);
	CMonMsg.setMsgNode("index",matrixIndex);
	CMonMsg.setMsgNode("start",start);
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
		if(json.kedacomxmldata.command == G200_ET.G200MS_BS_CAMERALIST_ACK)	
		{
		    if(!json.kedacomxmldata.content.cameralist)
			{
				printf("���ݽ���Ϊ��.");
			}
			else if(json.kedacomxmldata.content.cameralist.camera && !json.kedacomxmldata.content.cameralist.camera.length)
			{
				//��������
				cameraList.push(json.kedacomxmldata.content.cameralist.camera);
			}
			else
			{
				//�������ϵ�����
				cameraList = cameraList.concat(json.kedacomxmldata.content.cameralist.camera);
			}
			
			if(json.kedacomxmldata.content.over == 0)
			{
				//������ȡ����ͷ�б�
				getCameraList(matrixIndex,cameraList.length + 1);
			}
			else
			{
				//��ʾ����ͷ
				var content = {rows:cameraList};
				cameraStore = new Ext.data.Store({
					proxy: new Ext.data.MemoryProxy(content),
					reader: new Ext.data.JsonReader({id:'guid',root:'rows'},[
						{name:'guid'},
						{name:'name'},
						{name:'status'},
						{name:'matrixid'},
						{name:'videoport',type:'int'},
						{name:'type'},
						{name:'addr',type:'int'},
					]),
					sortInfo:{
						field:'videoport',
						direction:"ASC"
					}
				});
				cameraStore.load();
				
				$("cameralist").innerHTML = "";//ɾ����ǰ���б�
				cameraGrid = new Ext.grid.GridPanel({
					renderTo:'cameralist',
					store:cameraStore,
					width: 612,
					height: 200,
					stripeRows:true,
					columns:[
						 new Ext.grid.RowNumberer(),
						{header:'����',dataIndex:'name'},
					//	{header:'��������',dataIndex:'matrixid',renderer:showMatrixName},
						{header:'����˿�',dataIndex:'videoport',renderer:showMatrixPort,sortable:true},
						{header:'���������',dataIndex:'type',renderer:showCameraType},
						{header:'��ַ��',dataIndex:'addr'}
						],
					sm: new Ext.grid.RowSelectionModel({
						singleSelect:true
					})
				});	//end gridPanel
			}//end else
		}
	});//end load xml
}

function getEncoderList(matrixIndex,start)
{
	CMonMsg.setId(G200_ET.BS_G200MS_ENCODERLIST_REQ);
	CMonMsg.setMsgNode("start",start);
	CMonMsg.setMsgNode("index",matrixIndex);
	ResourceLoader.loadXmlData_Syn(CMonMsg.getMsg(),function(json){
		if(json.kedacomxmldata.command == G200_ET.G200MS_BS_ENCODERLIST_ACK)	
		{
			if(!json.kedacomxmldata.content.encoderlist)
			{
				//printf("�յ�������Ϊ��.");
			}
			else if(json.kedacomxmldata.content.encoderlist.encoder && !json.kedacomxmldata.content.encoderlist.encoder.length)
			{
				//��������
				encoderList.push(json.kedacomxmldata.content.encoderlist.encoder);
			}
			else//(json.kedacomxmldata.content.encoderlist.encoder.length)
			{
				//�������ϵ�����
				encoderList = encoderList.concat(json.kedacomxmldata.content.encoderlist.encoder);
				//printf("��ȡ���������ϵļ�¼ encoderlist.length = " + encoderList.length);
			}
			
			//printf("over��־ = " + json.kedacomxmldata.content.over);
			if(json.kedacomxmldata.content.over == 0)
			{
				//printf("ѭ����ȡ������start = " + encoderList.length);
				getEncoderList(matrixIndex,encoderList.length + 1);
			}
			else
			{
				//��ʾ������
				//printf("��ʾ�������б�");
				buildArrData(encoderList);

				var content ={rows:showData};
				
				encoderStore = new Ext.data.GroupingStore({
					proxy:new Ext.data.MemoryProxy(content),
					sortInfo:{field:'encoderport',direction:"ASC"},
					groupField:'guid',
					reader:new Ext.data.JsonReader({root:'rows'},[
					{name:'guid'},
					{name:'name'},
					{name:'matrixid'},
					{name:'link'},
					{name:'portnum',type:'int'},
					{name:'encoderport',type:'int'},
					{name:'matrixport',type:'int'},
					{name:'status'},
					{name:'serialstatus'}
					])
				});
				encoderStore.load();
				
				$("encoderlist").innerHTML = "";//ɾ����ǰ���б�
				encoderGrid = new Ext.grid.EditorGridPanel({
					renderTo:'encoderlist',
					width: 612,
					height: 300,
					store:encoderStore,
					stripeRows:true,
					columns:[
					{header:'������',dataIndex:'guid',hidden:true,renderer:encoderInfo},
					{header:'��ƵԴ�˿ں�',dataIndex:'encoderport',sortable:true,menuDisabled:true,type:'int'},
					{header:'��������˿ں�',dataIndex:'matrixport',renderer:showEncoderSrcStatus,editor:{xtype: 'numberfield',
													allowBlank: false,allowNegative:false,allowDecimals:false},
													sortable:true,menuDisabled:true,type:'int',align:'center'}
					],
					view:new Ext.grid.GroupingView({forceFit:true,
					groupTextTpl: '{group}',
					enableGroupingMenu:false}),
					sm: new Ext.grid.RowSelectionModel({
							singleSelect:true
					}),
					listeners:{
						groupclick :function(grid,field,value,e){
							//alert('groupclick:' + field + value);
							curSelectedEncoderGuid = value;
						},
						afteredit:function(e){
							e.record.commit();
						}
					}
				});
				encoderGrid.on('validateedit',validateEdit,this);
			}//end else
		}//ack cmd
	});//end ResourceLoader
}
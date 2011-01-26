var matrixType = new Ext.data.SimpleStore({
	fields:['id','type'],
	data:[[128,"AB"],[131,"��ƻ��"],[132,"AD"],[133,'Ӣ����'],[134,'�ɶ���'],[135,'����Τ��']]
});

function updateMatrix()
{
	var sm = grid.getSelectionModel();

	if(sm.hasSelection())
	{
		var sel = sm.getSelected();

		Ext.QuickTips.init();
		var winMatrix = new Ext.Window({
			layout:'form',
			autoHeight:true,
			width:310,
			title:'�޸ľ���',
			resizable:false,
			items:[
				{xtype:'textfield',fieldLabel:'����',id:'matrixName',allowBlank:false,width:150,maxLength:30,
					autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
				{xtype:'combo',fieldLabel:'����',id:'matrixType',mode:'local',store:matrixType,displayField:'type',
					width:120,allowBlank:false,valueField:'id',editable:false,triggerAction:'all'},
				{xtype:'numberfield',fieldLabel:'����˿���',id:'matrixOutport',width:100,allowBlank:false,disabled:true,
					allowNegative:false,allowDecimals:false},
				{xtype:'numberfield',fieldLabel:'����˿���',id:'matrixInport',width:100,allowBlank:false,disabled:true,
					allowNegative:false,allowDecimals:false}
			],
			modal:true,
			buttons:[
						{
							text:'ȷ��',
							handler:function(){
								if(!UiUtil.checkField('matrixName','��������'))
									return;
								if(!UiUtil.checkEmpty('matrixType','��������'))
									return;
									
								//����Ϣ�����������޸�
								CMonMsg.setId(G200_ET.BS_G200MS_UPDATE_MATRIX_REQ);
								CMonMsg.setMsgNode("index",sel.data.index);
								CMonMsg.setMsgNode("type", winMatrix.findById("matrixType").getValue());
								CMonMsg.setMsgNode("name", winMatrix.findById("matrixName").getValue());								
//								CMonMsg.setMsgNode("inport",winMatrix.findById("matrixInport").getValue());
//								CMonMsg.setMsgNode("outport",winMatrix.findById("matrixOutport").getValue());
								ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
									if(!json)
									{
										return;
									}
									if(json.kedacomxmldata.command != G200_ET.G200MS_BS_UPDATE_MATRIX_ACK)
									{
										myAlert(null,"�޸�ʧ�ܣ�",0,0);
										return;
									}
									//��������ӵ����ش洢��
									sel.set("name",winMatrix.findById("matrixName").getValue());
									sel.set("type",winMatrix.findById("matrixType").getValue());
//									sel.set("inport",winMatrix.findById("matrixInport").getValue());
//									sel.set("outport",winMatrix.findById("matrixOutport").getValue());
									sel.commit();
									winMatrix.close();
									grid.getView().refresh();
									myAlert(null,"�޸ĳɹ�,����������ϵͳ!",250,0);
								});
								
							}
						},
						{
							text:'ȡ��',
							handler:function(){
								winMatrix.close();
							}
						}
					]
		});
	
		winMatrix.findById("matrixName").setValue(sel.data.name);
		winMatrix.findById("matrixType").setValue(sel.data.type);
		winMatrix.findById("matrixInport").setValue(sel.data.inport);
		winMatrix.findById("matrixOutport").setValue(sel.data.outport);
		winMatrix.show();
	}
	else
	{
		myAlert(null,"��ѡ����Ҫ�޸ĵľ���",230,0);	
	}
}

function addMatrix()
{
	Ext.QuickTips.init();
	var winMatrix = new Ext.Window({
		layout:'form',
		autoHeight:true,
		width:310,
		title:'��Ӿ���',
		resizable:false,
		items:[
			{xtype:'textfield',fieldLabel:'����',id:'matrixName',allowBlank:false,width:150,maxLength:30,
				autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
			{xtype:'combo',fieldLabel:'����',id:'matrixType',mode:'local',store:matrixType,displayField:'type',valueField:'id',
				width:120,allowBlank:false,editable:false, triggerAction:'all'},
			{xtype:'numberfield',fieldLabel:'����˿���',id:'matrixOutport',width:100,allowBlank:false,allowNegative:false,
				allowDecimals:false},
			{xtype:'numberfield',fieldLabel:'����˿���',id:'matrixInport',width:100,allowBlank:false,allowNegative:false,
				allowDecimals:false}
		],
		modal:true,
		buttons:[
					{
						text:'ȷ��',
						handler:function(){
							if(!UiUtil.checkField('matrixName','��������'))
								return;
							if(!UiUtil.checkEmpty('matrixType','��������'))
								return;
							if(!UiUtil.checkEmpty('matrixOutport','��������˿���'))
								return;
							if(!UiUtil.checkPstvNumber('matrixOutport','��������˿���'))
								return;
							if(!UiUtil.checkEmpty('matrixInport','��������˿���'))
								return;
							if(!UiUtil.checkPstvNumber('matrixInport','��������˿���'))
								return;
							if(parseInt($('matrixOutport').value) > 2048)
							{
								myAlert(null,"��������˿�������!",250,0);
								return;
							}
							if(parseInt($('matrixInport').value) > 4096)
							{
								myAlert(null,"��������˿�������!",250,0);
								return;
							}
							
							//����Ϣ����������
							CMonMsg.setId(G200_ET.BS_G200MS_ADD_MATRIX_REQ);
							CMonMsg.setMsgNode("name", winMatrix.findById("matrixName").getValue());
							CMonMsg.setMsgNode("type", winMatrix.findById("matrixType").getValue());
							CMonMsg.setMsgNode("inport",winMatrix.findById("matrixInport").getValue());
							CMonMsg.setMsgNode("outport",winMatrix.findById("matrixOutport").getValue());
							ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
								if(!json)
								{
									return;
								}
								if(json.kedacomxmldata.command != G200_ET.G200MS_BS_ADD_MATRIX_ACK)
								{
									myAlert("���ʧ��",ERROR_CODE[json.kedacomxmldata.content.status],250,0);
									return;
								}
								//��������ӵ����ش洢��
								var newData = {index:json.kedacomxmldata.content.index,
									name:winMatrix.findById("matrixName").getValue(),
									type:winMatrix.findById("matrixType").getValue(),
									inport:winMatrix.findById("matrixInport").getValue(),
									outport:winMatrix.findById("matrixOutport").getValue()
								};
								var newRec = new store.recordType(newData);
								store.add(newRec);
								store.commitChanges();
								winMatrix.close();
								myAlert(null,"��ӳɹ�,����������ϵͳ!",250,0);
							});
						}
					},
					{
						text:'ȡ��',
						handler:function(){
							winMatrix.close();
						}
					}
				]
	});
	winMatrix.show();
}


function deleteMatrix()
{
	var sm = grid.getSelectionModel();
	if(sm.hasSelection())
	{
		Ext.Msg.confirm('��ʾ',"ɾ������",function(btn,text){
			if(btn == "yes"){
				var sel = sm.getSelected();
				CMonMsg.setId(G200_ET.BS_G200MS_DELETE_MATRIX_REQ);
				CMonMsg.setMsgNode("index",sel.data.index);
				ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
					if(!json)
					{
						return;
					}	
					if(json.kedacomxmldata.command != G200_ET.G200MS_BS_DELETE_MATRIX_ACK)
					{
						myAlert(null,"ɾ������ʧ�ܣ�",0,0);
						return;
					}
					store.remove(sel);
					store.commitChanges();
					grid.getView().refresh();
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
		myAlert(null,"��ѡ����Ҫɾ���ľ���",230,0);
	}
}

var grid;
var store;
function loadMatrixList()
{
	ResourceLoader.loadTemplate("matrixlist");
	
	//��ʼ����ť����ʽ
	var arrBts = $('new','delete','update','import','export');
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
	
	CMonMsg.setId(G200_ET.BS_G200MS_MATRIXSINFO_REQ);
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){											 
		if(!json)
		{
			return false;
		}
		if(json.kedacomxmldata.command != G200_ET.G200MS_BS_MATRIXSINFO_ACK)
		{
			myAlert(null,"��������ʧ�ܣ�",0,0);
			return false;
		}
		store = new Ext.data.Store({
			proxy: new Ext.data.MemoryProxy(json.kedacomxmldata.content),
			reader:new Ext.data.JsonReader({id:'index',root:'matrix'},[
				{name:'index'},
				{name:'name'},
				{name:'type'},
				{name:'inport',type:'int'},
				{name:'outport',type:'int'}
			]),
			sortInfo:{field:'inport',direction:'ASC'}
		});	
		store.load();
		
		grid = new Ext.grid.GridPanel({
			renderTo:'matrixlist',
			store:store,
			width: 612,
			height: 300,
			stripeRows:true,
			columns:[
				 new Ext.grid.RowNumberer(),
				{header:'����',dataIndex:'name',width:120},
				{header:'����',dataIndex:'type',renderer:type,width:160},
				{header:'����˿���',dataIndex:'inport',sortable:true,type:'int'},
				{header:'����˿���',dataIndex:'outport',type:'int'}
				],
			sm: new Ext.grid.RowSelectionModel({
				singleSelect:true
			})
		});	
	});
}

function type(val)
{
	var strType = "�Ƿ�����";
	matrixType.each(function(rec){
		if(val == rec.data.id){
			strType = rec.data.type;
			return false;
		};
	},this);
	return strType;
}

function exportFile(form)
{
	CMonMsg.setId(G200_ET.BS_G200MS_EXPORTCONFIG_REQ);
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
		if(!json){
			return;
		}
		if(json.kedacomxmldata.command != G200_ET.G200MS_BS_EXPORTCONFIG_ACK){
			myAlert(null,"��������ʧ�ܣ�",0,0);
			return;
		}
		$('hrefExport').click();
	});
}


function formsubmit(form)
{
	var fileName = $('file').value;
	if(fileName == "")
	{
		myAlert(null,"��ѡ���������ļ�",0,0);
		return;
	}
	if(fileName.lastIndexOf("conf.tar") == -1 && fileName.lastIndexOf("kedalicense.key") == -1)
	{
		myAlert(null,"��ѡ����ȷ���ļ�����",230,0);
		return;
	}
	ResourceLoader.uploadFile(form,function(json){
		if(!json){
			return;
		}
		myAlert(null,"�ļ��ϴ��ɹ�������������ϵͳ��",280,0);
	});
}

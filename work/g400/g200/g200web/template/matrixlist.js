var matrixType = new Ext.data.SimpleStore({
	fields:['id','type'],
	data:[[128,"AB"],[131,"红苹果"],[132,"AD"],[133,'英飞拓'],[134,'派尔高'],[135,'霍尼韦尔']]
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
			title:'修改矩阵',
			resizable:false,
			items:[
				{xtype:'textfield',fieldLabel:'名称',id:'matrixName',allowBlank:false,width:150,maxLength:30,
					autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
				{xtype:'combo',fieldLabel:'类型',id:'matrixType',mode:'local',store:matrixType,displayField:'type',
					width:120,allowBlank:false,valueField:'id',editable:false,triggerAction:'all'},
				{xtype:'numberfield',fieldLabel:'输出端口数',id:'matrixOutport',width:100,allowBlank:false,disabled:true,
					allowNegative:false,allowDecimals:false},
				{xtype:'numberfield',fieldLabel:'输入端口数',id:'matrixInport',width:100,allowBlank:false,disabled:true,
					allowNegative:false,allowDecimals:false}
			],
			modal:true,
			buttons:[
						{
							text:'确定',
							handler:function(){
								if(!UiUtil.checkField('matrixName','矩阵名称'))
									return;
								if(!UiUtil.checkEmpty('matrixType','矩阵类型'))
									return;
									
								//将信息发往服务器修改
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
										myAlert(null,"修改失败！",0,0);
										return;
									}
									//将数据添加到本地存储中
									sel.set("name",winMatrix.findById("matrixName").getValue());
									sel.set("type",winMatrix.findById("matrixType").getValue());
//									sel.set("inport",winMatrix.findById("matrixInport").getValue());
//									sel.set("outport",winMatrix.findById("matrixOutport").getValue());
									sel.commit();
									winMatrix.close();
									grid.getView().refresh();
									myAlert(null,"修改成功,请重新启动系统!",250,0);
								});
								
							}
						},
						{
							text:'取消',
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
		myAlert(null,"请选择需要修改的矩阵！",230,0);	
	}
}

function addMatrix()
{
	Ext.QuickTips.init();
	var winMatrix = new Ext.Window({
		layout:'form',
		autoHeight:true,
		width:310,
		title:'添加矩阵',
		resizable:false,
		items:[
			{xtype:'textfield',fieldLabel:'名称',id:'matrixName',allowBlank:false,width:150,maxLength:30,
				autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
			{xtype:'combo',fieldLabel:'类型',id:'matrixType',mode:'local',store:matrixType,displayField:'type',valueField:'id',
				width:120,allowBlank:false,editable:false, triggerAction:'all'},
			{xtype:'numberfield',fieldLabel:'输出端口数',id:'matrixOutport',width:100,allowBlank:false,allowNegative:false,
				allowDecimals:false},
			{xtype:'numberfield',fieldLabel:'输入端口数',id:'matrixInport',width:100,allowBlank:false,allowNegative:false,
				allowDecimals:false}
		],
		modal:true,
		buttons:[
					{
						text:'确定',
						handler:function(){
							if(!UiUtil.checkField('matrixName','矩阵名称'))
								return;
							if(!UiUtil.checkEmpty('matrixType','矩阵类型'))
								return;
							if(!UiUtil.checkEmpty('matrixOutport','矩阵输出端口数'))
								return;
							if(!UiUtil.checkPstvNumber('matrixOutport','矩阵输出端口数'))
								return;
							if(!UiUtil.checkEmpty('matrixInport','矩阵输入端口数'))
								return;
							if(!UiUtil.checkPstvNumber('matrixInport','矩阵输入端口数'))
								return;
							if(parseInt($('matrixOutport').value) > 2048)
							{
								myAlert(null,"矩阵输出端口数过大!",250,0);
								return;
							}
							if(parseInt($('matrixInport').value) > 4096)
							{
								myAlert(null,"矩阵输入端口数过大!",250,0);
								return;
							}
							
							//将信息发往服务器
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
									myAlert("添加失败",ERROR_CODE[json.kedacomxmldata.content.status],250,0);
									return;
								}
								//将数据添加到本地存储中
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
								myAlert(null,"添加成功,请重新启动系统!",250,0);
							});
						}
					},
					{
						text:'取消',
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
		Ext.Msg.confirm('提示',"删除矩阵！",function(btn,text){
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
						myAlert(null,"删除矩阵失败！",0,0);
						return;
					}
					store.remove(sel);
					store.commitChanges();
					grid.getView().refresh();
					myAlert(null,"删除成功,请重新启动系统!",250,0);
				});
			}
			else
			{
				
			}
		});	
	}
	else 
	{
		myAlert(null,"请选择需要删除的矩阵！",230,0);
	}
}

var grid;
var store;
function loadMatrixList()
{
	ResourceLoader.loadTemplate("matrixlist");
	
	//初始化按钮的样式
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
			myAlert(null,"加载数据失败！",0,0);
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
				{header:'名称',dataIndex:'name',width:120},
				{header:'类型',dataIndex:'type',renderer:type,width:160},
				{header:'输入端口数',dataIndex:'inport',sortable:true,type:'int'},
				{header:'输出端口数',dataIndex:'outport',type:'int'}
				],
			sm: new Ext.grid.RowSelectionModel({
				singleSelect:true
			})
		});	
	});
}

function type(val)
{
	var strType = "非法类型";
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
			myAlert(null,"导出配置失败！",0,0);
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
		myAlert(null,"请选择升级的文件",0,0);
		return;
	}
	if(fileName.lastIndexOf("conf.tar") == -1 && fileName.lastIndexOf("kedalicense.key") == -1)
	{
		myAlert(null,"请选择正确的文件导入",230,0);
		return;
	}
	ResourceLoader.uploadFile(form,function(json){
		if(!json){
			return;
		}
		myAlert(null,"文件上传成功，请重新启动系统！",280,0);
	});
}

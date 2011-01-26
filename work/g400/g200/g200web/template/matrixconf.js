

//存储所有矩阵信息
var storeMatrixs;
var storeCamType = new Ext.data.SimpleStore({
	fields:['id','type'],
	data:[[1,"SONY"],[2,"AD"],[3,"PELCO"],[4,"PIH"],[5,"PELCO_P"],[6,"PARCO"],[7,"AB"],[8,"YUNTAI"],[9,"三星"],
		  [10,"雅安"],[11,"AD168"],[12,"卡特拉"],[13,"JN"],[14,"松下650"],[15,"松下850"],[16,"艾立克"],[17,"PANASONIC-OLD"],[18,"PANASONIC-NEW"]]
});

var cameraStore;
var encoderStore;
var cameraGrid;
var encoderGrid;
//编码器表格的显示数据
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
		title:'添加摄像机',
		resizable:false,
		modal:true,
		items:[
			   	{xtype:'textfield',fieldLabel:'名称',id:'cameraName',allowBlank:false,width:150,maxLength:30,
					autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
			   	{xtype:'combo',fieldLabel:'类型',id:'cameraType',allowBlank:false,width:120,store:storeCamType,
					valueField:'id',displayField:'type',mode:'local',editable:false,triggerAction:'all'},
				//{xtype:'combo',fieldLabel:'连接的矩阵',id:'cameraMatrix',allowBlank:false,width:120,store:storeMatrixs,mode:'local',
//					valueField:'index',displayField:'name',editable:false,triggerAction:'all'},
				{xtype:'numberfield',fieldLabel:'矩阵端口',id:'cameraPort',allowBlank:false,width:80,allowNegative:false,
					allowDecimals:false},
				{xtype:'numberfield',fieldLabel:'地址码',id:'cameraAddr',allowBlank:false,width:80,allowNegative:false,
					allowDecimals:false}
			  ],
		buttons:[
					{	text:'确定',
						handler:function(){
							
							if(!UiUtil.checkField('cameraName','摄像机名称'))
								return;
							if(!UiUtil.checkEmpty('cameraType','摄像机类型'))
								return;
							if(!UiUtil.checkEmpty('cameraPort','矩阵端口'))
								return;
							if(!UiUtil.checkPstvNumber('cameraPort','矩阵端口'))
								return;
							if(!UiUtil.checkEmpty('cameraAddr','摄像机地址码'))
								return;
							if(!UiUtil.checkNumber('cameraAddr','摄像机地址码'))
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
									myAlert("添加失败",ERROR_CODE[json.kedacomxmldata.content.status],250,0);
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
								myAlert(null,"添加成功,请重新启动系统!",250,0);
							});
						}
					},
					{	text:'取消',
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
		Ext.Msg.confirm('提示',"删除此摄像机！",function(btn,text){
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
						myAlert(null,"删除摄像机失败！",0,0);
						return;
					}
					cameraStore.remove(sel);
					cameraStore.commitChanges();
					cameraGrid.getView().refresh();
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
		myAlert(null,"请选择需要删除的摄像机！",230,0);	
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
			title:'修改摄像机',
			resizable:false,
			modal:true,
			items:[
				{xtype:'textfield',fieldLabel:'名称',id:'cameraName',allowBlank:false,width:150,maxLength:30,
					autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
			   	{xtype:'combo',fieldLabel:'类型',id:'cameraType',allowBlank:false,width:120,store:storeCamType,
					valueField:'id',displayField:'type',mode:'local',editable:false,triggerAction:'all'},
				/*{xtype:'combo',fieldLabel:'连接的矩阵',id:'cameraMatrix',allowBlank:false,width:120,store:storeMatrixs,mode:'local',
					valueField:'index',displayField:'name',editable:false,triggerAction:'all'},*/
				{xtype:'numberfield',fieldLabel:'矩阵端口',id:'cameraPort',allowBlank:false,width:80,allowNegative:false,
					allowDecimals:false},
				{xtype:'numberfield',fieldLabel:'地址码',id:'cameraAddr',allowBlank:false,width:80,allowNegative:false,
					allowDecimals:false}
			],
			buttons:[
						{
							text:'确定',
							handler:function(){
								
								if(!UiUtil.checkField('cameraName','摄像机名称'))
									return;
								if(!UiUtil.checkEmpty('cameraType','摄像机类型'))
									return;
								if(!UiUtil.checkEmpty('cameraPort','矩阵端口'))
									return;
								if(!UiUtil.checkPstvNumber('cameraPort','矩阵端口'))
									return;
								if(!UiUtil.checkEmpty('cameraAddr','摄像机地址码'))
									return;
								if(!UiUtil.checkNumber('cameraAddr','摄像机地址码'))
									return;
								if(!isCanUseInport(matrixId,winCamera.findById('cameraPort').getValue(),sel.data.guid))
								{
									return;
								}
								
								if(!isCanUseCamAddr(matrixId,winCamera.findById('cameraAddr').getValue(),sel.data.guid))
								{
									return;	
								}
								//将信息发往服务器修改
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
										myAlert(null,"修改失败！",0,0);
										return;
									}
									
									//将数据添加到本地存储中
									sel.set("name",winCamera.findById("cameraName").getValue());
									sel.set("type",winCamera.findById("cameraType").getValue());
									sel.set("matrixid",matrixId);
									sel.set("videoport",winCamera.findById("cameraPort").getValue());
									sel.set("addr",winCamera.findById("cameraAddr").getValue());
									sel.commit();
									
									//cameraStore.filter('matrixid',matrixId,false,false);
									winCamera.close();
									myAlert(null,"修改成功,请重新启动系统!",250,0);
								});
								
							}
						},
						{
							text:'取消',
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
		myAlert(null,"请选择需要修改的摄像机！",230,0);	
	}
}


function addEncoder()
{
	Ext.QuickTips.init();
	var winEncoder = new Ext.Window({
		layout:'form',
		autoHeight:true,
		width:310,
		title:'添加编码器',
		resizable:false,
		modal:true,
		items:[
			  	{xtype:'textfield',fieldLabel:'名称',width:150,id:'encoderName',allowBlank:false,maxLength:30,
					autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
			  	{xtype:'numberfield',fieldLabel:'端口数',width:80,id:'encoderPorts',allowBlank:false,
					allowNegative:false,allowDecimals:false,maxValue:64},
				//{xtype:'combo',fieldLabel:'连接的矩阵',width:120,id:'encoderMatrix',allowBlank:false,editable:false,triggerAction:'all',
//					store:storeMatrixs,mode:'local',valueField:'index',displayField:'name'},
				{xtype:'checkbox',fieldLabel:'',id:'encoderConn',boxLabel:'串口与矩阵连接',labelSeparator:''}
			  ],
		buttons:[
					{	text:'确定',
						handler:function(){		
						
							if(!UiUtil.checkField('encoderName','编码器名称'))
								return;
							if(!UiUtil.checkEmpty('encoderPorts','视频源端口数'))
								return;
							if(!UiUtil.checkPstvNumber('encoderPorts','视频源端口数'))
								return;
							
							if(parseInt($('encoderPorts').value) > 64)
							{
								myAlert(null,"超过最大视频源端口数64!",250,0);
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
									myAlert("添加失败",ERROR_CODE[json.kedacomxmldata.content.status],250,0);
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
								myAlert(null,"添加成功,请重新启动系统!",250,0);
							});
						}
					},
					{	text:'取消',
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
			title:'修改编码器',
			resizable:false,
			items:[
				{xtype:'textfield',fieldLabel:'编号',id:'encoderGuid',readOnly:true,width:230},
				{xtype:'textfield',fieldLabel:'名称',width:150,id:'encoderName',allowBlank:false}, 
			  	{xtype:'numberfield',fieldLabel:'端口数',width:80,id:'encoderPorts',allowBlank:false,disabled:true,allowNegative:false},
				/*{xtype:'combo',fieldLabel:'连接的矩阵',width:120,id:'encoderMatrix',allowBlank:false,editable:false,triggerAction:'all',
					store:storeMatrixs,mode:'local',valueField:'index',displayField:'name'},*/
				{xtype:'checkbox',fieldLabel:'',id:'encoderConn',boxLabel:'串口与矩阵连接',labelSeparator:''}
			],
			modal:true,
			buttons:[
						{
							text:'确定',
							handler:function(){								
								if(!UiUtil.checkField('encoderName','编码器名称'))
									return;	
									
								//将信息发往服务器修改
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
										myAlert(null,"修改失败！",0,0);
										return;
									}
									recs.each(function(item){
										item.data.name = winEncoder.findById("encoderName").getValue();
										item.data.link = UiUtil.parseBool(winEncoder.findById('encoderConn').getValue());  
									},this);
									encoderStore.commitChanges();
									encoderStore.filter('matrixid',matrixId,false,false);
									winEncoder.close();
									myAlert(null,"修改成功,请重新启动系统!",250,0);
								});
								
							}
						},
						{
							text:'取消',
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
		myAlert(null,"请选择需要修改的编码器！",230,0);	
	}
}


function deleteEncoder()
{
	if(curSelectedEncoderGuid)
	{
		Ext.Msg.confirm('提示',"删除编码器",function(btn,text){
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
						//删除一组数据
						encoderStore.each(function(rec){
							if(rec.data.guid == curSelectedEncoderGuid)
							{
								encoderStore.remove(rec);
							}
						});
						encoderStore.commitChanges();
						curSelectedEncoderGuid = null;
						myAlert(null,"删除成功,请重新启动系统!",250,0);
					}
					else
					{
						myAlert(null,"删除失败！",0,0);
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
		myAlert(null,"请选择需要删除的编码器！",230,0);
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
	//初始化按钮的样式
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
	
	//载入当前系统配置的矩阵
	loadMatrixListPara();
}


function encoderInfo(val,object,pmEncoderStore)
{
	var showName = Ext.util.Format.ellipsis(pmEncoderStore.data.name,10,false);
	
	var info = val + "[" + showName + "](" + pmEncoderStore.data.portnum + "端口)";
	info = "<div style='float:left; width = 400px; margin-left:3px'>编码器：" + info + "</div>";
		
	if(pmEncoderStore.data.link == 0)
	{
		info += "<div style='float:left; width = 70px; margin-left:20px'>未连接矩阵</div>";
	}
	else
	{
		if(pmEncoderStore.data.serialstatus != 0)
		{
			info += "<div style='float:left; width = 70px; margin-left:20px; color:green'>连接矩阵</div>";
		}
		else
		{
			info += "<div style='float:left; width = 70px; margin-left:20px;'>连接矩阵</div>";
		}
		
	}
	
	if(pmEncoderStore.data.status == 0)
	{
		info += "<div style='float:left; width = 50px; margin-left:30px; color:red;'>下线</div>";
	}
	else
	{
		info += "<div style='float:left; width = 50px; margin-left:30px; color:green;'>在线</div>";
	}
	
	return info;
}


/*
	建立编码器与矩阵的端口链接
*/
function validateEdit(e)
{
	var data = e.record.data;
	var bRet = true;
	if(e.value == 0)
	{
		//删除连接
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
				myAlert(null,"删除连接成功，请重新启动系统！",280,0);
				return true;
			}
			else
			{
				myAlert(null,"删除连接失败！",0,0);
				e.cancel = true;
				bRet = false;
				return false;
			}
		});
		return bRet;
	}
	else
	{
		//建立连接或修改连接
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
				myAlert(null,"连接成功，请重新启动系统！",280,0);
				return true;
			}
			else
			{
				myAlert(null,"连接失败！",0,0);
				e.cancel = true;
				bRet = false;
				return false;
			}
		});
		return bRet;
	}
}

/*
	将服务器端编码器的信息组合成需要的显示格式
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
		//只有一行数据，不构成数组
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
		//数组数据
		for(var i = 0; i < encoder.length; i++)
		{
			//端口数
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
			myAlert(null,"加载数据失败！",0,0);
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
		
		//添加监听事件
		combo.on("select",onChange);
		
		//选择第一项进行过滤显示
		if(storeMatrixs.getCount() > 0)
		{
			var matrixItem = storeMatrixs.getAt(0);
			
			combo.setValue(matrixItem.data.index);
			onChange(combo);
			//设置定时器获取编码器的状态
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
	//空闲设备显示在最后
	curSelectedEncoderGuid = null;
	
	//显示某一矩阵下的设备
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
		matrixName = "空闲";
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
		matrixPort = "无效";
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
		EncoderSrcStatus = "空闲";
	}
	return EncoderSrcStatus;
}

function isCanUseInport(matrixId,inport,camNo)
{
	//输入端口不能大于矩阵的最大输入端口号
	var recMatrix = storeMatrixs.getById(matrixId);
	if(recMatrix == undefined)
	{
		return false;
	}
	if(inport > recMatrix.data.inport)
	{
		myAlert(null,"超过矩阵最大输入端口数("+ recMatrix.data.inport +")",250,0);
		return false;
	}
	
	var bRet = true;
	cameraStore.each(function(rec){
		if(rec.data.matrixid == matrixId && rec.data.videoport == inport && rec.data.guid != camNo)
		{
			bRet = false;
			myAlert(null,"端口已被占用！",0,0);
			//退出循环
			return false;
		}
	});
	
	return bRet;
}

function isCanUseOutport(matrixId,outport,encoderNo)
{
	//输出端口不能大于矩阵的最大输出端口号
	var recMatrix = storeMatrixs.getById(matrixId);
	if(recMatrix == undefined)
	{
		return false;
	}
	
	if(outport > recMatrix.data.outport)
	{
		myAlert(null,"超过矩阵最大端输出口数("+ recMatrix.data.outport +")",250,0);
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
			myAlert(null,"端口已被占用！",0,0);
			//退出循环
			return false;
		}
	});
	
	return bRet;
}


function isCanUseCamAddr(matrixId,camAddr,camNo)
{
	//输入端口不能大于矩阵的最大输入端口号
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
			myAlert(null,"地址码已被占用！",0,0);
			//退出循环
			return false;
		}
	});
	
	return bRet;
}


//获取编码器的在线状态
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
					
					//printf("**************转换数据:" + encoderArrayList.length);
					encoderArrayList.each(function(encoder){						
							var recEncoders = encoderStore.query('guid',encoder.guid);
							recEncoders.each(function(rec){
								rec.data.status = encoder.status;
								rec.data.serialstatus = 0;
							});
					});
					
					//printf("**************编码器状态赋值完成");
					var recSerialEncoder =  encoderStore.query('guid',encoderSendCmd);
					recSerialEncoder.each(function(rec){
								rec.data.serialstatus = 1;
					});
					
					//printf("**************完成数据更新");
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
				printf("数据接收为空.");
			}
			else if(json.kedacomxmldata.content.cameralist.camera && !json.kedacomxmldata.content.cameralist.camera.length)
			{
				//单个数据
				cameraList.push(json.kedacomxmldata.content.cameralist.camera);
			}
			else
			{
				//两个以上的数据
				cameraList = cameraList.concat(json.kedacomxmldata.content.cameralist.camera);
			}
			
			if(json.kedacomxmldata.content.over == 0)
			{
				//继续获取摄像头列表
				getCameraList(matrixIndex,cameraList.length + 1);
			}
			else
			{
				//显示摄像头
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
				
				$("cameralist").innerHTML = "";//删除以前的列表
				cameraGrid = new Ext.grid.GridPanel({
					renderTo:'cameralist',
					store:cameraStore,
					width: 612,
					height: 200,
					stripeRows:true,
					columns:[
						 new Ext.grid.RowNumberer(),
						{header:'名称',dataIndex:'name'},
					//	{header:'矩阵名称',dataIndex:'matrixid',renderer:showMatrixName},
						{header:'矩阵端口',dataIndex:'videoport',renderer:showMatrixPort,sortable:true},
						{header:'摄像机类型',dataIndex:'type',renderer:showCameraType},
						{header:'地址码',dataIndex:'addr'}
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
				//printf("收到的数据为空.");
			}
			else if(json.kedacomxmldata.content.encoderlist.encoder && !json.kedacomxmldata.content.encoderlist.encoder.length)
			{
				//单个数据
				encoderList.push(json.kedacomxmldata.content.encoderlist.encoder);
			}
			else//(json.kedacomxmldata.content.encoderlist.encoder.length)
			{
				//两个以上的数据
				encoderList = encoderList.concat(json.kedacomxmldata.content.encoderlist.encoder);
				//printf("获取了两个以上的记录 encoderlist.length = " + encoderList.length);
			}
			
			//printf("over标志 = " + json.kedacomxmldata.content.over);
			if(json.kedacomxmldata.content.over == 0)
			{
				//printf("循环获取编码器start = " + encoderList.length);
				getEncoderList(matrixIndex,encoderList.length + 1);
			}
			else
			{
				//显示编码器
				//printf("显示编码器列表");
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
				
				$("encoderlist").innerHTML = "";//删除以前的列表
				encoderGrid = new Ext.grid.EditorGridPanel({
					renderTo:'encoderlist',
					width: 612,
					height: 300,
					store:encoderStore,
					stripeRows:true,
					columns:[
					{header:'编码器',dataIndex:'guid',hidden:true,renderer:encoderInfo},
					{header:'视频源端口号',dataIndex:'encoderport',sortable:true,menuDisabled:true,type:'int'},
					{header:'矩阵输出端口号',dataIndex:'matrixport',renderer:showEncoderSrcStatus,editor:{xtype: 'numberfield',
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
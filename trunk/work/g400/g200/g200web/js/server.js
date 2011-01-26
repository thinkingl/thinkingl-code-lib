// JavaScript Document
//UI�߼����������
//����:�����˵�,��ѡ��,Tabѡ��,����/��ʾ,IP�ֽ����
var UiUtil = {
	// ��������Ԫ�صĳ�ʼֵ

	setSelectedIndexValue : function (selectId,value){
		var someNodeList = $(selectId).getElementsByTagName('option');
		var nodes = $A(someNodeList);
		nodes.each(function(node){
			if(node.value == value){
				node.selected = "selected";
				return false;
			}
		});
	},

	setSelectedIndexText : function(selectId,text){
			var someNodeList = $(selectId).getElementsByTagName('option');
			var nodes = $A(someNodeList);
			nodes.each(function(node){
				 if(node.text == text){
					node.selected = "selected";	 
					return false;
			     }
			})
		},
		
	getSelectedIndexValue : function (selectId){
		var someNodeList = $(selectId).getElementsByTagName('option');
		var nodes = $A(someNodeList);
		var value = -1;
		nodes.each(function(node){
			if(node.selected){
				value = node.value;
				return false;
			}
		});
		return value;
	},

	getSelectedIndexText : function(selectId){
		var someNodeList = $(selectId).getElementsByTagName('option');
		var nodes = $A(someNodeList);
		var text = "";
		nodes.each(function(node){
			if(node.selected){
				text = node.text;
				return false;
			}
		});
		return text;
	},

	delSelectedIndex :function (selectId){
		var op =  $(selectId).options;

		for(var i=0;i<op.length;i++)
		{
			if(op[i].selected)
			op.remove(i);
		}
	},

	addSelectedIndex :function (selectId,value,text){
		var op =  $(selectId).options;
		var creatOption = new Option();
		creatOption.value = value;
		creatOption.text = text;
		op[op.length] = creatOption;
	},

	
	//������ѡ��ͨ���˵�����ʽ
	setClassName : function(selectedNum,channelNum){
		
		for (var i=0;i < channelNum ;i++ ){
			if(i == selectedNum){
				$("tab"+i).className = "current";
		
			}else{
				$("tab"+i).className = "";
				
			}
		}
	},	
		
	//���ø�ѡ�� 
	setCheckBox : function(checkBoxId,value){
		if(value == 1){
			$(checkBoxId).checked = true;
		}else{
			$(checkBoxId).checked = false;
		}
	},	

	getCheckBox : function(checkBoxId){
		if($(checkBoxId).checked){
			return 1;
		}else{
			return 0;
		}
	},

	setDisabled : function(triggerId){
		if($(triggerId).checked){
			for(var i=1;i<arguments.length;i++){
				$(arguments[i]).disabled = false;
			}
		}else{
			for(var i=1;i<arguments.length;i++){
				$(arguments[i]).disabled = true;
			}
		}
	},

	setEnabled : function(triggerId){
		if(!$(triggerId).checked){
			for(var i=1;i<arguments.length;i++){
				$(arguments[i]).disabled = false;
			}
		}else{
			for(var i=1;i<arguments.length;i++){
				$(arguments[i]).disabled = true;
			}
		}
	},

	splitIP: function (element,address){
		var arr = address.split(".");
		for(var i=0;i<4;i++){
			$(element+i).value = arr[i];
		}
	},

	connectIP: function (element){
		var IP = "";
		for(var i=0;i<4;i++){
			IP += $F(element+i);
			if(i<3)
			IP +=".";
		}
		return IP;
	},
	
	parseBool:function(element){
		if(element)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	},
	
	checkEmpty: function (name,msg)
	{
		var obj = $(name);
		if(obj.value == "")
		{
			myAlert(null,"������"+msg,250,0);
			obj.focus();
			return false;
		}
		return true;
	},
	
	checkNumber:function(name,msg)
	{
		var obj = $(name);
		var bRet = Validation.isInteger(obj.value);
		if(!bRet)
		{
			myAlert(null, msg + ":����Ƿ�!",250,0);
			obj.focus();
			return false;
		}
		return true;
	},
	
	checkPstvNumber:function(name,msg)
	{
		var obj = $(name);
		var bRet = Validation.isPositiveInteger(obj.value);
		if(!bRet)
		{
			myAlert(null, msg + ":����Ƿ�!",250,0);
			obj.focus();
			return false;
		}
		return true;
	},
	
	checkIp:function(name,msg)
	{
		var obj = $(name);
		var bRet = Validation.isIp(obj.value);
		if(!bRet)
		{
			myAlert(null, msg + ":�Ƿ�!",250,0);
			obj.focus();
			return false;
		}
		return true;
	},
	
	checkField:function(name,msg)
	{
		var obj = $(name);
		var bRet = Validation.isFiledString(obj.value);
		if(!bRet)
		{
			myAlert(null, msg + ":�Ƿ�!",250,0);
			obj.focus();
			return false;
		}
		return true;
	}
};

var globalHandlers= {
	onCreate:function()
	{
		//Element.show("sysworking");
	},
	onComplete:function()
	{
		if(Ajax.activeRequestCount == 0)
		{
			//Element.hide("sysworking");
		}
	}
};




//ע��ȫ���¼�
Ajax.Responders.register(globalHandlers);
var SystemEncode = 0;
var currentTemplete = "";
var requestTimeout = 30000;

/*
	����ext��ajax��ʽ���������շ�
*/
//������Դ,��̬�������Ŀ¼�µ�.tpl,.xml,.js��Դ,���:��Դ����
//����ҳ��ģ��:ResourceLoader.loadTemplate
//����XML����:ResourceLoader.loadXmlData
//����js����:ResourceLoader.loadScript
var ResourceLoader = {

	_resourceUrl:"./template/", 
	_cgiUrl:"/kedacomxmldata",
	_cgiFile:"/kedacomfile",
	
	loadTemplate : function(templateName){
		var url =  this._resourceUrl+templateName+".tpl";
		//��¼��ǰ��ҳ��
		printf("$ - start loading template="+url);
		new Ajax.Updater(
			{success: 'container'}, 
			url, 
			{ 
        		asynchronous  :false,
				method: 'get', 
				onFailure : AjaxErrorHandler
			});
	},
	loadHomePage : function(templateName){
		var url =  this._resourceUrl+templateName+".tpl";
		//��¼��ǰ��ҳ��
		printf("$ - start loading template="+url);
		new Ajax.Updater(
			{success: 'homepage'}, 
			url, 
			{ 
        		asynchronous  :false,
				method: 'get', 
				onFailure : AjaxErrorHandler
			});	
	},
	loadXmlData : function(xmlMessage,handler){
		printf("\n"+"sendMsg------------>"+xmlMessage+"\n");
	
		var url =  this._cgiUrl;

		printf("$ - start loading xml data="+url);
		Ext.Ajax.request({
			url:url,
			method:'POST',
			success:function(response,opts){
				handler(WebConsole_Interceptor(response));
			},
			failure:function(response,opts){
				AjaxErrorHandler(response);
			},
			timeout:requestTimeout,
			xmlData:xmlMessage
		});
	},
	
	loadXmlData_Syn:function(xmlMessage,handler){
		printf("\n"+"sendMsg------------>"+xmlMessage+"\n");
	
		var url =  this._cgiUrl;

		printf("$ - start loading xml data="+url);
		new Ajax.Request(
			url, 
			{  
				asynchronous :false,
				method: 'post', 
				postBody:xmlMessage,
				onComplete: function (request){
					handler(WebConsole_Interceptor(request));
				},
				onFailure : AjaxErrorHandler
			});
	},
	
	sendXmlData : function(xmlMessage){
		printf("\n"+"sendMsg------------>"+xmlMessage+"\n");
	
		var url =  this._cgiUrl;

		printf("$ - start loading xml data="+url);
		Ext.Ajax.request({
			url:url,
			method:'POST',
			xmlData:xmlMessage
		});
	},
	
	loadScript:function(scriptName){
			var url = this._resourceUrl+scriptName+".js";
			printf("$ - start loading js="+url);
			var script=document.createElement("script");
			//script.defer=true;
			script.src=url;
			
			document.body.appendChild(script);
			printf("@ - start JS");
	},
	uploadFile:function(formName,handler){
		var url = this._cgiFile;
		printf("\n@start to upload file...\n");
		Ext.Ajax.request({
			url:url,
			method:'POST',
			success:function(response,opts){
				handler(WebConsole_Interceptor(response));
			},
			failure:function(response,opts){
				AjaxErrorHandler(response);
			},
			form:formName,
			isUpload:true
		});
	}
};


function AjaxErrorHandler(t){
	if(t.status=='12029'|| t.status=='12007')
	{
		myAlert(null,"��ȷ�����豸�����Ƿ�������",240,0);
	}
	else if(t.status == -1)
	{
		myAlert(null,"�ȴ���ʱ��",0,0);
	}
}

//��xml����ʽת����json�ĸ�ʽ����
function WebConsole_Interceptor(t){
	printf("\n"+"Ӧ����Ϣ"+(t.responseText)+"\n");	
	var json = false;
	try{
		json = JXmlParser.parseElement(t.responseXML);
	}catch(e){
		printf(e.description);
		json = false;
	}
	return json;
}




function logout()
{
	location.href="./index.html";	
}

var CMonMsg =
{
	id : "",
	msgBody : [],
	
	setId:function(strId)
	{
		this.id = strId;
	},
	setMsgNode:function(name,value)
	{
		var strNode = "<" + name + ">" + value + "</" + name + ">";
		this.msgBody.push(strNode);
	},
	
	getHeader:function()
	{
		return "<?xml version = \"1.0\" encoding = \"gb2312\"?>"
	},
	
	/*getTail:function()
	{
		return "</xml>";	
	},*/
	
	getMsg:function()
	{
		var body = "";
		for(var index = 0;index < this.msgBody.length; index++)
		{
			body += this.msgBody[index];
		}
		var msg = this.getHeader()
		           + "<kedacomxmldata>"
				   + "<command>" + this.id + "</command>"
				   + "<content>"
				   + body
				   + "</content>"
				   + "</kedacomxmldata>";
				   this.id = "";
				   this.msgBody.length = 0;
		return msg;
	}
	
};
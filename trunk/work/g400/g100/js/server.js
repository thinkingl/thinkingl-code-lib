//��װ��Ϣ
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

//��װajax
var ResourceLoader = {

	_resourceUrl:"./template/", 
	_cgiUrl:"/kedacomxmldata",
	_cgiFile:"/kedacomfile",
	
	loadTemplate : function(templateName,handler){
		var url =  this._resourceUrl+templateName+".tpl";
		//��¼��ǰ��ҳ��
		currentTemplete = templateName;
		printf("$ - start loading template="+url);
		ajGet(url,function(html,ret){
               if(ret){ 
                    handler(html);
               }else{
                  	myAlert(null,"���糬ʱ,��������!",200,0);
               }
        });	
	},
	loadXmlData : function(xmlMessage,handler,bSynParam){	
		var url =  this._cgiUrl;
		printf("$send request = " + xmlMessage);
        new ajPost(url , xmlMessage , function(obj,ret){         
               if(ret){ 
                handler(WebConsole_Interceptor(obj));
               }else{ 
               }
        },bSynParam);
	},

	loadScript:function(scriptName){
		//[FIXME] write code to avoid the script is loaded more than once!
			if(currentTemplete == scriptName){ 
				return false;
			}
			var url = this._resourceUrl+scriptName+".js";
			printf("$ - start loading js="+url);
			var script=document.createElement("script");
			//script.defer=true;
			script.src=url;
			document.body.appendChild(script);
			printf("@ - satrt JS");
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
		var msg="��ȷ�����豸�����Ƿ�����!";
		myAlert(null,msg,200,0);
		return;
	}
	myAlert(null,"δ�õ��豸��Ӧ",200,0);
}

//��xml����ʽת����json�ĸ�ʽ����
function WebConsole_Interceptor(t){
        printf(t.responseText);
		json = JXmlParser.parseElement(t.responseXML);
        return json;
}

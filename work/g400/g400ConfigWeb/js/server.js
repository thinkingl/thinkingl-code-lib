
// 消息流水号
var g_msgSn = 0;

// session.
var g_G400Session = "";

// 消息打印过滤. 因为一直会发送 MSG_GET_NOTIFY ,所以默认把它去掉,否则打印太多了.
var g_MessageLogFilt = new Array();

// 默认过滤这个消息..
g_MessageLogFilt[G400_MESSAGE.MSG_GET_NOTIFY] = 1;

//封装消息
function CMonMsg() 
{
    this.id="";
    this.msgBody = new Object();



    // 每次实例化时流水号自增.
    g_msgSn++;

    this.setId = function (strId) {
        this.id = strId;
    };

    this.getId = function(){
        return this.id;
    }
    
    this.setMsgNode = function (name, value) {
        //       var strNode = "<" + name + ">" + value + "</" + name + ">";
        //       this.msgBody.push(strNode);
        this.msgBody[name] = value;
    };

    // 获取xml头.
    this.getHeader = function () {
        return "<?xml version = \"1.0\" encoding = \"utf-8\"?>";
    };

    

    this.getMsg = function () {
        var bodyXml = "";
        for (var name in this.msgBody) {
            var xmlText = "<" + name + ">" + this.msgBody[name] + "</" + name + ">";
            bodyXml += xmlText;
        }
        var msg = this.getHeader()
		           + "<KedacomXMLData>"
				   + "<Msg>" + this.id + "</Msg>"
                   + "<Sn>" + g_msgSn + "</Sn>"
                   + "<Session>" + g_G400Session + "</Session>"
				   + "<Content>"
				   + bodyXml
				   + "</Content>"
				   + "</KedacomXMLData>";
        
        return msg;
    };
};

CMonMsg.setSession = function ( newSession ) {
        g_G400Session = newSession;
    };

function unitTestCMonMsg() {
    var utMsg = new CMonMsg();
    utMsg.setId("msgIdTest");
    utMsg.setMsgNode("name", "value12");
    utMsg.setMsgNode("name", "newValue");
    utMsg.setMsgNode("age", 10);
    var msg = utMsg.getMsg();
    alert(msg);

    ResourceLoader.loadXmlData(msg, function (json) {
        alert( json.KedacomXMLData.Head.Session );
    });
    
}

//封装ajax
var ResourceLoader = {

    _resourceUrl: "./template/",
    _cgiUrl: "/kedacomxmldata",
    _cgiFile: "/kedacomfile",
    m_fastCgiUrl: "/g400-xml/g400.fcgi",   //login_ack.xml",

    loadTemplate: function (templateName, handler) {
        var noCacheParam = new Date().getTime();
        var url = this._resourceUrl + templateName + ".tpl?time="+noCacheParam;
        //记录当前的页面
        currentTemplete = templateName;
        printf("$ - start loading template=" + url);

        ajGet(url, function (html, ret) {
            if (ret) {
                handler(html);
            } else {
                myAlert(null, "网络超时,请检查网络!", 200, 0);
            }
        });
    },

    loadXmlData: function (monMsg, handler, bSynParam) {
        var xmlMessage = monMsg.getMsg();
        var msgId = monMsg.getId();

        // url 附加参数只为调试更方便..
        var g400url = this.m_fastCgiUrl + "?msg=" + msgId;
        if (!g_MessageLogFilt[msgId]) {
            JLog.writeXML("$send request = " + xmlMessage);
        }
        //       alert("sendreq: "+ xmlMessage);

        ajPost(g400url, xmlMessage, function (xhr, ret) {
            if (ret) {
                handler(WebConsole_Interceptor(xhr));
                //handler(xhr);
                $('#result').html(xhr.responseText);
                //               alert(xhr.responseText);
                //                  alert( xhr.responseXML.text );
                //                  alert(ret);
            } else {
            }
        }, bSynParam);
    },

    loadScript: function (scriptName) {
        //[FIXME] write code to avoid the script is loaded more than once!
        if (currentTemplete == scriptName) {
            return false;
        }
        var url = this._resourceUrl + scriptName + ".js";
        printf("$ - start loading js=" + url);
        var script = document.createElement("script");
        //script.defer=true;
        script.src = url;
        document.body.appendChild(script);
        printf("@ - satrt JS");
    },

    uploadFile: function (formName, handler) {
        var url = this._cgiFile;
        printf("\n@start to upload file...\n");
        Ext.Ajax.request({
            url: url,
            method: 'POST',
            success: function (response, opts) {
                handler(WebConsole_Interceptor(response));
            },
            failure: function (response, opts) {
                AjaxErrorHandler(response);
            },
            form: formName,
            isUpload: true
        });
    }
};

function AjaxErrorHandler(t) {
    if (t.status == '12029' || t.status == '12007') {
        var msg = "请确认与设备连接是否正常!";
        myAlert(null, msg, 200, 0);
        return;
    }
    myAlert(null, "未得到设备回应", 200, 0);
}

//将xml的形式转换成json的格式对象
function WebConsole_Interceptor( xhr ) {
//    printf(xhr.responseText);
    

//    alert( "rcvmsg: " + xhr.responseText);
    json = JXmlParser.parseElement(xhr.responseXML);

    // 打印.
    var msgId = json.KedacomXMLData.Msg;
    if (!g_MessageLogFilt[msgId]) {
        JLog.writeXML("rcvmsg: " + xhr.responseText);
    }
    return json;
}

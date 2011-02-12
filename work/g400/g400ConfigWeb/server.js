//封装消息
var CMonMsg =
{
    id: "",
    msgBody: [],

    setId: function (strId) {
        this.id = strId;
    },
    setMsgNode: function (name, value) {
        var strNode = "<" + name + ">" + value + "</" + name + ">";
        this.msgBody.push(strNode);
    },

    getHeader: function () {
        return "<?xml version = \"1.0\" encoding = \"gb2312\"?>"
    },

    /*getTail:function()
    {
    return "</xml>";	
    },*/

    getMsg: function () {
        var body = "";
        for (var index = 0; index < this.msgBody.length; index++) {
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
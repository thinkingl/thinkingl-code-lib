

// 页面加载成功后调用的函数.
$(document).ready(function () {
    $(".titleBar").hide();
    $(".rightFrame").hide();

    $("#changePass").click(function (event) { changePassword(event); }); //修改密码
    $("#unlogin").click(function (event) { unlogin(event); }); //注销
    $('#btLog').attr('class', 'btLoginNor');
    $('#txt_username').keypress(function (event) {
        if (window.event.keyCode == 13) {
            $('#btLog').click();
        }
    });
    $('#txt_password').keypress(function (event) {
        if (window.event.keyCode == 13) {
            $('#btLog').click();
        }
    });

    // 尝试用cookie中的登录session登录.
    // 如果这个session还没有失效,那么就不必再次登录了.
    // 这样用户刷新页面时不必重新登录.
    // 还未实现.

    //登陆按键按下时响应
    $('#btLog').click(function () {
        if (!saveCookie())
            return false;
        var loginMsg = new CMonMsg();
        loginMsg.setId(G400_MESSAGE.MSG_LOGIN);
        loginMsg.setMsgNode("User", $('#txt_username').val());
        loginMsg.setMsgNode("Password", $('#txt_password').val());
        ResourceLoader.loadXmlData(loginMsg.getMsg(), function (json)//通过ajax发送登陆消息到服务器
        {
            //            alert("login " + json.KedacomXMLData.Head.Session);
            if (isErrorCommand(json) === false)
                return;
            var loginStatus = json.KedacomXMLData.Head.Status;
            if (loginStatus == 1) {//登陆成功
                loginSuccessed(json);
                return;
            }
            else {
                var loginErr = json.KedacomXMLData.Content.ErrorMesssage;
                myAlert("提示", loginErr, 180, 0);
            }


        });
    });
    button_hover_function();
    loadCookie();
});   ////////onready方法over

/************************************************
登陆成功后调用这个方法，更新左面的leftGuaid,获得g100的基本数据
*************************************************/
function loginSuccessed(json) {
    ResourceLoader.loadTemplate("leftGuaid", function (html) {
        $("#leftGuaid").html(html);


        LEFT_GUAID();               //leftList.js中的方法，点击左测导航栏的响应事件

        CMonMsg.setId(G100_COMMAND.COMMAND_GETG100);
        CMonMsg.setMsgNode("range", "1");           //只得到G100的类型
        CMonMsg.setMsgNode("host", location.host);
        ResourceLoader.loadXmlData(CMonMsg.getMsg(), function (json) {
            $("#loginWin").hide();
            $(".titleBar").show();
            $(".rightFrame").show();
            OnProcessG100Main(json);
        }, false);
    });
}; //loginSuccessed() over

/*************************login javascript***************************************/
function getCookie(name) {
    var cookies = document.cookie.split(name + "=");
    if (cookies.length > 1) {
        var values = cookies[1].split(";");
        return unescape(values[0]);
    }
    return null;
}

//在设置Cookie之前必须删除以前的Cookie
function delCookie(name) {
    var exp = new Date();
    exp.setTime(exp.getTime() - 1);
    var cval = getCookie(name);
    if (cval != null) document.cookie = name + "=" + cval + ";expires=" + exp.toGMTString();
}

//设置cookie，有效期一年
function setCookie(name, value) {
    delCookie(name);
    var now = new Date();
    now.setFullYear(now.getFullYear() + 1);
    var expires = "; expires=" + now.toGMTString();
    document.cookie = name + "=" + escape(value) + expires + '; path=/;';
}
//读入保存在cookie中的用户名和密码
function loadCookie() {
    var user = getCookie("user");
    var remember = getCookie("remember");
    if (user) {
        $('#txt_username').val(user);
        $('#txt_password').focus();
    }
    else {
        $('#txt_username').focus();
    }

    if (remember == "yes") {
        var psw = getCookie("psw");

        if (psw == "") {
            return false;
        }
        $('#txt_password').val(psw);
        $('#chkRem').attr("checked", true);
    }
}



function saveCookie() {

    //判断是否要记住密码
    var checkFlag = $('#chkRem').attr("checked");
    if (checkFlag) {
        setCookie("remember", "yes");
        setCookie("psw", $('#txt_password').val());
    }
    else {
        setCookie("remember", "no");
    }
    setCookie("user", $('#txt_username').val());
    return true;
}


////////////////////////////////////////////////////////////////////////over


//判断返回的命令是否为错误命令
function isErrorCommand(json) {
    var msgId = json.KedacomXMLData.Head.Msg;
    if ( msgId == "" ) {
        myAlert(null, json.KedacomXMLData.Head.Status, 330, 0);
        //alert(json.kedacomxmldata.content.status);
        return false;
    }
    return true;
}

//登陆成功后，根据G100的类型来确定页面所在的位置
function OnProcessG100Main(json) {
    //根据类型设置左边导航栏的状态
    if (json.kedacomxmldata.content.type == "A") {
        $("#main_mtu").attr('class', 'leftGuaidList_disable');
        $("#main_dvr").attr('class', 'leftGuaidList_clicked');
        goToDvrlist();
    } else if (json.kedacomxmldata.content.type == "T") {
        $("#main_dvr").attr('class', 'leftGuaidList_disable');
        $("#main_mtu").attr('class', 'leftGuaidList_clicked');
        goToMTUDetails();
    } else {
        $("#main_dvr").attr('class', 'leftGuaidList_clicked');
        goToDvrlist();
    }

    $('body').everyTime(3000, "heartSkip", heartSkip); //设置一个心跳计时器，定期向服务器发送心跳  
};


//修改密码
function changePassword(event) {
    event.preventDefault();
    Ext.QuickTips.init();
    var win = new Ext.Window({
        frame: true,
        title: '修改密码',
        width: 350,
        modal: true,
        layout: 'form',
        items: [{
            xtype: 'textfield',
            fieldLabel: '旧密码',
            id: 'oldPass',
            allowBlank: false,
            inputType: 'password',
            maxLength: '24',
            autoCreate: { tag: 'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '24' }

        }, {
            xtype: 'textfield',
            fieldLabel: '新密码',
            id: 'newPass',
            allowBlank: false,
            inputType: 'password',
            maxLength: '24',
            autoCreate: { tag: 'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '24' }
        }, {
            xtype: 'textfield',
            fieldLabel: '再输入一次',
            id: 'surePass',
            allowBlank: false,
            inputType: 'password',
            maxLength: '24',
            autoCreate: { tag: 'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '24' }
        }],
        buttons: [{
            text: '确定',
            handler: function () {
                if (win.findById('newPass').getValue() != win.findById('surePass').getValue()) {
                    myAlert("提示", "两次输入的新密码不正确,请重新输入新密码", 350, 0);
                    return;
                }

                if (!UiUtil.checkField('oldPass', '旧密码'))
                    return;
                if (!UiUtil.checkField('newPass', '新密码'))
                    return;
                if (!UiUtil.checkField('surePass', '确认密码'))
                    return;

                CMonMsg.setId(G100_COMMAND.COMMAND_CHANGEPASSWORD);
                CMonMsg.setMsgNode("oldpassword", win.findById('oldPass').getValue());
                CMonMsg.setMsgNode("newpassword", win.findById('newPass').getValue());
                ResourceLoader.loadXmlData(CMonMsg.getMsg(), function (json) {
                    if (isErrorCommand(json) === false)
                        return;
                    var value = json.kedacomxmldata.content.status;
                    if (value == 1) {
                        win.close();
                        myAlert("提示", "密码修改成功", 200, 0);
                    } else if (value == 2) {
                        myAlert("提示", "密码修改失败", 200, 0);
                    } else if (value == 3) {
                        myAlert("提示", "旧密码不正确", 200, 0);
                    }
                }); //loadXmlData over
            } //function over
        }, {
            text: '取消',
            handler: function () {
                win.close();
            }
        }
        ]
    });
    win.show();
};


//注销
function unlogin(event) {
    CMonMsg.setId(G100_COMMAND.COMMAND_UNLOGIN);
    CMonMsg.setMsgNode("unlogin", "unlogin");
    ResourceLoader.loadXmlData(CMonMsg.getMsg(), function (json) {
        if (isErrorCommand(json) === false)
            return;
        var value = json.kedacomxmldata.content.status;
        if (value == 1) {
            location.reload();
        } else if (value == 2) {
            myAlert("提示", "注销失败,请关闭浏览器", 220, 0);
        }
    }); //loadXmlData over    
    event.preventDefault();

};

//确定用户在线的心跳
function heartSkip() {
    CMonMsg.setId(G100_COMMAND.COMMAND_HERTSKIP);
    CMonMsg.setMsgNode("skip ", "skip ");
    ResourceLoader.loadXmlData(CMonMsg.getMsg(), function (json) { }, false);  //以异步的方式发送心跳
};

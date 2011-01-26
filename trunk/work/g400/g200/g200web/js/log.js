function getCookie(name)
{
	var cookies = document.cookie.split(name+"=");
	if(cookies.length>1)
	{
		var values=cookies[1].split(";");
		return unescape(values[0]);
	}
	return null;
}

//在设置Cookie之前必须删除以前的Cookie
function delCookie(name)
{
	 var exp = new Date();
	 exp.setTime(exp.getTime() - 1);
	 var cval = getCookie(name);
	 if(cval != null) document.cookie= name + "="+cval+";expires="+exp.toGMTString();
}

//设置cookie，有效期一年
function setCookie(name,value)
{
	delCookie(name);
	var now=new Date();
	now.setFullYear(now.getFullYear()+1);
	var expires="; expires=" + now.toGMTString();
	document.cookie=name+"=" + escape(value) + expires + '; path=/;';
}
//读入保存在cookie中的用户名和密码
function loadCookie()
{
	var user = getCookie("user");
	var remember = getCookie("remember");
	if(user)
	{
		$('user').value = user;
		$('psw').focus();
	}
	else
	{
		$('user').focus();	
	}
	
	if(remember == "yes")
	{
		var psw = getCookie("psw");
		if (psw == "")
		{
			return false;	
		}
		$('psw').value = psw;		
		$('chkRem').checked = true;
	}
}

function showMessage(msg)
{
	//document.all.prompt.innerText = msg;
}

function saveCookie()
{
	if(!UiUtil.checkEmpty("user","用户名"))
	{
		return false;	
	}
	if(!UiUtil.checkEmpty("psw","密码"))
	{
		return false;
	}
	
	//判断是否要记住密码
	var checkFlag = $('chkRem').checked;
	if(checkFlag)
	{
		setCookie("remember","yes");
		setCookie("psw",$('psw').value);
	}
	else
	{
		setCookie("remember","no");	
	}
	setCookie("user",$('user').value);
	return true;
}

//检测到用户按下enter键时，进行登录操作
function onEnter(e)
{
	if(window.event.keyCode == 13)
	{
		loginOn();
	}
}

function loginOn()
{
	if(!saveCookie())
	return false;
	
	CMonMsg.setId(G200_ET.BS_G200MS_USER_LOGIN_REQ);
	CMonMsg.setMsgNode("username",$('user').value);
	CMonMsg.setMsgNode("password",$('psw').value);
	
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
	{
		if(!json)
		{
			return false;	
		}
		if(json.kedacomxmldata.command != G200_ET.G200MS_BS_USER_LOGIN_ACK)
		{
			myAlert(null,ERROR_CODE[json.kedacomxmldata.content.status],200,0);
			return;
		}
		
		Element.hide('loginWin');
		$('curUserName').innerHTML = $('user').value;
		Element.show('bannerLog');
		//载入主页
		//ResourceLoader.loadScript("mainpage");
		loadMainPage();
	});
}

function sysQuit()
{
	CMonMsg.setId(G200_ET.BS_G200MS_SYSQUIT_REQ);
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
		location.reload();
	});
}

function updatePsw()
{
	Ext.QuickTips.init();
	var winUser = new Ext.Window({
		layout:'form',
		autoHeight:true,
		width:310,
		title:'修改帐户',
		resizable:false,
		items:[
			{xtype:'textfield',fieldLabel:'新用户名',id:'userName',allowBlank:false,width:150,maxLength:30,
				autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
			{xtype:'textfield',fieldLabel:'原始密码',id:'oldPsw',allowBlank:false,width:150,inputType:'password',maxLength:30,
				autoCreate:{tag:'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '30'}},
			{xtype:'textfield',fieldLabel:'新密码',id:'newPsw',allowBlank:false,width:150,inputType:'password',maxLength:30,
				autoCreate:{tag:'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '30'}},
			{xtype:'textfield',fieldLabel:'请再输一次',id:'newPswAgain',allowBlank:false,width:150,inputType:'password',maxLength:30,
				autoCreate:{tag:'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '30'}}
		],
		modal:true,
		buttons:[
					{
						text:'确定',
						handler:function(){
							if(!UiUtil.checkEmpty('userName','新用户名'))
								return;
							if(!UiUtil.checkEmpty('oldPsw','原始密码'))
								return;
							if(!UiUtil.checkEmpty('newPsw','新密码'))
								return;
							if(!UiUtil.checkEmpty('newPswAgain','新密码再一次'))
								return;
							
							if($('newPsw').value != $('newPswAgain').value)
							{
								myAlert(null,"两次输入的新密码不一样!",250,0);
								return;
							}
							
							//将信息发往服务器
							CMonMsg.setId(G200_ET.BS_G200MS_UPDATE_USER_REQ);
							CMonMsg.setMsgNode("oldpsw", winUser.findById("oldPsw").getValue());
							CMonMsg.setMsgNode("username", winUser.findById("userName").getValue());						
							CMonMsg.setMsgNode("newpsw",winUser.findById("newPsw").getValue());
							ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
								if(!json)
								{
									return;
								}
								if(json.kedacomxmldata.command == G200_ET.G200_BS_UPDATE_USER_ACK)
								{		
									$('curUserName').innerHTML = winUser.findById("userName").getValue();
									winUser.close();
									myAlert(null,"修改成功",0,0);
								}
								else
								{
									myAlert(null,"修改失败",0,0);
								}
																				 
							});
						}
					},
					{
						text:'取消',
						handler:function(){
							winUser.close();
						}
					}
				]
	});
	winUser.findById("userName").setValue($('curUserName').innerHTML);
	winUser.show();
}
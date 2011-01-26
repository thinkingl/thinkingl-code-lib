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

//������Cookie֮ǰ����ɾ����ǰ��Cookie
function delCookie(name)
{
	 var exp = new Date();
	 exp.setTime(exp.getTime() - 1);
	 var cval = getCookie(name);
	 if(cval != null) document.cookie= name + "="+cval+";expires="+exp.toGMTString();
}

//����cookie����Ч��һ��
function setCookie(name,value)
{
	delCookie(name);
	var now=new Date();
	now.setFullYear(now.getFullYear()+1);
	var expires="; expires=" + now.toGMTString();
	document.cookie=name+"=" + escape(value) + expires + '; path=/;';
}
//���뱣����cookie�е��û���������
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
	if(!UiUtil.checkEmpty("user","�û���"))
	{
		return false;	
	}
	if(!UiUtil.checkEmpty("psw","����"))
	{
		return false;
	}
	
	//�ж��Ƿ�Ҫ��ס����
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

//��⵽�û�����enter��ʱ�����е�¼����
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
		//������ҳ
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
		title:'�޸��ʻ�',
		resizable:false,
		items:[
			{xtype:'textfield',fieldLabel:'���û���',id:'userName',allowBlank:false,width:150,maxLength:30,
				autoCreate:{tag:'input', type: 'text', size: '20', autocomplete: 'off', maxlength: '30'}},
			{xtype:'textfield',fieldLabel:'ԭʼ����',id:'oldPsw',allowBlank:false,width:150,inputType:'password',maxLength:30,
				autoCreate:{tag:'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '30'}},
			{xtype:'textfield',fieldLabel:'������',id:'newPsw',allowBlank:false,width:150,inputType:'password',maxLength:30,
				autoCreate:{tag:'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '30'}},
			{xtype:'textfield',fieldLabel:'������һ��',id:'newPswAgain',allowBlank:false,width:150,inputType:'password',maxLength:30,
				autoCreate:{tag:'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '30'}}
		],
		modal:true,
		buttons:[
					{
						text:'ȷ��',
						handler:function(){
							if(!UiUtil.checkEmpty('userName','���û���'))
								return;
							if(!UiUtil.checkEmpty('oldPsw','ԭʼ����'))
								return;
							if(!UiUtil.checkEmpty('newPsw','������'))
								return;
							if(!UiUtil.checkEmpty('newPswAgain','��������һ��'))
								return;
							
							if($('newPsw').value != $('newPswAgain').value)
							{
								myAlert(null,"��������������벻һ��!",250,0);
								return;
							}
							
							//����Ϣ����������
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
									myAlert(null,"�޸ĳɹ�",0,0);
								}
								else
								{
									myAlert(null,"�޸�ʧ��",0,0);
								}
																				 
							});
						}
					},
					{
						text:'ȡ��',
						handler:function(){
							winUser.close();
						}
					}
				]
	});
	winUser.findById("userName").setValue($('curUserName').innerHTML);
	winUser.show();
}
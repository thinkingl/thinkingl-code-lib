var szURI="/kedacomxmldata";
var szStart="<?xml version='1.0' encoding='GB2312'?><kedacomxmldata>";
var szEnd="</kedacomxmldata>";
var cmuType = "vs";

/************************************************
        ����ҳ���سɹ��󣬵����������
*************************************************/
$(document).ready(function() {
    $(".titleBar").hide();
    $(".rightFrame").hide();

    $("#changePass").click(function(event){changePassword(event);}); //�޸�����
    $("#unlogin").click(function(event){unlogin(event);});//ע��
    $('#btLog').attr('class','btLoginNor'); 
    $('#txt_username').keypress(function(event){
		if(window.event.keyCode == 13)
		{
			$('#btLog').click();
		}
	});
	$('#txt_password').keypress(function(event){
		if(window.event.keyCode == 13)
		{
			$('#btLog').click();
		}
	});
    //��½��������ʱ��Ӧ
    $('#btLog').click(function(){
    	if(!saveCookie())
        	return false;    
    	CMonMsg.setId(G100_COMMAND.COMMAND_LOGION);
    	CMonMsg.setMsgNode("username",$('#txt_username').val());
    	CMonMsg.setMsgNode("password",$('#txt_password').val());
    	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)//ͨ��ajax���͵�½��Ϣ��������
    	{
    	    if(isErrorCommand(json)===false)
    	       return;
    		var value = json.kedacomxmldata.content.status;
            if(value == 1){//��½�ɹ�
                loginSuccessed(json);    
                return;
            }else if(value == 2){
                     messageToShow= "�û���������";   
            }else if(value == 3){
                     messageToShow = "�������";                           
            }else if(value == 4){
                     messageToShow = "���������û���¼";                           
            }else {
                    messageToShow = "ĩ����Ĵ���";
            }
            myAlert("��ʾ", messageToShow ,180 ,0);
    	});
    });
    button_hover_function();   
    loadCookie();
});////////onready����over

/************************************************
        ��½�ɹ������������������������leftGuaid,���g100�Ļ�������
*************************************************/
function loginSuccessed(json){
     	ResourceLoader.loadTemplate("leftGuaid",function(html){
            $("#leftGuaid").html(html);
            
			
			LEFT_GUAID();               //leftList.js�еķ����������⵼��������Ӧ�¼�
			
        	CMonMsg.setId(G100_COMMAND.COMMAND_GETG100);
        	CMonMsg.setMsgNode("range","1");           //ֻ�õ�G100������
            CMonMsg.setMsgNode("host" , location.host);         
        	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
                $("#loginWin").hide();    
				$(".titleBar").show();
                $(".rightFrame").show();   
                OnProcessG100Main(json);
            },false);
        });                    
};//loginSuccessed() over

/*************************login javascript***************************************/
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
		$('#txt_username').val(user);
		$('#txt_password').focus();
	}
	else
	{
		$('#txt_username').focus();	
	}
	
	if(remember == "yes")
	{
		var psw = getCookie("psw");

		if (psw == "")
		{
			return false;	
		}
		$('#txt_password').val(psw);		
		$('#chkRem').attr("checked" , true);
	}
}



function saveCookie()
{
	
	//�ж��Ƿ�Ҫ��ס����
	var checkFlag = $('#chkRem').attr("checked");
	if(checkFlag)
	{
		setCookie("remember","yes");
		setCookie("psw",$('#txt_password').val());
	}
	else
	{
		setCookie("remember","no");	
	}
	setCookie("user",$('#txt_username').val());
	return true;
}


////////////////////////////////////////////////////////////////////////over
 

//�жϷ��ص������Ƿ�Ϊ��������
function isErrorCommand(json){
    var command = json.kedacomxmldata.command;
    if(command == 0){
		myAlert(null,json.kedacomxmldata.content.status,330,0);
        //alert(json.kedacomxmldata.content.status);
        return false;
    }
    return true;
}

//��½�ɹ��󣬸���G100��������ȷ��ҳ�����ڵ�λ��
function OnProcessG100Main(json )
{
    //��������������ߵ�������״̬
    if(json.kedacomxmldata.content.type == "A"){
         $("#main_mtu").attr('class','leftGuaidList_disable');
         $("#main_dvr").attr('class' , 'leftGuaidList_clicked');   
         goToDvrlist();          
    }else if(json.kedacomxmldata.content.type == "T"){
        $("#main_dvr").attr('class','leftGuaidList_disable');
        $("#main_mtu").attr('class' , 'leftGuaidList_clicked'); 
        goToMTUDetails();         
    }else{
    ����$("#main_dvr").attr('class' , 'leftGuaidList_clicked');   
    ����goToDvrlist();        
    }
    
   $('body').everyTime(3000 , "heartSkip" , heartSkip);//����һ��������ʱ�����������������������  
};


//�޸�����
function changePassword(event){
   event.preventDefault();
   Ext.QuickTips.init();
   var win = new Ext.Window({
        frame: true,
        title:'�޸�����',            
        width:350,
        modal : true,
        layout: 'form',
        items:[{
            xtype:'textfield',
            fieldLabel:'������',
            id:'oldPass',
            allowBlank: false,
            inputType:'password',
			maxLength: '24',
			autoCreate:{tag:'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '24'}
            
        },{
            xtype:'textfield',
            fieldLabel:'������',
            id:'newPass',
            allowBlank: false,
            inputType:'password',
			maxLength: '24',
			autoCreate:{tag:'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '24'}
        },{
            xtype:'textfield',
            fieldLabel:'������һ��',
            id:'surePass',
            allowBlank: false,
            inputType:'password',
			maxLength: '24',
			autoCreate:{tag:'input', type: 'password', size: '20', autocomplete: 'off', maxlength: '24'}
        }],
        buttons:[{
            text:'ȷ��',
            handler:function(){
                        if(win.findById('newPass').getValue()!=win.findById('surePass').getValue()){
                            myAlert("��ʾ" , "��������������벻��ȷ,����������������",350 ,0 );
                            return;
                        }

						if(!UiUtil.checkField('oldPass','������'))
							return;
						if(!UiUtil.checkField('newPass','������'))
							return;
						if(!UiUtil.checkField('surePass','ȷ������'))
							return;
							
                        CMonMsg.setId(G100_COMMAND.COMMAND_CHANGEPASSWORD);
                        CMonMsg.setMsgNode("oldpassword",win.findById('oldPass').getValue());   
                        CMonMsg.setMsgNode("newpassword",win.findById('newPass').getValue());
                       	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
                    	    if(isErrorCommand(json)===false)
                    	       return;
                 	        var value = json.kedacomxmldata.content.status;
                 	        if (value == 1){
                 	           win.close();
                             ��myAlert("��ʾ" , "�����޸ĳɹ�",200 ,0 );
                            }else if(value == 2){
                            ����myAlert("��ʾ" , "�����޸�ʧ��",200 ,0 );
                            }else if(value == 3){
                            ����myAlert("��ʾ" , "�����벻��ȷ",200 ,0 );
                            }          
                       });//loadXmlData over
                }//function over
        },{
            text:'ȡ��',
            handler:function(){
             win.close();
            }
        }
        ]        
    });
    win.show();  
};

 
//ע��
function unlogin(event){
    CMonMsg.setId(G100_COMMAND.COMMAND_UNLOGIN);
    CMonMsg.setMsgNode("unlogin","unlogin");   
   	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
	    if(isErrorCommand(json)===false)
	       return;
        var value = json.kedacomxmldata.content.status; 
        if (value == 1){ 
		   location.reload();
        }else if(value == 2){
           myAlert("��ʾ" , "ע��ʧ��,��ر������",220 ,0 );
        }         
   });//loadXmlData over    
   event.preventDefault();

};

//ȷ���û����ߵ�����
function heartSkip(){
    CMonMsg.setId(G100_COMMAND.COMMAND_HERTSKIP);
    CMonMsg.setMsgNode("skip ","skip ");   
    ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){},false);  //���첽�ķ�ʽ��������
};

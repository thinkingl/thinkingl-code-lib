// JavaScript Document

var mplattype = 1;
var mplatip = "";
var mplatport = 1722;
var ip3a = "";
var port3a = 30000;
var enterNetNo = "";
var user3a = "";
var psw3a = "";

var selIp = "";
var subMask = "";
var defaultGate = "";

function loadSysConfig()
{
	ResourceLoader.loadTemplate("sysconfig");
	
	//初始化按钮的样式
	var arrBts = $('btAppSysConf','btUpdateSysConf','btAppLocalIp','btUpdateIp');
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
	
	getPlatStatus();
	
	CMonMsg.setId(G200_ET.BS_G200_SYSCONF_REQ);
	
	//获取指定网卡上的ip
	CMonMsg.setMsgNode("hostip",location.host);
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){		
		if(json.kedacomxmldata.command == G200_ET.G200_BS_SYSCONF_ACK)
		{
			var conf = json.kedacomxmldata.content;
			
			$("mplattype").innerHTML = MPLAT_TYPE[conf.mplattype];
			mplattype = conf.mplattype;
			
			$('mplatip').innerHTML = conf.mplatip;
			mplatip = conf.mplatip;
			
			$('mplatport').innerHTML = conf.mplatport;
			mplatport = conf.mplatport;
			
			$("ip3a").innerHTML = conf.ip3a;
			ip3a = conf.ip3a;
			
			$("port3a").innerHTML = conf.port3a; 
			port3a = conf.port3a
			
			$('No').innerHTML = conf.No;
			enterNetNo = conf.No;
			
			$('3auser').innerHTML = conf.user3a;
			user3a = conf.user3a;
			
			$('3apsw').innerHTML = conf.psw3a;
			psw3a = conf.psw3a;
			
			$('selIp').innerHTML = conf.selIp;
			selIp = conf.selIp;
			
			$('subMask').innerHTML = conf.subMask;
			subMask = conf.subMask;
			
			$('defaultGate').innerHTML = conf.defaultGate;
			defaultGate = conf.defaultGate;
		}
		else if(json.kedacomxmldata.command == G200_ET.G200_BS_SYSCONF_NACK)
		{
			myAlert(null,"加载数据失败！",0,0);
		}
	});
	
	timerPlatStatus = window.setInterval(getPlatStatus,5*1000);
}
function saveSysConf()
{
	if(!UiUtil.checkNumber('inputmplatport','端口号'))
		return;
	if(!UiUtil.checkNumber('inputNo','设备入网id'))
		return;
	if($('inputNo').value.length != 32)
	{
		myAlert(null, "入网编号长度非法!",200,0);
		return;
	}
	
	CMonMsg.setId(G200_ET.BS_G200_SYSPARASET_REQ);
	
	CMonMsg.setMsgNode("mplatip",$('inputmplatip').value);
	CMonMsg.setMsgNode("mplatport",$('inputmplatport').value);
	CMonMsg.setMsgNode("mplattype",UiUtil.getSelectedIndexValue("selMplatType"));
	CMonMsg.setMsgNode("ip3a",$('inputip3a').value);
	CMonMsg.setMsgNode("port3a",$('inputport3a').value);
	CMonMsg.setMsgNode("no",$('inputNo').value);
	CMonMsg.setMsgNode("user3a",$('input3auser').value);
	CMonMsg.setMsgNode("psw3a",$('input3apsw').value);
	
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
		if(json.kedacomxmldata.command == G200_ET.G200_BS_SYSPARASET_ACK)
		{
			myAlert(null,"设置成功,请重新启动系统!",250,0);
			mplatip = $('inputmplatip').value;
			mplatport = $('inputmplatport').value;
			enterNetNo = $('inputNo').value;
			mplattype = UiUtil.getSelectedIndexValue("selMplatType");
			ip3a = $('inputip3a').value;
			port3a = $('inputport3a').value;
			user3a = $('input3auser').value;
			psw3a = $('input3apsw').value;
			$('btUpdateSysConf').onclick();
			//设置重新启动系统
		}
		else if(json.kedacomxmldata.command == G200_ET.G200_BS_SYSPARASET_NACK)
		{
			myAlert(null,"设置失败！",0,0);
		}
	});
}

function updateSysConf()
{
	if($('btUpdateSysConf').value == "修改")
	{
		$('btUpdateSysConf').value = "取消";
		$('mplattype').innerHTML = "<select id='selMplatType' style='width:120px'></select>";
		$('mplatip').innerHTML = "<input name='inputmplatip' type='text' class='inputIp' value ="+ mplatip + " onkeypress='return pressIpNumOnly();'/>";
		$('mplatport').innerHTML = "<input name='inputmplatport' type='text' class='inputPort' value="+ mplatport +" onkeypress='return pressNumOnly();'/>";
		$('ip3a').innerHTML = "<input name='inputip3a' type='text' class='inputIp' value="+ ip3a + " onkeypress='return pressIpNumOnly();'/>";
		$('port3a').innerHTML = "<input name='inputport3a' type='text' class='inputPort' value="+ port3a + " onkeypress='return pressNumOnly();'/>";
		$('No').innerHTML = "<input name='inputNo' type='text' class='inputEnterNet' value="+ enterNetNo +" onkeypress='return pressNumOnly();' maxlength='32'/>";
		$('3auser').innerHTML = "<input name='input3auser' type='text' class='inputIp' value="+ user3a +" maxlength='32'/>";
		$('3apsw').innerHTML = "<input name='input3apsw' type='text' class='inputIp' value="+ psw3a +" maxlength='32'/>";
		
		for(var value in MPLAT_TYPE)
		{
			UiUtil.addSelectedIndex("selMplatType",value,MPLAT_TYPE[value]);
		}
		UiUtil.setSelectedIndexValue("selMplatType",mplattype);
		
		//显示出应用按钮
		$('btAppSysConf').style.visibility = 'visible';
	}
	else if($('btUpdateSysConf').value == "取消")
	{
		$('btUpdateSysConf').value = "修改";
		
		//还原以前的值
		$('mplatip').innerHTML = mplatip;
		$('mplatport').innerHTML = mplatport;
		$('mplattype').innerHTML = MPLAT_TYPE[mplattype];
		$("ip3a").innerHTML = ip3a;		
		$("port3a").innerHTML = port3a; 
		$('No').innerHTML = enterNetNo;
		$('3auser').innerHTML = user3a;
		$('3apsw').innerHTML = psw3a;
		$('btAppSysConf').style.visibility = 'hidden';
	}
}

function savelocalIp()
{
	var gateway = $('inputdefaultGate').value;
	//hostip指定应修改的网卡
	CMonMsg.setId(G200_ET.BS_G200_SYSIPSET_REQ);
	CMonMsg.setMsgNode('hostip',location.host);
	CMonMsg.setMsgNode('selip',$('inputselIp').value);
	CMonMsg.setMsgNode('submask',$('inputsubMask').value);
	if(gateway == "undefined")
	{
		gateway = "";
	}
	CMonMsg.setMsgNode('defaultgate',gateway);
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
		if(json.kedacomxmldata.command == G200_ET.G200_BS_SYSIPSET_ACK)
		{
			myAlert(null,"设置成功,请重新启动系统!",250,0);
			selIp = $('inputselIp').value;
			subMask = $('inputsubMask').value;
			defaultGate = $('inputdefaultGate').value;
			
			$('btUpdateIp').onclick();
		}
		else if(json.kedacomxmldata.command == G200_ET.G200_BS_SYSIPSET_NACK)
		{
			myAlert(null,"设置失败！",0,0);
		}
	});
}

function updateLocalIp()
{
	if($('btUpdateIp').value == "修改")
	{
		$('btUpdateIp').value = "取消";
		$('selIp').innerHTML = "<input name='inputselIp' type='text' class='inputIp' value="+ selIp +" onkeypress='return pressIpNumOnly();'/>";
		$('subMask').innerHTML = "<input name='inputsubMask' type='text' class='inputIp' value="+ subMask+" onkeypress='return pressIpNumOnly();'/>";
		$('defaultGate').innerHTML = "<input name='inputdefaultGate' type='text' class='inputIp' value="+ defaultGate +" onkeypress='return pressIpNumOnly();'/>";
		
		$('btAppLocalIp').style.visibility = 'visible';
	}
	else if($('btUpdateIp').value == "取消")
	{
		$('btUpdateIp').value = "修改";
		
		//还原以前的值
		$('selIp').innerHTML = selIp;
		$('subMask').innerHTML = subMask;
		$('defaultGate').innerHTML = defaultGate;
		
		$('btAppLocalIp').style.visibility = 'hidden';
	}
}


function getPlatStatus()
{
	CMonMsg.setId(G200_ET.BS_G200MS_G200_MPLATSTATUS_REQ);
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
			if(json.kedacomxmldata.command == G200_ET.G200_G200MS_BS_MPLATSTATUS_ACK)
			{
					if(json.kedacomxmldata.content.mplatstatus == 0)
					{
						$("mplatStatus").innerHTML = "未连接";
					}
					else
					{
						$("mplatStatus").innerHTML = "已连接";
					}
					
			}
	});
	CMonMsg.setId(G200_ET.BS_G200MS_3ASTATUS_REQ);
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
			if(json.kedacomxmldata.command == G200_ET.G200MS_BS_3ASTATUS_ACK)
			{
					if(json.kedacomxmldata.content.status3a == 0)
					{
						$("3aStatus").innerHTML = "未连接";
					}
					else
					{
						$("3aStatus").innerHTML = "已连接";
					}
					
			}
	});
}
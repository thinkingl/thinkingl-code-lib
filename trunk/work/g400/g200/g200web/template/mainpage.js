// JavaScript Document


var undefined;
//定时获取平台状态
var timerPlatStatus;

//定时获取编码器状态
var timerEncoderStatus;


var totalMenuItems = 4;
function menu_Plat_Info_Click(index)
{

	dealTimer();
	setClassName(index);
	loadSysConfig();
}

function menu_Matrix_List_Click(index)
{
	dealTimer();
	setClassName(index);
	loadMatrixList();
}

function menu_Matrix_Conf_Click(index)
{
	dealTimer();
	setClassName(index);
	loadMatrixConf();
}


function menu_System_Main_Click(index)
{
	dealTimer();
	setClassName(index);
	loadSysMain();
}

function setClassName(index)
{
	var range = $R(0,totalMenuItems,true);
	range.each(function(val){
		var menu = "menu" + val;
		if(val == index)
		{
			$(menu).className = "sel";	
		}
		else
		{
			$(menu).className = "";
		}
	});
}

function dealTimer()
{
	if(timerPlatStatus != undefined)
	{
		window.clearInterval(timerPlatStatus);
		timerPlatStatus = undefined;
	}
	
	if(timerEncoderStatus != undefined)
	{
		window.clearInterval(timerEncoderStatus);
		timerEncoderStatus = undefined;
	}
	
}

//每十秒钟发送一次心跳
function sendHeartBeat()
{
	CMonMsg.setId(G200_ET.BS_G200_HEARTBEAT_NTF);
	ResourceLoader.sendXmlData(CMonMsg.getMsg());
}

function loadMainPage()
{
	ResourceLoader.loadHomePage("mainpage");
	
	var range = $R(0,totalMenuItems,true);
	range.each(function(val){
		var menu = "menu" + val;
		$(menu).onmouseover = function(){	
			if(!Ext.get(menu).hasClass("sel")){
				$(menu).className = "hvr";
			}
		};
		$(menu).onmousedown = function(){$(menu).className = "prs";};
		$(menu).onmouseout = function(){Ext.get(menu).removeClass("hvr");Ext.get(menu).removeClass("prs");};
	});
	menu_Plat_Info_Click(2);
	window.setInterval(sendHeartBeat,5*1000);
}


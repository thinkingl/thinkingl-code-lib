function loadSysMain()
{	
	ResourceLoader.loadTemplate("systemmain");
	
	var arrBts = $('submitReboot','selFile','updateSoftware');
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

	CMonMsg.setId(G200_ET.BS_G200MS_SYSMAIN_REQ);
	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
		if(!json)
		{
			return false;
		}
		if(json.kedacomxmldata.command != G200_ET.G200MS_BS_SYSMAIN_ACK)
		{
			myAlert(null,"加载系统信息失败...",280,0);
			return false;
		}
		var verData = json.kedacomxmldata.content;
		$('verInfo').innerHTML = verData.verinfo;
		if(verData.licenseinfo == 0)
		{
			$('licenseInfo').innerHTML = "许可证不可用";
		}
		else
		{
			$('licenseInfo').innerHTML = "许可证有效";
		}	
		$('mchKey').innerHTML = verData.mchkey;
		$('maxdate').innerHTML = verData.maxdate;
		$('approval').innerHTML = verData.approval;
		$('videonum').innerHTML = verData.videonum;
		$('encodernum').innerHTML = verData.encodernum;
	});
}

//系统重启
function systemReboot()
{
	Ext.Msg.confirm('提示',"重新启动系统",function(btn,text){
		if(btn == "yes"){
			CMonMsg.setId(G200_ET.BS_G200_SYSTEMREBOOT_REQ);
			ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
				if(!json)
				{
					return;
				}	
				if(json.kedacomxmldata.command == G200_ET.G200_BS_SYSTEMREBOOT_ACK)
				{
					location.href = "index.htm";
				}
				else if(json.kedacomxmldata.command == G200_ET.G200_BS_SYSTEMREBOOT_NACK)
				{
					myAlert(null,"重启系统失败！",0,0);
				}
			});
		}
		else
		{
			
		}
	});
	
}

function onSubmit(btSubmit)
{
	var fileName = $('fileName').value;
	if(fileName == "")
	{
		myAlert(null,"请选择升级的文件",0,0);
		return;
	}
	if(fileName.lastIndexOf("g200exe.tar") == -1)
	{
		myAlert(null,"请选择正确的软件升级包",230,0);
		return;
	}
	
	ResourceLoader.uploadFile(btSubmit,function(json){
		if(!json){
			return;
		}
		myAlert(null,"文件上传成功，请重新启动系统！",280,0);
		$('updateSoftware').disabled = false;
		$('selFile').disabled = false;
		$('file').disabled = false;
		$('fileName').value = "";
		$('file').value = "";
	});
	$('updateSoftware').disabled = true;
	$('selFile').disabled = true;
	$('file').disabled = true;
}
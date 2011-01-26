function goToVersion(){
 	ResourceLoader.loadTemplate("version",function(html){
        $(".rightTab").html(html);
        $('#version_restart').click(restart);    
		//$('#updatelicense').change(formsubmit);
        updateVer();
        button_hover_function();      
    }); 
};

function onSubmit(btSubmit)
{
	var fileName = $('#fileName').val();
	if(fileName == "" || $('#file').val() == "")
	{
		myAlert(null,"请选择升级的文件...",200,0);
		return;
	}
	
	if(fileName.lastIndexOf("upgrade.rar") == -1)
	{
		myAlert(null,"请选择正确的软件升级包",230,0);
		return;
	}
	
	ResourceLoader.uploadFile(btSubmit.form,function(json){
		if(!json){
			return;
		}
		
		var strResult = json.kedacomxmldata.content;
		if(strResult == "fail")
		{
			myAlert(null,"磁盘空间不足，上传文件失败",240,0);
		}
		else
		{
			Ext.Msg.show({
				title:"提示",
				msg: "文件上传成功，请重新启动系统",
				width:280,
				icon:Ext.MessageBox.INFO,
				buttons:{
					yes: true,
					no:true
				},
				fn:function(btn){
					if(btn == "yes")
					{
					   CMonMsg.setId(9);
					   CMonMsg.setMsgNode("restart","restart"); 	
					   ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
					   {
						   if(isErrorCommand(json)===false)
							return; 
						   location.href = "index.htm";
					   });
					}
				}
			});
			$('#fileName').val("");
			$('#file').val("");
		}
		$('#updateSoftware').attr('disabled',false);
		$('#selFile').attr('disabled',false);
		$('#file').attr('disabled',false);
	});
	
	$('#updateSoftware').attr('disabled','true');
	$('#selFile').attr('disabled','true');
	$('#file').attr('disabled','true');
}

function updateVer(){
        CMonMsg.setId(G100_COMMAND.COMMAND_GETVERSION);
    	CMonMsg.setMsgNode("version","version"); 	
    	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
    	{
	       if(isErrorCommand(json)===false)
	        return; 
           $("#version_ver").html(json.kedacomxmldata.content.version );
		
		   if(json.kedacomxmldata.content.licenseinfo == 0)
		   {
			   $("#licenseinfo").html("许可证无效");
		   }
		   else
		   {
			   $("#licenseinfo").html("许可证有效");
		   }
		   $("#mchkey").html(json.kedacomxmldata.content.mchkey);
		   $("#approval").html(json.kedacomxmldata.content.approval);
		   $("#maxdate").html(json.kedacomxmldata.content.maxdate);
		   $("#videonum").html(json.kedacomxmldata.content.videonum);
		   $("#dvrnum").html(json.kedacomxmldata.content.dvrnum);
    	},false);
}; 

function restart(){
     Ext.Msg.show({
        title:"提示",
		width:200,
        msg:"是否重启G100系统！",
		icon:Ext.MessageBox.INFO, 
        buttons:{
                    yes:true,
					no:true
        },
        fn:function(btn){
            if(btn =="yes"){
               CMonMsg.setId(9);
    	       CMonMsg.setMsgNode("restart","restart"); 	
           	   ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
           	   {
        	       if(isErrorCommand(json)===false)
        	        return; 
                   $("#version_ver").html(json.kedacomxmldata.content.version );  
				   location.href = "index.htm";
           	   }); 
            }
            else
            {
                return;
            }
        }
    });             
};


/*
	上传license文件
*/
function formsubmit(form)
{
	var fileName = $("#filelicense").val();
	if(fileName == "")
	{
		myAlert(null,"请选择升级的文件",0,0);
		return;
	}
	if(fileName.lastIndexOf("kedalicense.key") == -1)
	{
		myAlert(null,"请选择正确的文件导入",230,0);
		return;
	}
	ResourceLoader.uploadFile(form,function(json){
		if(!json){
			return;
		}
		/*Ext.Msg.show({
            title:"提示",
            msg: "文件上传成功，请重新启动系统",
            width:280,
            icon:Ext.MessageBox.INFO,
            buttons:{
                yes: true,
				no:true
            },
			fn:function(btn){
				if(btn == "yes")
				{
				   CMonMsg.setId(9);
				   CMonMsg.setMsgNode("restart","restart"); 	
				   ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
				   {
					   if(isErrorCommand(json)===false)
						return; 
					   location.href = "index.htm";
				   });
				}
			}
        });*/
		myAlert(null,"license更新成功!",200,0);
	});
}

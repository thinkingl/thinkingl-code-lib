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
		myAlert(null,"��ѡ���������ļ�...",200,0);
		return;
	}
	
	if(fileName.lastIndexOf("upgrade.rar") == -1)
	{
		myAlert(null,"��ѡ����ȷ�����������",230,0);
		return;
	}
	
	ResourceLoader.uploadFile(btSubmit.form,function(json){
		if(!json){
			return;
		}
		
		var strResult = json.kedacomxmldata.content;
		if(strResult == "fail")
		{
			myAlert(null,"���̿ռ䲻�㣬�ϴ��ļ�ʧ��",240,0);
		}
		else
		{
			Ext.Msg.show({
				title:"��ʾ",
				msg: "�ļ��ϴ��ɹ�������������ϵͳ",
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
			   $("#licenseinfo").html("���֤��Ч");
		   }
		   else
		   {
			   $("#licenseinfo").html("���֤��Ч");
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
        title:"��ʾ",
		width:200,
        msg:"�Ƿ�����G100ϵͳ��",
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
	�ϴ�license�ļ�
*/
function formsubmit(form)
{
	var fileName = $("#filelicense").val();
	if(fileName == "")
	{
		myAlert(null,"��ѡ���������ļ�",0,0);
		return;
	}
	if(fileName.lastIndexOf("kedalicense.key") == -1)
	{
		myAlert(null,"��ѡ����ȷ���ļ�����",230,0);
		return;
	}
	ResourceLoader.uploadFile(form,function(json){
		if(!json){
			return;
		}
		/*Ext.Msg.show({
            title:"��ʾ",
            msg: "�ļ��ϴ��ɹ�������������ϵͳ",
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
		myAlert(null,"license���³ɹ�!",200,0);
	});
}

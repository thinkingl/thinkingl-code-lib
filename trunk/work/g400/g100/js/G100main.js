var g100Info;
var mtuChecked;
var g_g100Type;// vsƽ̨ת����yyƽ̨ʱ��ģʽA��L
var g_g100SelList;//true����ѡ��g100�����б�
function goToG100Main(){
	ResourceLoader.loadTemplate("g100main",function(html){
		$(".rightTab").html(html);
		
		$("#btn_net_apply").hide();
		$("#btn_g100_apply").hide();    
		
		CMonMsg.setId(G100_COMMAND.COMMAND_GETG100);
		CMonMsg.setMsgNode("range","4");           //ֻ�õ�G100�����е���Ϣ
		CMonMsg.setMsgNode("host" , location.host);         
		ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
			 if(isErrorCommand(json)===false)
				 return;
			 g100Info = json.kedacomxmldata.content;
			 updateG100MainInfo(g100Info);
			 updateG100NetInfo(g100Info);    
		},false);
		   
		//�޸İ�������ʱ��Ӧ�ķ���
		$("#btn_g100_change").click(onG100Change); 
		$("#btn_net_change").click(onNetChange);
	
	
		$("#btn_g100_apply").click(onG100Apply);
		$("#btn_net_apply").click(onNetApply);    
		 //����ת�������check��Ӧ�¼�
		$('#main_g100Mtu').click(function(){
			if($(this).attr("checked") == true)
			{
				$(".mainEdit.Nat").attr("disabled",true); 
				$("#main_g100Nat").attr("checked",false);   
			}
			else
			{
				$(".mainEdit.Nat").removeAttr("disabled");
				$("#main_g100Nat").attr("checked",UiUtil.parseBoolFromNum(g100InfoLocal.nat));
			}    
		});           
		  
		 
		button_hover_function(); 
	
	});
        
};//goToG100Main over
  
function updateG100MainInfo(g100InfoLocal){
    if(g100InfoLocal.cmu.cmutype == "vs"){
         $('#g100_p_cmuType').html("��ҵ���");  
    }
    else{
         $('#g100_p_cmuType').html("��Ӫ");         
    }
           
    $('#g100_p_cmuIp').html(g100InfoLocal.cmu.cmuip);  
    $('#g100_p_cmuListenPort').html(g100InfoLocal.cmu.listenport);
    $('#g100_p_cmuPuPort').html(g100InfoLocal.cmu.puport);         
    
    if(g100InfoLocal.type == "A"){
        $('#g100_p_g100Type').html("����");
        $("#main_g100Mtu").attr("checked", false);  
		$("#main_g100Nat").attr("checked", UiUtil.parseBoolFromNum(g100InfoLocal.nat) );    
    }else if(g100InfoLocal.type == "L"){
        $('#g100_p_g100Type').html("����");  
        $("#main_g100Mtu").attr("checked" , false);    
		$("#main_g100Nat").attr("checked", UiUtil.parseBoolFromNum(g100InfoLocal.nat) );       
    }else if(g100InfoLocal.type == "AT"){
        $('#g100_p_g100Type').html("����"); 
        $("#main_g100Mtu").attr("checked" , true);      
    }else if(g100InfoLocal.type == "T"){
        $('#g100_p_g100Type').html("�ӻ�"); 
        $("#main_g100Mtu").attr("checked" , true);    
    } 
    $("#main_g100Mtu").attr("disabled",true);
	$(".mainEdit.Nat").attr("disabled",true);
};  
function updateG100NetInfo(g100InfoLocal){
    $("#g100_p_netIp").html(g100InfoLocal.net.netip);  
    $("#g100_p_netMask").html(g100InfoLocal.net.netmask);
    $("#g100_p_netGate").html(g100InfoLocal.net.netgate); 
};


function updateG100MainInfoInEditor(g100InfoLocal){
//����������TextField�ؼ�
     $('#g100_p_cmuType').html("<select name='select2' class='mainEdit cmu' id='main_cmuType'>"+
        "<option>��ҵ���</option>"+
        "<option>��Ӫ</option>"+
        "</select>");  
        
     if(g100InfoLocal.cmu.cmutype == "vs"){
         $('#main_cmuType').val("��ҵ���");
     }else{
         $('#main_cmuType').val("��Ӫ"); 
     }         
     $('#g100_p_cmuIp').html("<input type='text' size='15' class='mainEdit cmu' id='main_cmuIp'"+
        "value='"+g100InfoLocal.cmu.cmuip+"'/>");                          
     $('#g100_p_cmuListenPort').html("<input type='text' size='15' class='mainEdit cmu' id='main_cmuListenport'"+
        "value='"+g100InfoLocal.cmu.listenport+"'/>");
     $('#g100_p_cmuPuPort').html("<input  type='text' size='15' class='mainEdit cmu' id='main_cmupuport' "+
        "value='"+g100InfoLocal.cmu.puport+"'/>"); 
    
      
     if(g100InfoLocal.cmu.cmutype == "vs" && g100InfoLocal.type != "L"){
        $('#g100_p_g100Type').html("<select name='select' class='mainEdit cmu' id='main_g100Type'>"+
          "<option>����</option>"+
          "<option>�ӻ�</option>"+
          "</select>"); 
		g_g100SelList =  true;
     } 
   
	 if(g_g100SelList)
	 {
		 if(g100InfoLocal.type == "A"){
		
			$('#main_g100Type').val("����");
			$(".mainEdit.g100").removeAttr("disabled");
			
		 }else if(g100InfoLocal.type == "AT"){
			 
			$('#main_g100Type').val("����");   
			$(".mainEdit.g100").removeAttr("disabled");
				 
		 }else if(g100InfoLocal.type == "T"){
			$('#main_g100Type').val("�ӻ�");            
		 } 
	 }
     
	 if(g100InfoLocal.type == "L" || g100InfoLocal.type == "A")
	 {
	     $(".mainEdit.Nat").removeAttr("disabled");
	 }
 
    //ƽ̨���������б���Ӧ�¼�
    $('#main_cmuType').change(function(){
        if($(this).val() == "��ҵ���" && g100Info.type != "L"){
            $('#g100_p_g100Type').html("<select name='select' class='mainEdit cmu' id='main_g100Type'>"+
              "<option>����</option>"+
              "<option>�ӻ�</option>"+
              "</select>");   
            $(".mainEdit.g100").removeAttr("disabled");       
			
			
			//Ĭ����ʾ����
			if($('#main_g100Mtu').attr("checked") == false)
			{
				$(".mainEdit.Nat").removeAttr("disabled");
				$("#main_g100Nat").attr("checked",UiUtil.parseBoolFromNum(g100InfoLocal.nat) );//Aģʽ
			}
			else
			{
				$("#main_g100Nat").attr("checked",false);
				$(".mainEdit.Nat").attr("disabled",true);	
			}
			
			$('#main_g100Type').change(function(){
				if($(this).val() == "����"){
			
					$(".mainEdit.g100").removeAttr("disabled"); 
					$("#main_g100Mtu").attr("checked",false); 
					
					$(".mainEdit.Nat").removeAttr("disabled");
					$("#main_g100Nat").attr("checked",UiUtil.parseBoolFromNum(g100InfoLocal.nat) );//
				}
				else{
		
					$(".mainEdit.g100").attr("disabled",true); 
					$("#main_g100Mtu").attr("checked",true);   
					
					$(".mainEdit.Nat").attr("disabled",true);
					$("#main_g100Nat").attr("checked",false);
				}    
				
			});
			
			g_g100SelList =  true;
        }
        else
		{				
			if(g100Info.type == "A" || g100Info.type == "AT"){
				$('#g100_p_g100Type').html("����");
			}else{
				$('#g100_p_g100Type').html("����");  
			}
			
			$("#main_g100Mtu").attr("disabled","true"); 
			$("#main_g100Mtu").attr("checked",false);
			
			g_g100SelList =  false;
			
			//��ҵL����Ӫģʽ���Է���nat
			$(".mainEdit.Nat").removeAttr("disabled");
			$("#main_g100Nat").attr("checked",UiUtil.parseBoolFromNum(g100InfoLocal.nat));
		} 
    });     
	
	//G100���������б���Ӧ�¼�
	if(g_g100SelList)
	{
		$('#main_g100Type').change(function(){
			if($(this).val() == "����"){
		
				$(".mainEdit.g100").removeAttr("disabled");  
				$("#main_g100Mtu").attr("checked",false);
				
				$(".mainEdit.Nat").removeAttr("disabled");
				$("#main_g100Nat").attr("checked",UiUtil.parseBoolFromNum(g100InfoLocal.nat));
			}
			else if($(this).val() == "�ӻ�"){
	
				$(".mainEdit.g100").attr("disabled","true"); 
				$("#main_g100Mtu").attr("checked",true); 
				
				$(".mainEdit.Nat").attr("disabled",true);
				$("#main_g100Nat").attr("checked",false);
			}    
			
		});	
	}
};

function updateG100NetInfoInEditor(g100InfoLocal){
     $('#g100_p_netIp').html("<input type='text' size='15' class='mainEdit cmu' id='main_g100Ip'"+
        "value='"+g100InfoLocal.net.netip+"'/>");                          
     $('#g100_p_netMask').html("<input type='text' size='15' class='mainEdit cmu' id='main_g100Mask'"+
        "value='"+g100InfoLocal.net.netmask+"'/>");
     $('#g100_p_netGate').html("<input  type='text' size='15' class='mainEdit cmu' id='main_g100Gate' "+
        "value='"+g100InfoLocal.net.netgate+"'/>"); 
}

//�޸İ�ť��Ӧ�¼�
function onG100Change(){
    if($(this).attr('name') == "btn_change"){
         $("#btn_g100_apply").show();
         $("#btn_g100_change").attr('name' , 'btn_undo');
         $("#btn_g100_change").val("����");
         updateG100MainInfoInEditor(g100Info);
    }else{
         $("#btn_g100_apply").hide();
         $("#btn_g100_change").attr('name' , 'btn_change');
         $("#btn_g100_change").val("�޸�"); 
         updateG100MainInfo(g100Info);
    }
};

//�޸İ�ť��Ӧ�¼�
function onNetChange(){
    if($(this).attr('name') == "btn_change"){
         $("#btn_net_apply").show();
         $(btn_net_change).attr('name' , 'btn_undo');
         $(btn_net_change).val("����");
         updateG100NetInfoInEditor(g100Info);
    }else{
         $("#btn_net_apply").hide();
         $(btn_net_change).attr('name' , 'btn_change');
         $(btn_net_change).val("�޸�"); 
         updateG100NetInfo(g100Info);
    }
};

//����G100Type��Ӧ�ð�������ʱ��Ӧ
function onG100Apply(){
	var g100Type = false;
	
	if(!UiUtil.checkEmpty('main_cmuType','ƽ̨����'))
		return;
	
	if(!UiUtil.checkIp('main_cmuIp','�ϼ�ƽ̨IP'))
		return;
		
	if(!UiUtil.checkPstvNumber('main_cmuListenport','ƽ̨�����˿�'))
		return;
	if(parseInt($('#main_cmuListenport').val()) > 65535)
	{
		myAlert(null,"�������˿���65535!",220,0);
		return;
	}
	
	if(!UiUtil.checkPstvNumber('main_cmupuport','ǰ�˽���˿�'))
		return;
	if(parseInt($('#main_cmupuport').val()) > 65535)
	{
		myAlert(null,"�������˿���65535!",220,0);
		return;
	}
		
	if(g100Info.cmu.cmutype == "vs" && g100Info.type != "L")
	{
		if(!UiUtil.checkEmpty('main_g100Type','�豸����'))
			return;
	}
	
	if($("#main_cmuType").val()=="��ҵ���" && g100Info.cmu.cmutype!="vs"){
        g100Type = true;
    }
    if($("#main_cmuType").val()=="��Ӫ" && g100Info.cmu.cmutype!="yy"){
        g100Type = true;
    }    
    if($("#main_cmuIp").val()!=g100Info.cmu.cmuip){

        g100Type = true;
    }      
    if($("#main_cmuListenport").val()!=g100Info.cmu.listenport){
        g100Type = true;
    }
    if($("#main_cmupuport").val()!=g100Info.cmu.puport){
        g100Type = true;
    }    
	
	if(g100Info.nat != Number($("#main_g100Nat").attr("checked")))
	{
		g100Type = true;
	}
	
	if(g_g100SelList)
	{
		if($("#main_g100Type").val()== "����" && g100Info.type!="A" && g100Info.type!="AT"){
			//alert("�ӻ�ת������");
			g100Type = true;
		}
		
		if($("#main_g100Type").val()=="�ӻ�" && g100Info.type != "T")
		{
			//alert("����ת���ӻ�");
			g100Type = true;
		}
		
		if(g100Info.type=="A" && $('#main_g100Mtu').attr("checked")==true){
			 //alert("A->AT");
			 g100Type = true;   
		}
		if(g100Info.type=="AT" && $('#main_g100Mtu').attr("checked")==false){
			 //alert("AT->A");
			 g100Type = true;   
		}  
	}
	
	
	if(g100Type)
	{
		CMonMsg.setId(G100_COMMAND.COMMAND_CHANGEG100TYPE);
		var valueTemp = $("#main_cmuType").val();
		if(valueTemp == "��ҵ���"){
			   CMonMsg.setMsgNode("cmutype","vs");    
		}else{
			   CMonMsg.setMsgNode("cmutype","yy");    
		}
		CMonMsg.setMsgNode("cmuip",$('#main_cmuIp').val());    
		CMonMsg.setMsgNode("cmulistenport",$('#main_cmuListenport').val());
		CMonMsg.setMsgNode("cmupuiport",$('#main_cmupuport').val());
		
		if(g_g100SelList)
		{
			var valueTemp = $("#main_g100Type").val();
			if(valueTemp == "����"){
			if($("#main_g100Mtu").attr("checked") == true)
				CMonMsg.setMsgNode("g100type","AT");  
			else
				CMonMsg.setMsgNode("g100type","A");                            
			}else if(valueTemp == "�ӻ�"){	
				CMonMsg.setMsgNode("g100type","T");                             
			}
		}
		else
		{
			if(g100Info.type != "L")
			{
				CMonMsg.setMsgNode("g100type","A");
				g_g100Type = "A";
			}
			else
			{
				CMonMsg.setMsgNode("g100type","L");
				g_g100Type = "L";
			}
		}
		CMonMsg.setMsgNode("nat",Number($("#main_g100Nat").attr("checked")));
		
		ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
		{
			if(isErrorCommand(json)===false)
			   return;
			var value = json.kedacomxmldata.content.status;
			if(value == "1"){
				saveToLocal();
				OnProcessAlterG100Cfg(); //��ʾ�޸�״̬
				onG100Change();//�ӱ༭״̬����
				
			 }else if(value == "2"){
				 myAlert(null,"�޸�ʧ�ܣ�������",0,0);   
			 }
			 else
			 {
				myAlert(null,"�޸�G100���ã�ĩ����ķ���ֵ��",0,0);
			 }
		},false); //ͬ����ʽ
	}	
	else
	{
		myAlert(null,"û���κθĶ���",200,0);
	}
};

function onNetApply(){
	if(!UiUtil.checkIp('main_g100Ip','G100IP��ַ'))
		return;
	if(!UiUtil.checkIp('main_g100Mask','G100��������'))
		return;
    Ext.Msg.show({
        title: '�豸����',
        msg: 'G100�豸��Ҫ�������޸Ĳ��������ã��Ƿ��޸�',
		icon:Ext.MessageBox.QUESTION,
		width:350,
        buttons:{
            yes: true,
            no: true
        },
        fn:function(btn){
            if(btn == "yes"){   //�������yes������Ҫ���ͷ�������������
            	CMonMsg.setId(G100_COMMAND.COMMAND_CHANGENET);
            	CMonMsg.setMsgNode("interface",location.host);    
            	CMonMsg.setMsgNode("netip",$('#main_g100Ip').val());
            	CMonMsg.setMsgNode("netmask",$('#main_g100Mask').val());
            	CMonMsg.setMsgNode("netgate",$('#main_g100Gate').val());	
            	
             	ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json)
            	{
            	    if(isErrorCommand(json)===false)
            	       return;
            		var value = json.kedacomxmldata.content.status;
                    if(value == "1"){
                    	location.reload();
                     }else if(value == "2"){
						 myAlert(null, "�޸�ʧ�ܣ�������",200,0); 
                     }
                     else
                     {
						myAlert(null, "�޸�G100�������ã�ĩ����ķ���ֵ��",280,0); 
                     }
            	},false);                      
            }
			else
			{
				onNetChange();
			}
        }
    });    	
};

//�޸�G100���ûص�����
function OnProcessAlterG100Cfg(){
	 Ext.Msg.show({
				title: '��ʾ',
				icon:Ext.MessageBox.INFO, 
				width:420,
				msg: '�����ļ����޸ĳɹ�,����������Ч,�Ƿ���������G100',
				buttons:{
					yes: true,
					no:true
				},
				fn:function(btn){
					if(btn == "yes"){
						CMonMsg.setId(G100_COMMAND.COMMAND_RESTARTG100);
						CMonMsg.setMsgNode("restart","restart");
						ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
							if(isErrorCommand(json)===false)
							   return;
							var value = json.kedacomxmldata.content.status;
							if(value == "1"){
								location.reload();
							 }else if(value == "2"){
								 myAlert(null,"�޸�ʧ�ܣ�������",200,0);
							 }
							 else
							 {
								 myAlert(null,"�޸�G100���ã�ĩ����ķ���ֵ��",250,0);
							 }                
						},false);
					}
				}
	}); 
}


//�޸�G100���ûص�����
function saveToLocal()
{
	if($("#main_cmuType").val() == "��ҵ���")
	{
		g100Info.cmu.cmutype = "vs";
	}
	else
	{
		g100Info.cmu.cmutype = "yy";
	}
	g100Info.cmu.cmuip = $("#main_cmuIp").val();
	g100Info.cmu.listenport = $("#main_cmuListenport").val();
	g100Info.cmu.puport = $("#main_cmupuport").val();
	
	if(g_g100SelList)
	{
		if($("#main_g100Type").val() == "�ӻ�")
		{
			g100Info.type = "T";
		}
		else if($('#main_g100Mtu').attr("checked") == true)
		{
			g100Info.type = "AT";
		}
		else
		{
			g100Info.type = "A";
		}	
	}
	else
	{
		g100Info.type = g_g100Type;
	}
	
	g100Info.nat = Number($("#main_g100Nat").attr("checked"));
}

 

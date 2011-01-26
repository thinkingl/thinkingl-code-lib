var g100Info;
var mtuChecked;
var g_g100Type;// vs平台转换成yy平台时的模式A，L
var g_g100SelList;//true存在选择g100类型列表
function goToG100Main(){
	ResourceLoader.loadTemplate("g100main",function(html){
		$(".rightTab").html(html);
		
		$("#btn_net_apply").hide();
		$("#btn_g100_apply").hide();    
		
		CMonMsg.setId(G100_COMMAND.COMMAND_GETG100);
		CMonMsg.setMsgNode("range","4");           //只得到G100的所有的信息
		CMonMsg.setMsgNode("host" , location.host);         
		ResourceLoader.loadXmlData(CMonMsg.getMsg(),function(json){
			 if(isErrorCommand(json)===false)
				 return;
			 g100Info = json.kedacomxmldata.content;
			 updateG100MainInfo(g100Info);
			 updateG100NetInfo(g100Info);    
		},false);
		   
		//修改按键按下时响应的方法
		$("#btn_g100_change").click(onG100Change); 
		$("#btn_net_change").click(onNetChange);
	
	
		$("#btn_g100_apply").click(onG100Apply);
		$("#btn_net_apply").click(onNetApply);    
		 //起用转码服务器check响应事件
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
         $('#g100_p_cmuType').html("行业监控");  
    }
    else{
         $('#g100_p_cmuType').html("运营");         
    }
           
    $('#g100_p_cmuIp').html(g100InfoLocal.cmu.cmuip);  
    $('#g100_p_cmuListenPort').html(g100InfoLocal.cmu.listenport);
    $('#g100_p_cmuPuPort').html(g100InfoLocal.cmu.puport);         
    
    if(g100InfoLocal.type == "A"){
        $('#g100_p_g100Type').html("主机");
        $("#main_g100Mtu").attr("checked", false);  
		$("#main_g100Nat").attr("checked", UiUtil.parseBoolFromNum(g100InfoLocal.nat) );    
    }else if(g100InfoLocal.type == "L"){
        $('#g100_p_g100Type').html("内联");  
        $("#main_g100Mtu").attr("checked" , false);    
		$("#main_g100Nat").attr("checked", UiUtil.parseBoolFromNum(g100InfoLocal.nat) );       
    }else if(g100InfoLocal.type == "AT"){
        $('#g100_p_g100Type').html("主机"); 
        $("#main_g100Mtu").attr("checked" , true);      
    }else if(g100InfoLocal.type == "T"){
        $('#g100_p_g100Type').html("从机"); 
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
//创建各个可TextField控件
     $('#g100_p_cmuType').html("<select name='select2' class='mainEdit cmu' id='main_cmuType'>"+
        "<option>行业监控</option>"+
        "<option>运营</option>"+
        "</select>");  
        
     if(g100InfoLocal.cmu.cmutype == "vs"){
         $('#main_cmuType').val("行业监控");
     }else{
         $('#main_cmuType').val("运营"); 
     }         
     $('#g100_p_cmuIp').html("<input type='text' size='15' class='mainEdit cmu' id='main_cmuIp'"+
        "value='"+g100InfoLocal.cmu.cmuip+"'/>");                          
     $('#g100_p_cmuListenPort').html("<input type='text' size='15' class='mainEdit cmu' id='main_cmuListenport'"+
        "value='"+g100InfoLocal.cmu.listenport+"'/>");
     $('#g100_p_cmuPuPort').html("<input  type='text' size='15' class='mainEdit cmu' id='main_cmupuport' "+
        "value='"+g100InfoLocal.cmu.puport+"'/>"); 
    
      
     if(g100InfoLocal.cmu.cmutype == "vs" && g100InfoLocal.type != "L"){
        $('#g100_p_g100Type').html("<select name='select' class='mainEdit cmu' id='main_g100Type'>"+
          "<option>主机</option>"+
          "<option>从机</option>"+
          "</select>"); 
		g_g100SelList =  true;
     } 
   
	 if(g_g100SelList)
	 {
		 if(g100InfoLocal.type == "A"){
		
			$('#main_g100Type').val("主机");
			$(".mainEdit.g100").removeAttr("disabled");
			
		 }else if(g100InfoLocal.type == "AT"){
			 
			$('#main_g100Type').val("主机");   
			$(".mainEdit.g100").removeAttr("disabled");
				 
		 }else if(g100InfoLocal.type == "T"){
			$('#main_g100Type').val("从机");            
		 } 
	 }
     
	 if(g100InfoLocal.type == "L" || g100InfoLocal.type == "A")
	 {
	     $(".mainEdit.Nat").removeAttr("disabled");
	 }
 
    //平台类型下拉列表响应事件
    $('#main_cmuType').change(function(){
        if($(this).val() == "行业监控" && g100Info.type != "L"){
            $('#g100_p_g100Type').html("<select name='select' class='mainEdit cmu' id='main_g100Type'>"+
              "<option>主机</option>"+
              "<option>从机</option>"+
              "</select>");   
            $(".mainEdit.g100").removeAttr("disabled");       
			
			
			//默认显示主机
			if($('#main_g100Mtu').attr("checked") == false)
			{
				$(".mainEdit.Nat").removeAttr("disabled");
				$("#main_g100Nat").attr("checked",UiUtil.parseBoolFromNum(g100InfoLocal.nat) );//A模式
			}
			else
			{
				$("#main_g100Nat").attr("checked",false);
				$(".mainEdit.Nat").attr("disabled",true);	
			}
			
			$('#main_g100Type').change(function(){
				if($(this).val() == "主机"){
			
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
				$('#g100_p_g100Type').html("主机");
			}else{
				$('#g100_p_g100Type').html("内联");  
			}
			
			$("#main_g100Mtu").attr("disabled","true"); 
			$("#main_g100Mtu").attr("checked",false);
			
			g_g100SelList =  false;
			
			//行业L和运营模式可以发送nat
			$(".mainEdit.Nat").removeAttr("disabled");
			$("#main_g100Nat").attr("checked",UiUtil.parseBoolFromNum(g100InfoLocal.nat));
		} 
    });     
	
	//G100类型下拉列表响应事件
	if(g_g100SelList)
	{
		$('#main_g100Type').change(function(){
			if($(this).val() == "主机"){
		
				$(".mainEdit.g100").removeAttr("disabled");  
				$("#main_g100Mtu").attr("checked",false);
				
				$(".mainEdit.Nat").removeAttr("disabled");
				$("#main_g100Nat").attr("checked",UiUtil.parseBoolFromNum(g100InfoLocal.nat));
			}
			else if($(this).val() == "从机"){
	
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

//修改按钮响应事件
function onG100Change(){
    if($(this).attr('name') == "btn_change"){
         $("#btn_g100_apply").show();
         $("#btn_g100_change").attr('name' , 'btn_undo');
         $("#btn_g100_change").val("撤销");
         updateG100MainInfoInEditor(g100Info);
    }else{
         $("#btn_g100_apply").hide();
         $("#btn_g100_change").attr('name' , 'btn_change');
         $("#btn_g100_change").val("修改"); 
         updateG100MainInfo(g100Info);
    }
};

//修改按钮响应事件
function onNetChange(){
    if($(this).attr('name') == "btn_change"){
         $("#btn_net_apply").show();
         $(btn_net_change).attr('name' , 'btn_undo');
         $(btn_net_change).val("撤销");
         updateG100NetInfoInEditor(g100Info);
    }else{
         $("#btn_net_apply").hide();
         $(btn_net_change).attr('name' , 'btn_change');
         $(btn_net_change).val("修改"); 
         updateG100NetInfo(g100Info);
    }
};

//按下G100Type的应用按键按下时响应
function onG100Apply(){
	var g100Type = false;
	
	if(!UiUtil.checkEmpty('main_cmuType','平台类型'))
		return;
	
	if(!UiUtil.checkIp('main_cmuIp','上级平台IP'))
		return;
		
	if(!UiUtil.checkPstvNumber('main_cmuListenport','平台监听端口'))
		return;
	if(parseInt($('#main_cmuListenport').val()) > 65535)
	{
		myAlert(null,"超过最大端口数65535!",220,0);
		return;
	}
	
	if(!UiUtil.checkPstvNumber('main_cmupuport','前端接入端口'))
		return;
	if(parseInt($('#main_cmupuport').val()) > 65535)
	{
		myAlert(null,"超过最大端口数65535!",220,0);
		return;
	}
		
	if(g100Info.cmu.cmutype == "vs" && g100Info.type != "L")
	{
		if(!UiUtil.checkEmpty('main_g100Type','设备类型'))
			return;
	}
	
	if($("#main_cmuType").val()=="行业监控" && g100Info.cmu.cmutype!="vs"){
        g100Type = true;
    }
    if($("#main_cmuType").val()=="运营" && g100Info.cmu.cmutype!="yy"){
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
		if($("#main_g100Type").val()== "主机" && g100Info.type!="A" && g100Info.type!="AT"){
			//alert("从机转到主机");
			g100Type = true;
		}
		
		if($("#main_g100Type").val()=="从机" && g100Info.type != "T")
		{
			//alert("主机转到从机");
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
		if(valueTemp == "行业监控"){
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
			if(valueTemp == "主机"){
			if($("#main_g100Mtu").attr("checked") == true)
				CMonMsg.setMsgNode("g100type","AT");  
			else
				CMonMsg.setMsgNode("g100type","A");                            
			}else if(valueTemp == "从机"){	
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
				OnProcessAlterG100Cfg(); //提示修改状态
				onG100Change();//从编辑状态返回
				
			 }else if(value == "2"){
				 myAlert(null,"修改失败！请重试",0,0);   
			 }
			 else
			 {
				myAlert(null,"修改G100配置，末定义的返回值！",0,0);
			 }
		},false); //同步方式
	}	
	else
	{
		myAlert(null,"没有任何改动！",200,0);
	}
};

function onNetApply(){
	if(!UiUtil.checkIp('main_g100Ip','G100IP地址'))
		return;
	if(!UiUtil.checkIp('main_g100Mask','G100子网掩码'))
		return;
    Ext.Msg.show({
        title: '设备重启',
        msg: 'G100设备需要重启，修改才能起做用，是否修改',
		icon:Ext.MessageBox.QUESTION,
		width:350,
        buttons:{
            yes: true,
            no: true
        },
        fn:function(btn){
            if(btn == "yes"){   //如果点了yes按键就要发送服务器重启命令
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
						 myAlert(null, "修改失败！请重试",200,0); 
                     }
                     else
                     {
						myAlert(null, "修改G100网络配置，末定义的返回值！",280,0); 
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

//修改G100配置回调函数
function OnProcessAlterG100Cfg(){
	 Ext.Msg.show({
				title: '提示',
				icon:Ext.MessageBox.INFO, 
				width:420,
				msg: '配置文件已修改成功,重启后方能生效,是否现在重启G100',
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
								 myAlert(null,"修改失败！请重试",200,0);
							 }
							 else
							 {
								 myAlert(null,"修改G100配置，末定义的返回值！",250,0);
							 }                
						},false);
					}
				}
	}); 
}


//修改G100配置回调函数
function saveToLocal()
{
	if($("#main_cmuType").val() == "行业监控")
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
		if($("#main_g100Type").val() == "从机")
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

 

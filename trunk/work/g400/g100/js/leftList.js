function LEFT_GUAID(){
    var isClicked = false;
    $("#leftGuaid > p").hover(function(){
        isClicked = false;
        if($(this).attr('class') == "leftGuaidList_disable")   
            return;    
        $(this).attr('style',"cursor:hand");
        if($(this).attr('class') == "leftGuaidList_clicked"){
            isClicked = true;
        } 
        $(this).attr('class' , 'leftGuaidList_hovered');           
    },function(){  
        if($(this).attr('class') == "leftGuaidList_disable")   
            return;      
        if(isClicked){
             $(this).attr('class' , 'leftGuaidList_clicked');           
        }else{
             $(this).attr('class' , 'leftGuaidList');      
        }
    });
    
    $("#leftGuaid > p").mousedown(function(){
        if($(this).attr('class') == "leftGuaidList_disable")   
            return;   
        $(this).attr('class' , 'leftGuaidList_pressed');     
    
    });
    
    $("#leftGuaid > p").click(function(){
        if($(this).attr('class') == "leftGuaidList_disable")   
            return;
            
        $("#leftGuaid > p").each(function(index){
            if($(this).attr('class') != "leftGuaidList_disable")
                $(this).attr('class' , 'leftGuaidList');   
        });
        $(this).attr('class' , 'leftGuaidList_clicked');
        isClicked = true;
        if ($(this).attr('id') == "main_kdm") {
            isInDvrList=false;
			mtuStatusTimer = false;
			goToKDMStatus();
        } else if ($(this).attr('id') == "main_kdm_device") {
            isInDvrList=false;   
			mtuStatusTimer = true;
			goToKDMDeviceList();
        } else if ($(this).attr('id') == "main_mt_log") {
            isInDvrList=true; 
			mtuStatusTimer = false;
			goToMTLog();
        } else if ($(this).attr('id') == "main_sys_config") {
           isInDvrList=false;
		   mtuStatusTimer = false;
		   goToSysCfg();
       } else if ($(this).attr('id') == "main_sys_manage") {
           isInDvrList = false;
           mtuStatusTimer = false;
           goToSysMng();
        }
    });
};

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
        if($(this).attr('id') == "main_g100Info"){
            isInDvrList=false;
			mtuStatusTimer = false;
            goToG100Main();    
        }else if($(this).attr('id') == "main_mtu"){
            isInDvrList=false;   
			mtuStatusTimer = true;
            goToMTUDetails();    
        }else if($(this).attr('id') == "main_dvr"){
            isInDvrList=true; 
			mtuStatusTimer = false;
            goToDvrlist();    
        }else if($(this).attr('id') == "main_g100Man"){
           isInDvrList=false;
		   mtuStatusTimer = false;
           goToVersion();      
        }
    });
};

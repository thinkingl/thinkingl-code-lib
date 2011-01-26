var G100_CONST={
    RULE_FORMAT_MPEG4_CLIENT :'MPEG4',
    RULE_FORMAT_MPEG4_SERVER : '97',
    RULE_CIF_QCIF_CLIENT : 'QCIF',
    RULE_CIF_QCIF_SERVER: '2',
    RULE_CIF_CIF_CLIENT : 'CIF',
    RULE_CIF_CIF_SERVER: '3',    
    RULE_CIF_2CIF_CLIENT : '2CIF',
    RULE_CIF_2CIF_SERVER: '4',    
    RULE_CIF_3CIF_CLIENT : 'D1',
    RULE_CIF_3CIF_SERVER: '5'    
};

var G100_COMMAND={
    COMMAND_BASE : 0,
    COMMAND_ERROR :				 0,
    COMMAND_LOGION:				 1,
    COMMAND_CHANGEPASSWORD:		 2,
    COMMAND_GETG100	:			 3,
    COMMAND_GETDVRS	:		     4,
    COMMAND_GETDVRDETAIL :       5,
    COMMAND_GETMTU	:			 6,
    COMMAND_GETMTUDETAIL:		 7,
    COMMAND_CHANGEG100TYPE:		 8,
    COMMAND_RESTARTG100	:		 9,
    COMMAND_STOPDVR	:			 10,
    COMMAND_STARTDVR:		     11,
    COMMAND_DELETEDVR:			 12,
    COMMAND_CHANGEDVR:			 13,
    COMMAND_CHANGEMTU  : 		 14,
    COMMAND_CHANGENET :  		 15,
    COMMAND_CHANGEMTURULE	:	 16,
    COMMAND_GETVERSION	:	     17,
    COMMAND_UNLOGIN  :           18,
    COMMAND_HERTSKIP:		     19,
    COMMAND_GETALTERED:          20,
	COMMAND_EXPORTCMD:           21,
	COMMAND_STARTMTU:            22,
	COMMAND_STOPMTU:             23,
	COMMAND_EDITMTU:             24,
	COMMAND_FACTORYTYPE:         25,
	COMMAND_GETDVRCONFIG:        26,
	COMMAND_SETDVRCONFIG:        27
};

var isInDvrList = false;

function convetRuleFormat(format , server2client){
    if(format === true){
//        if(format.toUpperCase() == G100_CONST.RULE_FORMAT_MPEG4_SERVER)
            return  G100_CONST.RULE_FORMAT_MPEG4_CLIENT;
    }else{
//        if(format.toUpperCase() ==  G100_CONST.RULE_FORMAT_MPEG4_CLIENT)
            return  G100_CONST.RULE_FORMAT_MPEG4_SERVER;      
    }
}

function convetRuleCif(format , server2client){
    if(format === true){
        if(format.toUpperCase() == G100_CONST.RULE_CIF_QCIF_SERVER){
            return G100_CONST.RULE_CIF_QCIF_CLIENT;        
        }else if(format.toUpperCase() == G100_CONST.RULE_CIF_CIF_SERVER){
            return G100_CONST.RULE_CIF_CIF_CLIENT;  
        }else if(format.toUpperCase() == G100_CONST.RULE_CIF_2CIF_SERVER){
            return G100_CONST.RULE_CIF_2CIF_CLIENT;  
        }else if(format.toUpperCase() == G100_CONST.RULE_CIF_3CIF_SERVER){
            return G100_CONST.RULE_CIF_3CIF_CLIENT;  
        }else{
            return G100_CONST.RULE_CIF_CIF_CLIENT;  
        }
    }else{
        if(format.toUpperCase() == G100_CONST.RULE_CIF_QCIF_CLIENT){
            return G100_CONST.RULE_CIF_QCIF_SERVER;        
        }else if(format.toUpperCase() == G100_CONST.RULE_CIF_CIF_CLIENT){
            return G100_CONST.RULE_CIF_CIF_SERVER;  
        }else if(format.toUpperCase() == G100_CONST.RULE_CIF_2CIF_CLIENT){
            return G100_CONST.RULE_CIF_2CIF_SERVER;  
        }else if(format.toUpperCase() == G100_CONST.RULE_CIF_3CIF_CLIENT){
            return G100_CONST.RULE_CIF_3CIF_SERVER;  
        }else{
            return G100_CONST.RULE_CIF_CIF_SERVER;  
        }     
    }  
};

function myAlert(myTitle , myMessage , myWidth , myHeight){
        if(myTitle == null)
           myTitle = "ÌבÊ¾";
        if(myWidth == 0)
            myWidth = 400;
        if(myHeight == 0)
            myHeight = 250;
            
        Ext.Msg.show({
            title:myTitle,
            msg: myMessage,
            width:myWidth,
            height:myHeight,
            icon:Ext.MessageBox.INFO,
            buttons:{
                yes: true
            }
        });
};

function button_hover_function(){
      $('#btLog').hover(function(){
                 $(this).attr('class','btLoginOver');                  
      },function(){
                $(this).attr('class','btLoginNor');                           
      }); 
      $("#btLog").mousedown(function(){
                $(this).attr('class','btLoginDown');                            
      });   
	  $(".mainButton").hover(function(){
                
                $(this).css({'color':'#0036FF','background-image': 'url(../images/button/btn_over.bmp)'});      
      },function(){
               $(this).css({'color':'#205699','background-image': 'url(../images/button/btn.bmp)'});      
      });    
      $(".mainButton").mousedown(function(){     
               $(this).css({'color':'#0036FF','background-image': 'url(../images/button/btn_press.bmp)'});            
      });  
};


var Try = {
  these: function() {
    var returnValue;

    for (var i = 0, length = arguments.length; i < length; i++) {
      var lambda = arguments[i];
      try {
        returnValue = lambda();
        break;
      } catch (e) {}
    }

    return returnValue;
  }
}


function GetAjaxObj()
{
	return Try.these(
      function() {return new XMLHttpRequest()},
      function() {return new ActiveXObject('Msxml2.XMLHTTP')},
      function() {return new ActiveXObject('Microsoft.XMLHTTP')}
    ) || false;
}

function ajGet(szURI, fCall)
{
    var obj = GetAjaxObj();
   
    obj.onreadystatechange = function()
    {   
      if(obj.readyState==4)
      {
          if(obj.status==200)
          {
                fCall(obj.responseText,true);
          }
          else
          {
               fCall(obj.responseText,false);
          }
		  obj.onreadystatechange = function(){};
		  obj = null;
      }
    } 
   obj.open('GET',szURI,true);
   obj.send(null);

}

function ajPost(szURI, sztext, fCall,bSynParam)
{
   var obj = GetAjaxObj();
   
   var bASyn = true;
   if(bSynParam != undefined)
   {
		bASyn = !bSynParam;
   }
   
   obj.open('POST',szURI,bASyn);
	
   var timeOut = 30000;//超时时间为30秒  
   var timer = setTimeout(function(){
	    obj.abort();  
	    obj.onreadystatechange = function(){};
	    obj = null;												
	},timeOut);
   
   obj.onreadystatechange = function()
   {   
	  if(obj.readyState==4)
	  {      
		  clearTimeout(timer);
		  
		  if(obj.status >= 200 && obj.status < 300 || obj.status == 304)
		  {
			  fCall(obj,true);
		  }
		  else
		  {
			   fCall(obj,false);
		  }
		  obj.onreadystatechange = function(){};
		  obj = null;
	  }
   } 
   
   obj.send(sztext);
}


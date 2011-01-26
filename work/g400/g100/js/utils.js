
/**
 * utility.js
 *
 * @author lqp
 * @version 1.0 Date 2010-1-10
 *
 */
function unspace()
{
    if(window.event.keyCode==32)
     return false;
}
/*check whether or not text field is num, and only num and dot can be shown*/
function pressNumOnly()
{
	if(window.event.keyCode >= 48 && window.event.keyCode <= 57){
		return true;
	}
	return false;
} 

function pressIpNumOnly()
{
	if(window.event.keyCode >= 48 && window.event.keyCode <= 57 || window.event.keyCode == 46){
		return true;
	}
	return false;
}

var UiUtil = {
	// 设置下拉元素的初始值

	setSelectedIndexValue : function (selectId,value){
		var someNodeList = $(selectId).getElementsByTagName('option');
		var nodes = $A(someNodeList);
		nodes.each(function(node){
			if(node.value == value){
				node.selected = "selected";
				return false;
			}
		});
	},

	setSelectedIndexText : function(selectId,text){
			var someNodeList = $(selectId).getElementsByTagName('option');
			var nodes = $A(someNodeList);
			nodes.each(function(node){
				 if(node.text == text){
					node.selected = "selected";	 
					return false;
			     }
			})
		},
		
	getSelectedIndexValue : function (selectId){
		var someNodeList = $(selectId).getElementsByTagName('option');
		var nodes = $A(someNodeList);
		var value = -1;
		nodes.each(function(node){
			if(node.selected){
				value = node.value;
				return false;
			}
		});
		return value;
	},

	getSelectedIndexText : function(selectId){
		var someNodeList = $(selectId).getElementsByTagName('option');
		var nodes = $A(someNodeList);
		var text = "";
		nodes.each(function(node){
			if(node.selected){
				text = node.text;
				return false;
			}
		});
		return text;
	},

	delSelectedIndex :function (selectId){
		var op =  $(selectId).options;

		for(var i=0;i<op.length;i++)
		{
			if(op[i].selected)
			op.remove(i);
		}
	},

	addSelectedIndex :function (selectId,value,text){
		var op =  $(selectId).options;
		var creatOption = new Option();
		creatOption.value = value;
		creatOption.text = text;
		op[op.length] = creatOption;
	},

	
	//设置所选中通道菜单的样式
	setClassName : function(selectedNum,channelNum){
		
		for (var i=0;i < channelNum ;i++ ){
			if(i == selectedNum){
				$("tab"+i).className = "current";
		
			}else{
				$("tab"+i).className = "";
				
			}
		}
	},	
		
	//设置复选框 
	setCheckBox : function(checkBoxId,value){
		if(value == 1){
			$(checkBoxId).checked = true;
		}else{
			$(checkBoxId).checked = false;
		}
	},	

	getCheckBox : function(checkBoxId){
		if($(checkBoxId).checked){
			return 1;
		}else{
			return 0;
		}
	},

	setDisabled : function(triggerId){
		if($(triggerId).checked){
			for(var i=1;i<arguments.length;i++){
				$(arguments[i]).disabled = false;
			}
		}else{
			for(var i=1;i<arguments.length;i++){
				$(arguments[i]).disabled = true;
			}
		}
	},

	setEnabled : function(triggerId){
		if(!$(triggerId).checked){
			for(var i=1;i<arguments.length;i++){
				$(arguments[i]).disabled = false;
			}
		}else{
			for(var i=1;i<arguments.length;i++){
				$(arguments[i]).disabled = true;
			}
		}
	},

	splitIP: function (element,address){
		var arr = address.split(".");
		for(var i=0;i<4;i++){
			$(element+i).value = arr[i];
		}
	},

	connectIP: function (element){
		var IP = "";
		for(var i=0;i<4;i++){
			IP += $F(element+i);
			if(i<3)
			IP +=".";
		}
		return IP;
	},
	
	parseNumFromBool:function(element){
		if(element)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	},
	
	parseBoolFromNum:function(element){
		if(parseInt(element) > 0)
		{
			return true;	
		}
		else
		{
			return false;
		}
	},
	
	checkEmpty: function (name,msg)
	{
		var obj = $('#' + name);
		if(obj.val() == "")
		{
			myAlert(null,"请输入"+msg,250,0);
			obj.focus();
			return false;
		}
		return true;
	},
	
	checkNumber:function(name,msg)
	{
		var obj = $('#' + name);
		var bRet = Validation.isInteger(obj.val());
		if(!bRet)
		{
			myAlert(null, msg + ":输入非法!",250,0);
			obj.focus();
			return false;
		}
		return true;
	},
	
	checkPstvNumber:function(name,msg)
	{
		var obj = $('#' + name);
		var bRet = Validation.isPositiveInteger(obj.val());
		if(!bRet)
		{
			myAlert(null, msg + ":输入非法!",250,0);
			obj.focus();
			return false;
		}
		return true;
	},
	
	checkIp:function(name,msg)
	{
		var obj = $('#' + name);
		var bRet = Validation.isIp(obj.val());
		if(!bRet)
		{
			myAlert(null, msg + ":非法!",250,0);
			obj.focus();
			return false;
		}
		return true;
	},
	
	checkField:function(name,msg)
	{
		var obj = $('#' + name);
		var bRet = Validation.isFiledString(obj.val());
		
		if(!bRet)
		{
			myAlert(null, msg + ":非法!",250,0);
			obj.focus();
			return false;
		}
		return true;
	},
	
	clearSpace:function(str)
	{
		return str.replace(/[ ]/g,'');
	}
};

 /**
 * JErrorHandler
 * it display error if (@debugging__=true && @logging__=true), 
 * 
 */
//Event.KEY_UP = 38;
//Event.KEY_DOWN = 40;

var debugging__ = false;

/*bind a function to onload Event handler*/
function addLoadEvent(func){
	var oldonload=window.onload;
	if(typeof window.onload!='function'){
		window.onload=func;
	}
	else{
		window.onload=function(){
			oldonload();
			func();
		}
	}
}

/************************************************************************************************************************/
window.onerror = function ignoreError(msg,url,line) {
	if(debugging__){
		alert('App Error Info:'
		  + '\n\n'
		  + '\n\n------------------------------------------------------'
		  + '\nText:' + msg + '\nurl:' + url + '\nline:' + line
		  + '\n------------------------------------------------------');
		return false;
	}
	else return true;
};

document.onkeydown = function (event){
	evt = event || window.event;
	if(evt.ctrlKey && evt.shiftLeft && evt.keyCode === 38){
		if(logging__){
				JLog.clear();
				debugging__ = false;
				logging__ = false;
				$('#logCtrl').hide();
				$('#log').hide();
		}
				
	}

	if(evt.ctrlKey && evt.shiftLeft && evt.keyCode === 40){
			debugging__ = true;
			logging__ = true;
			printf("::::: ReStart Debugging Output Window :::::");
			$('#logCtrl').show();
			$('#log').show();
	}

	if(evt.altKey && evt.keyCode === 40)
	{
		debugging__ = true;
		logging__  = true;
	}

	if(evt.altKey && evt.keyCode === 38)
	{
		debugging__ = false;
		logging__  = false;
	}
};

/**
 *An implementation of the debug log.
 */
var logging__ = false;
var loggingCount = 0;

function JLog() {};

JLog.lines = [];

JLog.write = function(s) {
  if (logging__) {
	s = "[" +loggingCount++ +"]" +s;
    this.lines.push(xmlEscapeText(s));
    this.show();
  }
};

// Writes the given XML with every tag on a new line.
JLog.writeXML = function(xml) {
  if (logging__) {
    var s0 = xml.replace(/</g, '\n<');
    var s1 = xmlEscapeText(s0);
    var s2 = s1.replace(/\s*\n(\s|\n)*/g, '<br/>');
    this.lines.push(s2);
    this.show();
  }
};

// Writes without any escaping
JLog.writeRaw = function(s) {
  if (logging__) {
    this.lines.push(s);
    this.show();
  }
};

JLog.clear = function() {
  if (logging__) {
    var l = this.div();
    l.innerHTML = '';
    this.lines = [];
  }
};

JLog.show = function() {
  var l = this.div();
  l.innerHTML += this.lines.join('<br/>') + '<br/>';
  this.lines = [];
  l.scrollTop = l.scrollHeight;
};

JLog.div = function() {
  var l = document.getElementById('log');
  if (!l) {
    l = document.createElement('div');
	l.setAttribute("id","log");
	l.setAttribute("title","lqp debug output");
	// fix me: the follow line ie not support ??
	//l.setAttribute("style"," position:absolute;right:5px;top:20px;width:350px;height:350;overflow:auto;"
	// + " background-color:#f0f0f0;border:1px solid gray; font-Size:12px;padding:5px; ");
	//l.style.position = "absolute";
	l.style.position ="absolute";
	l.style.width = "350px";
	l.style.height = "350px";
	l.style.right = "5px";
	l.style.top =" 20px";
	l.style.overflow = "auto";
	l.style.backgroundColor = "#f0f0f0";
	l.style.border = "1px solid gray";
	l.style.fontSize = "12px";
	l.style.padding = "5px";
	l.style.textAlign = "left";
	l.style.wordWrap  = "break-word"; //fixed

	l.innerHTML = "<div style=\"font-weight: bold;text-align:center\" >"
					+ "::::::: Debugging Output Window ::::::: </div>";

	logCtrl = document.createElement('div');
	logCtrl.setAttribute("id","logCtrl");
	logCtrl.setAttribute("title","Click me to hide/show debug window!");
	//fix me : the follow line ie not support 
	//logCtrl.setAttribute("style","position:absolute;right:5px;top:0px;width:60px;height:20px;font-Size:12px;");
	logCtrl.style.position = "absolute";
	logCtrl.style.right = "5px";
	logCtrl.style.top = "0px";
	logCtrl.style.width = "60px";
	logCtrl.style.height ="20px";
	logCtrl.style.fontSize ="12px";
	logCtrl.onclick = function(){
		if(l.style.display == "none")
		{
			l.style.display= "block";
		}
		else
		{
			l.style.display = "none";
		}
	};

	logCtrl.innerHTML = 'Hide/Show';

	if(document.body)
		document.body.appendChild(logCtrl);
		document.body.appendChild(l);
  }
  return l;
};


/**
 * equal to JLog.write(msg)
 */
function printf(msg){JLog.write(msg+"\n");}

/**
 * an implementation of timer
 */
function JTimer(iInterval){
	this.Interval = iInterval || 500;
	this._handleInterval;
	this.TimerEvent = null;
};

JTimer.prototype.TimerCallBack = function (){
		JLog.write('+Timer.TimerCallBack()');
		if (typeof this.TimerEvent=="function")
			this.TimerEvent();
		else if(this.TimerEvent!=null && this.TimerEvent.length>0)
			eval(this.TimerEvent);
		JLog.write('-Timer.TimerCallBack()');
};

JTimer.prototype.Start = function(){
		JLog.write('+Timer.Start()');
		if(this.Interval!=0){
			func=this.TimerEvent;
			this._handleInterval=setInterval(func,this.Interval);
		}
		JLog.write('-Timer.Start()');
};

JTimer.prototype.Stop = function(){
		JLog.write('+Timer.Stop()');
		clearInterval(this._handleInterval);
		JLog.write('-Timer Stop()');
};



/**
 *Split a string s at all occurrences of character c. This is like
 *the split() method of the string object, but IE omits empty
 *strings, which violates the invariant (s.split(x).join(x) == s).
 */
function stringSplit(s, c) {
  var a = s.indexOf(c);
  if (a == -1) {
    return [ s ];
  }
  
  var parts = [];
  parts.push(s.substr(0,a));
  while (a != -1) {
    var a1 = s.indexOf(c, a + 1);
    if (a1 != -1) {
      parts.push(s.substr(a + 1, a1 - a - 1));
    } else {
      parts.push(s.substr(a + 1));
    } 
    a = a1;
  }

  return parts;
}

/**
 *Reverses the given array in place.
 */
function reverseInplace(array) {
  for (var i = 0; i < array.length / 2; ++i) {
    var h = array[i];
    var ii = array.length - i - 1;
    array[i] = array[ii];
    array[ii] = h;
  }
}

/**
 *Shallow-copies an array.
 */
function copyArray(dst, src) { 
  for (var i = 0; i < src.length; ++i) {
    dst.push(src[i]);
  }
}

/**
 *Escape XML special markup chracters: tag delimiter < > and entity
 *reference start delimiter &. The escaped string can be used in XML
 *text portions (i.e. between tags).
 */
function xmlEscapeText(s) {
  return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
}


/**
 * if catche error ,return detail for dispaly
 * try{..}catch(e){..}
 */
function getErrorDescription(e){
	return e + e.description;
}

/**
 * Applies the given function to each element of the array.
 */
function mapExec(array, func) {
  for (var i = 0; i < array.length; ++i) {
    func(array[i]);
  }
}

/**
 *Returns an array that contains the return value of the given
 *function applied to every element of the input array.
 */
function mapExpr(array, func) {
  var ret = [];
  for (var i = 0; i < array.length; ++i) {
    ret.push(func(array[i]));
  }
  return ret;
};


//[delete] message function by zhangzuliang 2006-11-8 
//[delete] JEvent() function by zhangzuliang 2006-11-8


/**
 * JMessage class
 *
 * Show the message on the div which id by jlib_message  
 * 
 * if the jlib_message undefine 
 * the message will show on the ie left 
 * 
 * if have the jlib_message
 * the message will show on this div
 *
 * Copyright 1995-2005 KEDACOM (http://www.kedacom.com)
 * 
 * @author kongchun,zhangzuliang
 * @create 2005-11-24
 * last update 2006-11-9 zhangzuliang
 * @version $Revision v1.0 
 * @package JLib
 */
var JMessage = {
	message : "",
	timer:"",
	showMessage : function(message) {
			window.clearTimeout(this.timer);
			this.message = message ? message : this.message;
			var scrollY = 0;
			try{
				if (window.top.document.all)
					scrollY = window.top.document.body.scrollTop;
				else
					scrollY = window.top.pageYOffset;
			} catch(e) {}
				var div = window.top.document.getElementById("jlib_message");
				if (div == null) {
					div = window.top.document.createElement("div");
					div.id = "jlib_message";
					div.style.zIndex = "1000";
					div.style.position = "absolute";
					div.style.padding = "2px 8px"; //fix kongchun
					div.style.color = "white";
					div.style.backgroundColor = "#cc4444";
					//div.style.width = auto;
					div.style.heigth = "30px";
					div.style.top = "5px"; // fix me: cannot get top margin value zzl 2005-9-23
					div.style.right = "5px";
					div.style.fontFamily = "Arial,";
					div.style.fontWeight = "bold"; 
					div.style.fontSize = "12px";
					window.top.document.body.appendChild(div);
				}
				div.innerHTML = this.message;
				div.style.top = scrollY + 5;
				div.style.display = "";
	},
	
	hideMessage : function() {
			var div = window.top.document.getElementById("jlib_message");
			if (div != null){
				div.style.display = "none";
			}
	},
	showDelayHideMessage : function(message,delay){
		window.clearTimeout(this.timer);
		this.showMessage(message);
		var _delay = typeof(delay) ? delay:3000;
		var div = window.top.document.getElementById("jlib_message");
		if (div != null){
			this.timer = window.setTimeout(function(){div.style.display = "none";},_delay);
		}
	}
};

function GetQueryValue(sorStr,panStr)

{
	
	var vStr="";
	
	if(sorStr==null||sorStr==""||panStr==null||panStr=="")return vStr;
	
	sorStr=sorStr.toLowerCase();
	
	panStr+="=";
	
	var itmp=sorStr.indexOf(panStr);
	
	if(itmp<0)
	{
		return vStr;
		
	}
	sorStr=sorStr.substr(itmp+panStr.length);
	
	itmp=sorStr.indexOf("&");
	
	if(itmp<0)
	
	{
		
		return sorStr;
		
		
	}
	else 
	
	{
		
		sorStr=sorStr.substr(0,itmp);
		
		return sorStr;
		
		
	}
	
}

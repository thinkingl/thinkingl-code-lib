var Validation = {

	//是否数字
	isNumber:function(value){
		return typeof(value) == "number" ? true : false;
	},

	//是否字符串
	isString:function(value){
		return typeof(value) == "string" ? true : false;
	},

	//是否布尔型
	isBoolean:function(value){
		return typeof(value) == "boolean" ? true : false;
	},

	//是否对象
	isObject:function(value){
		return typeof(value) == "object" ? true : false;
	},

	//是否为真
	isTrue:function(value){
		if(this.isString(value)){
			return true;
		}
		var tmp = new String(value).toLowerCase().trim();
		return (tmp == "true" || tmp == "1") ? true : false;
	},

	//不为空 null 以及 undefined
	isNotNull:function(value){
		if(typeof(value) == "undefined"){
			return false;
		}
		if(value == null){
			return false;
		}
		if(value == ""){
			return false;
		}
		return true;
	},

	//是否是自定义的正则表达式
	isRegExp:function(reg,value){
		if(this.isString(value)){
			reg = new RegExp(reg);
		}
		if(!reg.test(value)){
			return false;
		}
		return true;
	},

	//是否为任意数字
	isNumeric:function(value){
		var reg = /^(-|\+)?\d+(\.\d+)?$/  
		return this.isRegExp(reg,value);
	},

	//检查是否为正数
	isUnsignedNumeric:function(value){
		var reg = /^\d+(\.\d+)?$/ 
		return this.isRegExp(reg,value);
	},

	//是否为整数
	isInteger:function(value){
		var reg = /^(-|\+)?\d+$/ 
		return this.isRegExp(reg,value);
	},

	//是否为非负整数
	isUnsignedInteger:function(value){
		var reg = /^\d+$/;
		return this.isRegExp(reg,value);
	},
	
	//是否为正整数
	isPositiveInteger:function(value){
		var reg = /^[0-9]*[1-9][0-9]*$/;
		return this.isRegExp(reg,value);
	},
	
	//是否是仅数字字母下滑线
	isFiledEn:function(value){
		var reg = /^\w+$/;
		return this.isRegExp(reg,value);
	},
	
	//是否仅中文
	isFiledCN:function(value){
		var reg = /^([\u4e00-\u9fa5])+$/;
		return this.isRegExp(reg,value);
	},
	
	//是否是中文数字字母下滑线
	isFiledString:function(value){
		var reg = /^(\w|[\u4e00-\u9fa5]|-)+$/;
		return this.isRegExp(reg,value);
	},
	
	//是否是中文数字字母下滑线点号
	isCodeString:function(value){
		var reg = /^(\w|[\u4e00-\u9fa5]|-|\.)+$/;
		return this.isRegExp(reg,value);
	},

	//是否在指定的范围
	isRange:function(max,min,value){
		if(value<min || value>max){
			return false;
		}else{
			return true;
		}
	},
	
	//是否为正浮点数
	isUnsignedFloat:function(value){
	  var reg= /^\d+(\.\d*)?$/;
	  return this.isRegExp(reg,value);
	},
	
	isIp:function(value){
		var reg= /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5]).(\d{1,2}|1\d\d|2[0-4]\d|25[0-5]).(\d{1,2}|1\d\d|2[0-4]\d|25[0-5]).(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/;
		return this.isRegExp(reg,value);
	}

}

//-------------------------------------------
function $Reg(Obj,reg){
	var value = $F(Obj).trim();
	return Validation.isRegExp(reg,value);
}
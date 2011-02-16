// convert from DOM Element to JSON 
// rewrite this class to static class style
// usage:
//		var json = ParseXML.parseElement(xmldocument);
// last update:2006-9-11 zhangzuliang (aliang.cn@gmail.com)

/*有效的数值符合以下的型别：
1-ELEMENT
2-ATTRIBUTE
3-TEXT
4-CDATA
5-ENTITY REFERENCE
6-ENTITY
7-PI (processing instruction)
8-COMMENT
9-DOCUMENT
10-DOCUMENT TYPE
11-DOCUMENT FRAGMENT
12-NOTATION*/

var JXmlParser= {

	parseElement : function ( elem ) {
		//printf('elem.nodeType is '+elem.nodeType);
		//  COMMENT_NODE
		if ( elem.nodeType == 7 ) {
			return;
		}

		//  TEXT_NODE CDATA_SECTION_NODE
		if ( elem.nodeType == 3 || elem.nodeType == 4 ) {
			var bool = elem.nodeValue.match( /[^\u0000-\u0040]/ );
			if ( bool == null ) return;     // ignore white spaces
			return elem.nodeValue;
		}

		var retval;
		var cnt = {};

		//  parse attributes
		if ( elem.attributes && elem.attributes.length ) {
			retval = {};
			for ( var i=0; i<elem.attributes.length; i++ ) {
				var key = elem.attributes[i].nodeName;
				if ( typeof(key) != "string" ) continue;
				var val = elem.attributes[i].nodeValue;
				if ( ! val ) continue;
				if ( typeof(cnt[key]) == "undefined" ) cnt[key] = 0;
				cnt[key] ++;
				this.addNode( retval, key, cnt[key], val );
			}
		}

		//  parse child nodes (recursive)
		if ( elem.childNodes && elem.childNodes.length ) {
			var textonly = true;
			if ( retval ) textonly = false;        // some attributes exists
			for ( var i=0; i<elem.childNodes.length && textonly; i++ ) {
				var ntype = elem.childNodes[i].nodeType;
				if ( ntype == 3 || ntype == 4 ) continue;
				textonly = false;
			}
			if ( textonly ) {
				if ( ! retval ) retval = "";
				for ( var i=0; i<elem.childNodes.length; i++ ) {
					retval += elem.childNodes[i].nodeValue;
				}
			} else {
				if ( ! retval ) retval = {};
				for ( var i=0; i<elem.childNodes.length; i++ ) {
					var key = elem.childNodes[i].nodeName;
					if ( typeof(key) != "string" ) continue;
					var val = this.parseElement( elem.childNodes[i] );
					if ( ! val ) continue;
					if ( typeof(cnt[key]) == "undefined" ) cnt[key] = 0;
					cnt[key] ++;
					this.addNode( retval, key, cnt[key], val );
				}
			}
		}
		return retval;
	},

	addNode : function ( hash, key, cnts, val ) {
		if ( cnts == 1 ) {                      // 1st sibling
			hash[key] = val;
		} else if ( cnts == 2 ) {               // 2nd sibling
			hash[key] = [ hash[key], val ];
		} else {                                // 3rd sibling and more
			hash[key][hash[key].length] = val;
		}

	}

};
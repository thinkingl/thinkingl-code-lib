#pragma once

/** 
*	a simple xml DOM parser ..
*	Created by thinkingl @ 2011/2/17
*	thinkingzx@gmail.com
*/

#include <string>
#include <map>

using namespace std;

class CXMLDom
{
public:
	CXMLDom();
	~CXMLDom();

	/** parse the xml in string .
	*	Build the XML DOM Tree.
	*/
	bool ParseString( char * pStr );

	/** Find Element. Create a new one if not exist. */
	CXMLDom& operator []( char *pName );
//	const CXMLDom& operator[]( char *pName ) const;C

	/** Get and Set the value of the xml element. */
	const string Value()const;
	void Value( char *pValue );

	void Clear();

	CXMLDom *Parent();
	void Parent( CXMLDom * pParent);

	/** Trans to string . */
	const string ToString() const;

private:

	/** Table */
	typedef std::map< string, CXMLDom > TXMLDomTreeTable;
	TXMLDomTreeTable m_xmlDomTree;

	/** value. */
	string m_strValue;

	/** parent. */
	CXMLDom *m_pParentXMLDOM;
};


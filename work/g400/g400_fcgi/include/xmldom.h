#pragma once

/** 
*	a simple xml DOM parser ..
*	Created by thinkingl @ 2011/2/17
*	thinkingzx@gmail.com
*/

#include <string>
#include <map>
#include "portabledefine.h"

using namespace std;

class CXMLDom
{
public:
	CXMLDom();
	~CXMLDom();

	CXMLDom( const CXMLDom& another );

	/** parse the xml in string .
	*	Build the XML DOM Tree. 
	*/
	bool ParseString( const char * pStrXML );

	bool ParseFile( const char* pStrFilePath );

	/** Find Element. Create a new one if not exist. */
	CXMLDom& operator []( const char *pName );
	CXMLDom& operator []( ctstring& strName );
//	const CXMLDom& operator[]( char *pName ) const;C

	CXMLDom& operator =( const CXMLDom& another );

	/** Get and Set the value of the xml element. */
	const string Value()const;
	void Value( const char *pValue );
	void Value( int nValue );

	void Clear();

	CXMLDom *Parent();
	const CXMLDom* Parent() const;
	void Parent( CXMLDom * pParent);

	/** Trans to string . */
	const string ToString() const;

	bool IsEmpty()const;

private:

	/** Table */
	typedef std::map< string, CXMLDom > TXMLDomTreeTable;
	TXMLDomTreeTable m_xmlDomTree;

	/** value. */
	string m_strValue;

	/** parent. */
	CXMLDom *m_pParentXMLDOM;
};


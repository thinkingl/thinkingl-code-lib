
#include "xmldom.h"

#include<stdio.h>
#include<stdlib.h>

#include<string.h>
#include <sstream>
#include <libxml/SAX2.h>
#include<libxml/parser.h>
#include<libxml/parserInternals.h>

#include "fcgi_stdio.h"

#ifndef CHAR
#define CHAR xmlChar
#endif



static void start_document(void *ctx);
static void end_document(void *ctx);
static void start_element(void *ctx,const CHAR*name,const CHAR**attrs);
static void end_element(void *ctx,const CHAR*name);
static void chars_found(void *ctx,const CHAR*chars,int len);

static CXMLDom* s_pXmlDomTmp = NULL;

static void start_document(void *ctx)
{
 //printf("Document start ud: %x \n", xmlParserCtxtPtr(ctx)->userData );

	if( s_pXmlDomTmp )
	{
		s_pXmlDomTmp->Clear();
	}
}

static void end_document(void *ctx)
{
//	printf("Document end\n");
	s_pXmlDomTmp = NULL;
}

static void start_element(void *ctx,const CHAR*name,const CHAR **attrs)
{
const CHAR *attr_ptr;
int curr_attr=0;
//printf("Element %s started \n",name );

	if( s_pXmlDomTmp )
	{
		CXMLDom& childXmlDom = (*s_pXmlDomTmp)[(char*)name];
		childXmlDom.Parent( s_pXmlDomTmp );
		s_pXmlDomTmp = &childXmlDom;
	}

if(attrs)
{
attr_ptr=*attrs;
while(attr_ptr){
//printf("\t Attributes %s \n",attr_ptr);
curr_attr++;
attr_ptr=*(attrs+curr_attr);
}//while
}//if
}//start_element


static void end_element(void *ctx,const CHAR*name)
{
//printf("Element %s ended\n",name);

	if( s_pXmlDomTmp )
	{
		s_pXmlDomTmp = s_pXmlDomTmp->Parent();
	}

}

bool IsIgnorableChar( char ch )
{
	return ( ch == ' ' || ch == '\n' || ch == '\r' );
}

#define CHAR_BUFFER 10240
static void chars_found(void *ctx,const CHAR*chars,int len)
{
	char szBuff[CHAR_BUFFER+1] = {0};

	int nBegin = 0;
	int nEnd = len ;

	for( ; nBegin<len; ++nBegin )
	{
		if ( !IsIgnorableChar( chars[nBegin] ) )
		{
			break;
		}
	}

	for( ; nEnd > nBegin; --nEnd )
	{
		if( !IsIgnorableChar( chars[nEnd-1] ) )
		{
			break;
		}
	}

//	printf( "begin: %d end: %d len: %d \n", nBegin, nEnd, len  );
	int newLen = min( CHAR_BUFFER, nEnd - nBegin );
	strncpy( szBuff, (char*)(&chars[nBegin]), newLen);


//printf( "Found %d characters: %s \n", strlen(szBuff), szBuff);

	if( s_pXmlDomTmp )
	{
		s_pXmlDomTmp->Value( szBuff );
	}

}

bool parseXML( const char* pStrXML )
{
	xmlParserCtxtPtr ctxt_ptr;

	static xmlSAXHandler mySAXParseCallbacks;
	memset(&mySAXParseCallbacks, 0,sizeof(mySAXParseCallbacks));
	mySAXParseCallbacks.startDocument=start_document;
	mySAXParseCallbacks.endDocument=end_document;
	mySAXParseCallbacks.startElement=start_element;
	mySAXParseCallbacks.endElement=end_element;
	mySAXParseCallbacks.characters=chars_found;


	ctxt_ptr=xmlCreateMemoryParserCtxt( pStrXML, strlen( pStrXML ) );
	if(!ctxt_ptr)
	{
		fprintf( stderr, "Failed to create file parser\n");
		return false;
	}

	ctxt_ptr->sax=&mySAXParseCallbacks;

	xmlParseDocument(ctxt_ptr);
//	printf("XML version %s, encoding %s \n",ctxt_ptr->version,ctxt_ptr->encoding);
	if(!ctxt_ptr->wellFormed)
	{
		fprintf( stderr, "Document not well formed doc:\n %s \n", pStrXML );
		return false;
	}

	ctxt_ptr->sax=NULL;

	xmlFreeParserCtxt(ctxt_ptr);
	return true;
//	printf("Parsing complete\n");

}

CXMLDom::CXMLDom()
{
	m_pParentXMLDOM = NULL;
}

CXMLDom::~CXMLDom()
{
}

bool CXMLDom::ParseString( const char* pStrXML )
{
	s_pXmlDomTmp = this;

	bool bRet = parseXML( pStrXML );

//	printf( "value is %s\n", (*this)["KedacomXMLData"]["Content"]["ErrorMesssage"].Value().c_str() );

//	(*this)["KedacomXMLData"]["Content"]["ErrorMesssage"].Value( "modified! " );
//	printf( "to string: %s \n", this->ToString().c_str() );

	return bRet;
}

CXMLDom& CXMLDom::operator []( const char* pName )
{
	this->m_xmlDomTree[ pName ].Parent( this );
	return this->m_xmlDomTree[ pName ];
}

CXMLDom& CXMLDom::operator []( ctstring& strName )
{
	return this->operator[]( strName.c_str() );
}

const string CXMLDom::Value() const
{
	return this->m_strValue;
}

void CXMLDom::Value( const char *pValue )
{
	this->m_strValue = pValue;
}

const string CXMLDom::ToString() const
{
	stringstream ssXml;
	if( this->Parent() == NULL )
	{
		// no parent.. root need head..
		ssXml << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
	}

	TXMLDomTreeTable::const_iterator cIt = this->m_xmlDomTree.begin();
	while( cIt != this->m_xmlDomTree.end() )
	{
		ssXml << "<" << cIt->first << ">";
		ssXml << cIt->second.ToString();
		ssXml << "</" << cIt->first << ">";

		++cIt;
	}

	ssXml << this->Value();

	return ssXml.str();
}

void CXMLDom::Clear()
{
	this->m_xmlDomTree.clear();
	this->m_strValue.clear();
}

CXMLDom* CXMLDom::Parent()
{
	return this->m_pParentXMLDOM;
}

const CXMLDom* CXMLDom::Parent()const
{
	return this->m_pParentXMLDOM;
}

void CXMLDom::Parent( CXMLDom* pParent )
{
	this->m_pParentXMLDOM = pParent;
}

bool CXMLDom::IsEmpty()const
{
	if( NULL == this->m_pParentXMLDOM )
	{
		// root element must has child.
		return this->m_xmlDomTree.empty();
	}
	else
	{
		return ( this->m_strValue.empty() && this->m_xmlDomTree.empty() );
	}
}




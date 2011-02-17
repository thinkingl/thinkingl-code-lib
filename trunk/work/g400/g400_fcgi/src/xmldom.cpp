
#include "xmldom.h"

#include<stdio.h>
#include<stdlib.h>

#include<string.h>
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
 printf("Document start ud: %x \n", xmlParserCtxtPtr(ctx)->userData );

	if( s_pXmlDomTmp )
	{
		s_pXmlDomTmp->Clear();
	}
}

static void end_document(void *ctx)
{
	printf("Document end\n");
	s_pXmlDomTmp = NULL;
}

static void start_element(void *ctx,const CHAR*name,const CHAR **attrs)
{
const CHAR *attr_ptr;
int curr_attr=0;
printf("Element %s started \n",name );

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
printf("\t Attributes %s \n",attr_ptr);
curr_attr++;
attr_ptr=*(attrs+curr_attr);
}//while
}//if
}//start_element


static void end_element(void *ctx,const CHAR*name)
{
printf("Element %s ended\n",name);

	if( s_pXmlDomTmp )
	{
		s_pXmlDomTmp = s_pXmlDomTmp->Parent();
	}

}

#define CHAR_BUFFER 10240
static void chars_found(void *ctx,const CHAR*chars,int len)
{
	char szBuff[CHAR_BUFFER+1] = {0};
	int nCursor = 0;
	for( int i=0; i<min(len, CHAR_BUFFER); ++i )
	{
		if( nCursor == 0 )
		{
			if( chars[i] == ' ' || chars[i] == '\n' )
			{
				continue;
			}
		}
		szBuff[ nCursor++ ] = chars[i];
	}

	for( int i=nCursor; i>0; --i )
	{
		if( szBuff[i-1] == ' ' || szBuff[i-1] == '\n' )
		{
			szBuff[i-1] = 0;
		}
		else
		{
			break;
		}
	}


//if(len>CHAR_BUFFER)len=CHAR_BUFFER;
//strncpy(buff, (char*)chars,len);
//buff[len]='\0';

printf( "Found %d characters: %s \n", strlen(szBuff), szBuff);

	if( s_pXmlDomTmp )
	{
		s_pXmlDomTmp->Value( szBuff );
	}

}

void unittestXMLDOM( CXMLDom* pThis )
{
	xmlParserCtxtPtr ctxt_ptr;

	static xmlSAXHandler mySAXParseCallbacks;
	memset(&mySAXParseCallbacks,sizeof(mySAXParseCallbacks),0);
	mySAXParseCallbacks.startDocument=start_document;
	mySAXParseCallbacks.endDocument=end_document;
	mySAXParseCallbacks.startElement=start_element;
	mySAXParseCallbacks.endElement=end_element;
	mySAXParseCallbacks.characters=chars_found;

	ctxt_ptr=xmlCreateFileParserCtxt("/mnt/svn-work/g400/xml_msg_sample/login_ack.xml");
	if(!ctxt_ptr)
	{
	fprintf(stderr,"Failed to create file parser\n");

	}
	
	ctxt_ptr->sax=&mySAXParseCallbacks;
	ctxt_ptr->userData = pThis;

	xmlParseDocument(ctxt_ptr);
	printf("XML version %s, encoding %s \n",ctxt_ptr->version,ctxt_ptr->encoding);
	if(!ctxt_ptr->wellFormed)
	{fprintf(stderr,"Document not well formed\n");}

	ctxt_ptr->sax=NULL;

	xmlFreeParserCtxt(ctxt_ptr);
	printf("Parsing complete\n");

}

CXMLDom::CXMLDom()
{
	m_pParentXMLDOM = NULL;	
}

CXMLDom::~CXMLDom()
{
}

bool CXMLDom::ParseString( char* pStrXML )
{
	s_pXmlDomTmp = this;
	unittestXMLDOM( this );

	printf( "value is %s\n", (*this)["KedacomXMLData"]["Content"]["ErrorMesssage"].Value().c_str() );
	return false;
}

CXMLDom& CXMLDom::operator []( char* pName )
{
	return this->m_xmlDomTree[ pName ];
}

const string CXMLDom::Value() const
{
	return this->m_strValue;
}

void CXMLDom::Value( char *pValue )
{
	this->m_strValue = pValue;
}

const string CXMLDom::ToString() const
{
	return "";
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

void CXMLDom::Parent( CXMLDom* pParent )
{
	this->m_pParentXMLDOM = pParent;
}
	/** parse the xml in string .
	*	Build the XML DOM Tree.
	*/
//	BOOL ParseString( char * pStr );

	/** Find Element. Create a new one if not exist. */
//	CXMLDom& operator []( char *pName );
//	const CXMLDom& operator[]( char *pName ) const;C

	/** Get and Set the value of the xml element. */
//	const string Vaule()const;
//	void Value( char *pValue );

	/** Trans to string . */
//	const string ToString() const;
	


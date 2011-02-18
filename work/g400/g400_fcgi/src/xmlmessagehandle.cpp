#include "xmlmessagehandle.h"


#include "g400define.h"

CXMLMessageHandle *CXMLMessageHandle::s_pInstance = NULL;


CXMLMessageHandle *CXMLMessageHandle::GetInstance()
{
    if( s_pInstance == NULL )
    {
        s_pInstance = new CXMLMessageHandle();
    }
    return s_pInstance;
}

void CXMLMessageHandle::ReleaseInstance()
{
    if( NULL != s_pInstance )
    {
        delete s_pInstance;
        s_pInstance = NULL;
    }
}

bool CXMLMessageHandle::HandleXMLMessage( CXMLDom& inXMLMessage, CXMLDom& outXMLMessage )
{
    for( TXmlMsgObserverList::iterator iter = this->m_xmlMsgObserverList.begin();
        iter != this->m_xmlMsgObserverList.end(); ++iter )
        {
            (*iter)->HandleXMLMessage( inXMLMessage, outXMLMessage );
        }

    if( outXMLMessage.IsEmpty())
    {
    	this->OnEmptyOutXml( inXMLMessage, outXMLMessage );
    }
    return true;
}

void CXMLMessageHandle::RegObserver( IXMLMessageObserver* pObserver )
{
    this->m_xmlMsgObserverList.insert( pObserver );
}

void CXMLMessageHandle::UnregObserver( IXMLMessageObserver* pObserver )
{
    this->m_xmlMsgObserverList.erase( pObserver );
}

void CXMLMessageHandle::OnEmptyOutXml( CXMLDom& inXmlMsg, CXMLDom& outXMLMsg )
{
	outXMLMsg[ G400XML::ELE_HEAD ] = inXmlMsg[ G400XML::ELE_HEAD ];

	outXMLMsg[ G400XML::ELE_HEAD ][ G400XML::ELE_STATUS ].Value( G400Status::UNKNOWN_ERR );
	outXMLMsg[ G400XML::ELE_CONTENT][ G400XML::ELE_ERRMSG ].Value( "Unknown error!" );

}



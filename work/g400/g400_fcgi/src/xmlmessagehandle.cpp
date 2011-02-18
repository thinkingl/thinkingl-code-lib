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
    	outXMLMessage[ G400XML::ELE_ROOT ][ G400XML::ELE_HEAD ][ G400XML::ELE_STATUS ].Value( "0" );
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




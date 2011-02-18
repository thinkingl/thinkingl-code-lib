#pragma once

#include <set>
#include "xmlmessageobserver.h"
#include "xmldom.h"


class CXMLMessageHandle
{
public:
	static CXMLMessageHandle* GetInstance();
	static void ReleaseInstance();


	/** handle xml message. */
	bool HandleXMLMessage( CXMLDom&  inXMLMessage, CXMLDom & outXMLDom);

	void RegObserver( IXMLMessageObserver* pObserver );
	void UnregObserver( IXMLMessageObserver* pObserver );

private:
	/** Handle empty out.. */
	void OnEmptyOutXml( CXMLDom&  inXMLMessage, CXMLDom & outXMLDom );
private:
	static CXMLMessageHandle *s_pInstance;

	typedef std::set<IXMLMessageObserver*> TXmlMsgObserverList;
	TXmlMsgObserverList m_xmlMsgObserverList;
};



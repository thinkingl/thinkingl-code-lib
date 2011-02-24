
#pragma once

#include "xmldom.h"

class IXMLMessageObserver
{
public:

	/** handle xml message. */
	virtual bool HandleXMLMessage( CXMLDom&  inXMLMessage, CXMLDom & outXMLDom) = 0;
};



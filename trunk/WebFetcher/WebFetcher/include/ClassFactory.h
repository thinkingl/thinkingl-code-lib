#pragma once

#include "IHttpFecher.h"
#include "IApplication.h"

class CClassFactory
{
public:
	CClassFactory(void);
	virtual ~CClassFactory(void);

	static IHttpFecher *CreateHttpFecher();
	
	static IApplication *CreateApp();
};

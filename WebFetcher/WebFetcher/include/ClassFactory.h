#pragma once

#include "IHttpFecher.h"
#include "IApplication.h"
#include "IHtmlPageParser.h"
#include "IThread.h"

class CClassFactory
{
public:
	CClassFactory(void);
	virtual ~CClassFactory(void);

	static IHttpFecher *CreateHttpFecher();
	
	static IApplication *CreateApp();

	static IHtmlPageParser *CreateHtmlPageParser();

	static IThread *CreatePageFetchThread();
};

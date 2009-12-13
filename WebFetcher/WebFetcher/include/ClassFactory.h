#pragma once

#include "IHttpDownloader.h"
#include "IApplication.h"
#include "IHtmlPageParser.h"
#include "IThread.h"
#include "IPageFecher.h"
#include "IMutex.h"
#include "IWebpageManager.h"
#include "IDatabase.h"

class CClassFactory
{
public:
	CClassFactory(void);
	virtual ~CClassFactory(void);

	static IHttpDownloader *CreateHttpDownloader();
	
	static IApplication *CreateApp();

	static IHtmlPageParser *CreateHtmlPageParser();

	static IThread *CreatePageFetchThread();

	static IPageFecher *CreatePageFetcher();

	static IMutex *CreateMutex();

	static IDatabase *CreateDatabase();
};

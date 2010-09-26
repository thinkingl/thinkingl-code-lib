#pragma once

#include "IHttpDownloader.h"
#include "IThread.h"
#include "IMutex.h"


#if 0
#include "IApplication.h"
#include "IHtmlPageParser.h"
#include "IPageFecher.h"
#include "IWebpageManager.h"
#include "IDatabase.h"
#endif

class CThinkinglClassFactory
{
public:
	CThinkinglClassFactory(void);
	virtual ~CThinkinglClassFactory(void);

	static IHttpDownloader *CreateHttpDownloader();
	


	static IThread *CreatePageFetchThread();
	static IMutex *CreateMutex();

	

#if 0 
	static IApplication *CreateApp();

	static IHtmlPageParser *CreateHtmlPageParser();
	static IPageFecher *CreatePageFetcher();
	
	static IDatabase *CreateDatabase();
#endif
};

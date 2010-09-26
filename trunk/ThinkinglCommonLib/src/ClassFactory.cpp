#include "ClassFactory.h"
#include "WinInetHttpFecher.h"
#include "Win32Mutex.h"

#if 0
#include "Win32Application.h"
#include "HtmlPageParser.h"
#include "Win32PageFetchThread.h"
#include "PageFecher.h"
#include "SqliteDatabase.h"
#include "IConfig.h"

#endif
//CThinkinglClassFactory::CThinkinglClassFactory(void)
//{
//}
//
//CThinkinglClassFactory::~CThinkinglClassFactory(void)
//{
//}


#if 0
IHttpDownloader *CThinkinglClassFactory::CreateHttpDownloader()
{
	IHttpDownloader * pDownloader = new CWinInetHttpFecher();
    BOOL bUseProxy = IConfig::Instance()->IsUseProxy();
    tstring strProxyIp = IConfig::Instance()->GetProxyIp();
    uint16 nProxyPort = IConfig::Instance()->GetProxyPort();
    tstring strProxyUser = IConfig::Instance()->GetProxyUserName();
    tstring strproxyPassword = IConfig::Instance()->GetProxyPassword();

    BOOL bOk = pDownloader->SetHttpProxy( bUseProxy, strProxyIp.c_str(), nProxyPort, strProxyUser.c_str(), strproxyPassword.c_str() );
//    ASSERT( bOk );
    return pDownloader;
}

IApplication *CThinkinglClassFactory::CreateApp()
{
	return new CWin32Application();
}

IHtmlPageParser *CThinkinglClassFactory::CreateHtmlPageParser()
{
	return new CHtmlPageParser();
}

IThread *CThinkinglClassFactory::CreatePageFetchThread()
{
	return new CWin32PageFetchThread();
}

IPageFecher *CThinkinglClassFactory::CreatePageFetcher()
{
	return new CPageFecher();
}


IDatabase *CThinkinglClassFactory::CreateDatabase()
{
	return new CSqliteDatabase();
}

#endif

IMutex *CThinkinglClassFactory::CreateMutex()
{
	return new CWin32Mutex();
}
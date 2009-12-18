#include "ClassFactory.h"
#include "WinInetHttpFecher.h"
#include "Win32Application.h"
#include "HtmlPageParser.h"
#include "Win32PageFetchThread.h"
#include "PageFecher.h"
#include "Win32Mutex.h"
#include "SqliteDatabase.h"
#include "IConfig.h"
//CClassFactory::CClassFactory(void)
//{
//}
//
//CClassFactory::~CClassFactory(void)
//{
//}



IHttpDownloader *CClassFactory::CreateHttpDownloader()
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

IApplication *CClassFactory::CreateApp()
{
	return new CWin32Application();
}

IHtmlPageParser *CClassFactory::CreateHtmlPageParser()
{
	return new CHtmlPageParser();
}

IThread *CClassFactory::CreatePageFetchThread()
{
	return new CWin32PageFetchThread();
}

IPageFecher *CClassFactory::CreatePageFetcher()
{
	return new CPageFecher();
}

IMutex *CClassFactory::CreateMutex()
{
	return new CWin32Mutex();
}

IDatabase *CClassFactory::CreateDatabase()
{
	return new CSqliteDatabase();
}
#include "ClassFactory.h"
#include "WinInetHttpFecher.h"
#include "Win32Application.h"
#include "HtmlPageParser.h"
#include "Win32PageFetchThread.h"
#include "PageFecher.h"
#include "Win32Mutex.h"
#include "SqliteDatabase.h"
//CClassFactory::CClassFactory(void)
//{
//}
//
//CClassFactory::~CClassFactory(void)
//{
//}



IHttpDownloader *CClassFactory::CreateHttpDownloader()
{
	return new CWinInetHttpFecher();
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
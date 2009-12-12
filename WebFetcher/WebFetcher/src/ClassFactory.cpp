#include "ClassFactory.h"
#include "WinInetHttpFecher.h"
#include "Win32Application.h"
#include "HtmlPageParser.h"
#include "PageFetchThread.h"
//CClassFactory::CClassFactory(void)
//{
//}
//
//CClassFactory::~CClassFactory(void)
//{
//}



IHttpFecher *CClassFactory::CreateHttpFecher()
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
	return new CPageFetchThread();
}
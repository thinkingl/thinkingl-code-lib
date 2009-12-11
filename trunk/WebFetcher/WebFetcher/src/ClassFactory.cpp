#include "ClassFactory.h"
#include "WinInetHttpFecher.h"
#include "Win32Application.h"
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
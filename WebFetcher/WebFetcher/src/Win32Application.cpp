#include "Win32Application.h"

CWin32Application::CWin32Application(void)
{
}

CWin32Application::~CWin32Application(void)
{
}

LPCTSTR CMD_EXIT = _T( "exit" );
LPCTSTR CMD_TEST_FETCH = _T( "testfetch" );

CWinApp theApp;

BOOL testFetchUrl()
{
	IHttpFecher *pFecher = CClassFactory::CreateHttpFecher();

	LPCTSTR strUrl = _T( "http://www.verycd.com/" );
	LPCTSTR strFile = _T( "F:\\download\\verycdindex.htm" );
	BOOL bRet = pFecher->FecheFile( strUrl, strFile );
	return bRet;
}

int CWin32Application::RunWebFetch()
{
	int nRetCode = 0;
	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		tstring strCmd;

		while( strCmd != CMD_EXIT )
		{
			tcin >> strCmd;

			if ( strCmd == CMD_TEST_FETCH )
			{
				testFetchUrl();
			}
		}
	}

	return nRetCode;
}
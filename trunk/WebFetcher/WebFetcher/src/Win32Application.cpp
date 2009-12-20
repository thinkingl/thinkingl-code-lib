#include "Win32Application.h"
#include "Log.h"
#include "IConfig.h"
#include "MimeType.h"

CWin32Application::CWin32Application(void)
{
}

CWin32Application::~CWin32Application(void)
{
}

LPCTSTR CMD_EXIT = _T( "exit" );
LPCTSTR CMD_TEST_FETCH = _T( "testfetch" );
LPCTSTR CMD_TEST_PAGE = _T( "testpage" );
LPCTSTR CMD_TEST_THREAD = _T( "testthread" );
LPCTSTR CMD_TEST_CFG = _T( "testcfg" );

CWinApp theApp;

BOOL testFetchUrl()
{
	IHttpDownloader *pFecher = CClassFactory::CreateHttpDownloader();

	LPCTSTR strUrl = _T( "http://www.verycd.com/" );
	LPCTSTR strFile = _T( "F:\\download\\verycdindex.htm" );
	BOOL bRet = pFecher->OpenUrl( strUrl );

	CMimeType eType;
	pFecher->GetMimeType( eType );

	pFecher->DownloadFile( strFile );

	delete pFecher;

	return bRet;
}

BOOL testParsePage()
{
	IHtmlPageParser *pParser = CClassFactory::CreateHtmlPageParser();

	LPCTSTR strPath = _T( "F:\\download\\verycdindex.htm" );
	LPCTSTR strUrl = _T( "http://www.verycd.com/" );

	BOOL bResult = pParser->Parse( strPath, strUrl );

	IHtmlPageParser::TUrlList tlist;
	bResult &= pParser->GetAllUrl( tlist );

	Log() << _T( "parse page url dump: " ) << endl;
	for ( size_t i=0; i<tlist.size(); ++i )
	{
		tcout << tlist[i] << endl;
	}
	Log() << endl << endl;

	LPCTSTR lpStrSrc = _T( "http://www.verycd.com/topics/2787816/" );
//	lpStrSrc = _T( "http://www.verycd.com/specs/opensearch.xml" );
	LPCTSTR lpStrDst = _T( "http://www.g.cn" );
	pParser->ReplaceAllUrl( lpStrSrc, lpStrDst );

	lpStrSrc = _T( "http://www.verycd.com/sto/movie/" );
	lpStrDst = _T( "/abc/d.html" );

	pParser->ReplaceAllUrl( lpStrSrc, lpStrDst );

	pParser->SaveFile( _T( "F:\\download\\replaced.htm" ) );
	

	delete pParser;

	return bResult;
}

BOOL testThread()
{
	IThread *pThread = CClassFactory::CreatePageFetchThread();
	pThread->Start();

	Sleep( 1000 );
	BOOL bRun = pThread->IsRunning();
	ASSERT( TRUE == bRun );

	Sleep( 10000 );

	pThread->Stop();

	bRun = pThread->IsRunning();
	ASSERT( !bRun );

	pThread->Start();
	Sleep( 100 );
	pThread->SetTimeOut( 100 );
	pThread->Stop();

	delete pThread;

	return TRUE;

}

BOOL testCfg()
{
	IConfig *pCfg = IConfig::Instance();
	pCfg->GetRootFolder();
	pCfg->GetRootUrl();

	return TRUE;
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
		// 使cout能够输出中文。
//		cout << "中文" << endl;

		wcout.imbue(locale(locale(),"",LC_CTYPE)); 
		// TODO: 在此处为应用程序的行为编写代码。

		Log() << _T( "暂时只使用单线程抓页面。 " ) << endl;

       
		
		int32 nThreadNum = IConfig::Instance()->GetThreadCount();
		nThreadNum = max( 1, nThreadNum );

		// test
		nThreadNum = 0;
		testParsePage();

       	std::vector< IThread * > tThreadList;
		for( int i=0; i<nThreadNum; ++i )
		{
			IThread *pThread = CClassFactory::CreatePageFetchThread();
			pThread->Start();
			tThreadList.push_back( pThread );
		}
		


		tstring strCmd;

		while( strCmd != CMD_EXIT )
		{
			tcin >> strCmd;

			if ( strCmd == CMD_TEST_FETCH )
			{
				testFetchUrl();
			}
			else if( strCmd == CMD_TEST_PAGE )
			{
				testParsePage();
			}
			else if( strCmd == CMD_TEST_THREAD )
			{
				testThread();
			}
			else if( strCmd == CMD_TEST_CFG )
			{
				testCfg();
			}
		}

		for ( size_t i=0; i<tThreadList.size(); ++i )
		{
			tThreadList[i]->Stop();
			delete tThreadList[i];
			tThreadList[i] = NULL;
		}
        IConfig::Release();
        IWebpageManager::Release();
	}

	return nRetCode;
}
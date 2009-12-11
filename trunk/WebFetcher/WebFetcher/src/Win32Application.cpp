#include "Win32Application.h"
#include "Log.h"

CWin32Application::CWin32Application(void)
{
}

CWin32Application::~CWin32Application(void)
{
}

LPCTSTR CMD_EXIT = _T( "exit" );
LPCTSTR CMD_TEST_FETCH = _T( "testfetch" );
LPCTSTR CMD_TEST_PAGE = _T( "testpage" );

CWinApp theApp;

BOOL testFetchUrl()
{
	IHttpFecher *pFecher = CClassFactory::CreateHttpFecher();

	LPCTSTR strUrl = _T( "http://www.verycd.com/" );
	LPCTSTR strFile = _T( "F:\\download\\verycdindex.htm" );
	BOOL bRet = pFecher->FecheFile( strUrl, strFile );
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

	CLog() << _T( "parse page url dump: " ) << endl;
	for ( size_t i=0; i<tlist.size(); ++i )
	{
		tcout << tlist[i] << endl;
	}
	CLog() << endl << endl;

	LPCTSTR lpStrSrc = _T( "http://www.verycd.com/topics/2786155/" );
//	lpStrSrc = _T( "http://www.verycd.com/specs/opensearch.xml" );
	LPCTSTR lpStrDst = _T( "http://www.g.cn" );
	pParser->ReplaceAllUrl( lpStrSrc, lpStrDst );

	pParser->SaveFile( _T( "F:\\download\\replaced.htm" ) );



	return bResult;
}

int CWin32Application::RunWebFetch()
{
	int nRetCode = 0;
	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// ʹcout�ܹ�������ġ�
//		cout << "����" << endl;

		wcout.imbue(locale(locale(),"",LC_CTYPE)); 
		wcout << L"2����222" << endl;
		

//		wcout.imbue(locale(locale(),"",LC_CTYPE)); 

//		cout << "����2" << endl;
		wcout << L"����2" << endl;

		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
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
		}
	}

	return nRetCode;
}
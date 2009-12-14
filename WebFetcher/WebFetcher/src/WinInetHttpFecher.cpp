#include "WinInetHttpFecher.h"

CWinInetHttpFecher::CWinInetHttpFecher(void)
{
}

CWinInetHttpFecher::~CWinInetHttpFecher(void)
{
}

BOOL CWinInetHttpFecher::SetHttpProxy( BOOL bUseProxy, LPCTSTR strIp, uint16 nPort, LPCTSTR strUser, LPCTSTR strPassword )
{
    BOOL bResult = FALSE;
    try
    {
        tstringstream ssProxy;
        ssProxy << strIp << _T( ":" ) << nPort;

        INTERNET_PROXY_INFO proxyInfo;
        proxyInfo.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
        proxyInfo.lpszProxy = ssProxy.str().c_str();

        bResult = this->m_wininetSession.SetOption( INTERNET_OPTION_PROXY, &proxyInfo, sizeof( proxyInfo ) );

        bResult &= this->m_wininetSession.SetOption( INTERNET_OPTION_PROXY_USERNAME, (LPVOID)strUser, _tcslen( strUser ) );

        bResult &= this->m_wininetSession.SetOption( INTERNET_OPTION_PROXY_PASSWORD, (LPVOID)strPassword, _tcslen( strPassword ) );

    }
    catch (CInternetException* pEx)
    {
        TCHAR sz[1024];
        pEx->GetErrorMessage(sz, 1024);
        _tprintf_s(_T("ERROR!  %s\n"), sz);
        pEx->Delete();

        bResult = FALSE;
    }
    return bResult;
}

BOOL CWinInetHttpFecher::DownloadFile( LPCTSTR strUrl, LPCTSTR strLocFilePath )
{
	BOOL bResult = FALSE;
	try
	{
		CStdioFile *pFile = this->m_wininetSession.OpenURL( strUrl );
		if ( pFile )
		{
			ofstream of;
			of.open( strLocFilePath );
			if( !of )
			{
				tcout << _T( "open file fail! path: " ) << strLocFilePath << endl;
			}
			const int nReadLen = 1000;
			char buff[ nReadLen ];
			while( 1 )
			{
				uint32 uLen = pFile->Read( buff, nReadLen );
				if ( uLen > 0 )
				{
					of.write( buff, uLen );
				}
				else
				{
					of.close();
					break;
				}
			}
			
		}
	}
	catch( CInternetException* pEx )
	{
		TCHAR sz[1024];
		pEx->GetErrorMessage(sz, 1024);
		_tprintf_s(_T("ERROR!  %s\n"), sz);
		pEx->Delete();

		bResult = FALSE;
	}
    return bResult;
}

BOOL CWinInetHttpFecher::TestNetwork()
{
    ASSERT( FALSE );
    return FALSE;
}
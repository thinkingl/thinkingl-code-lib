#include "WinInetHttpFecher.h"
#include "log.h"
#include "IConfig.h"
CWinInetHttpFecher::CWinInetHttpFecher(void)
{
	this->m_pInetFile = NULL;
}

CWinInetHttpFecher::~CWinInetHttpFecher(void)
{
	this->Release();
}

BOOL CWinInetHttpFecher::SetHttpProxy( BOOL bUseProxy, LPCTSTR strIp, uint16 nPort, LPCTSTR strUser, LPCTSTR strPassword )
{
    BOOL bResult = FALSE;
    try
    {
        tstringstream ssProxy;
        ssProxy << strIp << _T( ":" ) << nPort;

        INTERNET_PROXY_INFO proxyInfo;
        memset( &proxyInfo, 0, sizeof( proxyInfo ) );

        tstring strProxyAddr = ssProxy.str();
        proxyInfo.dwAccessType = bUseProxy ? INTERNET_OPEN_TYPE_PROXY : INTERNET_OPEN_TYPE_DIRECT;
        proxyInfo.lpszProxy = strProxyAddr.c_str();

        proxyInfo.lpszProxy = _T( "172.16.128.34:2000" );

        bResult = this->m_wininetSession.SetOption( INTERNET_OPTION_PROXY, &proxyInfo, sizeof( proxyInfo ) );

        if ( strUser && *strUser )
        {
            bResult &= this->m_wininetSession.SetOption( INTERNET_OPTION_PROXY_USERNAME, (LPVOID)strUser, _tcslen( strUser ) );
        }
        
        if ( strPassword && *strPassword )
        {
            bResult &= this->m_wininetSession.SetOption( INTERNET_OPTION_PROXY_PASSWORD, (LPVOID)strPassword, _tcslen( strPassword ) );
        }       

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

BOOL CWinInetHttpFecher::OpenUrl( LPCTSTR strUrl )
{
//	this->Release();

	BOOL bResult = FALSE;
	try
	{
		CStdioFile *pFile = this->m_wininetSession.OpenURL( strUrl );
		if ( pFile )
		{
			this->m_pInetFile = pFile;
			bResult = TRUE;
		}
		else
		{
			CLog() << _T( "OpenUrl fail!! " ) << strUrl << endl;
			bResult = FALSE;
		}
	}
	catch( CInternetException* pEx )
	{
		TCHAR sz[1024];
		pEx->GetErrorMessage(sz, 1024);
		_tprintf_s(_T("ERROR!  %s\n"), sz);
        CLog() << _T( "Open url ") << strUrl << _T( " fail:" ) << sz << endl;
		pEx->Delete();

		bResult = FALSE;
	}
	return bResult;
}

BOOL CWinInetHttpFecher::DownloadFile( LPCTSTR strLocFilePath )
{
	ASSERT( m_pInetFile );
	if ( !m_pInetFile )
	{
		return FALSE;
	}
	BOOL bResult = FALSE;
	try
	{
		if ( m_pInetFile )
		{			
			ofstream of;
			of.open( strLocFilePath );
			if( !of )
			{
				CLog() << _T( "open file fail! path: " ) << strLocFilePath << endl;
				_ASSERT( FALSE );
				return FALSE;
			}
			const int nReadLen = 1000;
			char buff[ nReadLen ];
			while( 1 )
			{
				uint32 uLen = m_pInetFile->Read( buff, nReadLen );
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
 //   ASSERT( FALSE );
    tstring strRootUrl = IConfig::Instance()->GetRootUrl();

    BOOL bResult = this->OpenUrl( strRootUrl.c_str() );

    return bResult;
}

BOOL CWinInetHttpFecher::GetFileType( EHttpFileType& eFileType )
{
	CHttpFile *pHttpFile = dynamic_cast< CHttpFile * > ( this->m_pInetFile );
	if ( pHttpFile )
	{
		CString strType;
		BOOL bResult = pHttpFile->QueryInfo( HTTP_QUERY_CONTENT_TYPE, strType, NULL );
		CLog() << _T( "GetFileType contenttype: " ) << (LPCTSTR)strType << _T( " ret " ) << bResult << endl;
		if ( bResult )
		{
				if ( strType.Find( _T( "text/html" ) ) != -1 )
				{
					eFileType = HttpFileHtmlHtm;
				}
				else
				{
					eFileType = HttpFileOther;
				}
		}
		else
		{
			eFileType = HttpFileOther;
		}

		pHttpFile->QueryInfo( HTTP_QUERY_RAW_HEADERS_CRLF, strType );
		CLog() << _T( "All http head: ********************************************" ) << endl 
			<<  (LPCTSTR)strType << endl << _T( "**********************************************" ) << endl;
	}
	else
	{
		eFileType = HttpFileOther;
	}
//	ASSERT( FALSE );
	return TRUE;
}

void CWinInetHttpFecher::Release()
{
	this->m_wininetSession.Close();
	m_pInetFile = NULL;
}
#include "WinInetHttpFecher.h"
#include "log.h"
#include "IConfig.h"
#include "Common.h"

CWinInetHttpFecher::CWinInetHttpFecher(void)
{
	this->m_pInetFile = NULL;

	this->EnableStatusCallback( TRUE );
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
        this->m_bUseProxy = bUseProxy;
        this->m_strProxyIp = strIp;
        this->m_nProxyPort = nPort;
        this->m_strProxyUsername = strUser;
        this->m_strProxyPassword = strPassword;

		CHttpFile *pHttpFile = dynamic_cast< CHttpFile* >( this->m_pInetFile );
		if ( pHttpFile )
		{
			tstringstream ssProxy;
			ssProxy << strIp << _T( ":" ) << nPort;

			INTERNET_PROXY_INFO proxyInfo;
			memset( &proxyInfo, 0, sizeof( proxyInfo ) );

			tstring strProxyAddr = ssProxy.str();
			proxyInfo.dwAccessType = bUseProxy ? INTERNET_OPEN_TYPE_PROXY : INTERNET_OPEN_TYPE_DIRECT;
			proxyInfo.lpszProxy = strProxyAddr.c_str();

			bResult = pHttpFile->SetOption( INTERNET_OPTION_PROXY, &proxyInfo, sizeof( proxyInfo ) );

			if ( strUser && *strUser )
			{
				bResult &= pHttpFile->SetOption( INTERNET_OPTION_PROXY_USERNAME, 
					(LPVOID)strUser, _tcslen( strUser ) );
				if ( !bResult )
				{
					Log() << _T( "SetOption INTERNET_OPTION_PROXY_USERNAME Fail! er: " ) 
						<< GetLastError() << endl;
				}
			}

			if ( strPassword && *strPassword )
			{
				bResult &= pHttpFile->SetOption( INTERNET_OPTION_PROXY_PASSWORD, 
					(LPVOID)strPassword, _tcslen( strPassword ) );
			}    
		}
		else
		{
//			ASSERT( FALSE );
			Log() << _T( "Set http proxy when Not a http file!!!" ) << endl;
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
	if ( this->m_pInetFile )
	{
		m_pInetFile->Close();
	}

	BOOL bResult = FALSE;
	try
	{
		DWORD       dwFlag = INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD ;
		CStdioFile *pFile = this->OpenURL( strUrl, 1, dwFlag  );
		if ( pFile )
		{
			this->m_pInetFile = pFile;
			bResult = TRUE;
		}
		else
		{
			Log() << _T( "OpenUrl fail!! " ) << strUrl << endl;
			bResult = FALSE;
		}

		CHttpFile *pHttpFile = dynamic_cast< CHttpFile* >(pFile);
		if ( pHttpFile )
		{
			// connection status
			CString      strStatusCode ;
			pHttpFile->QueryInfo ( HTTP_QUERY_STATUS_CODE, strStatusCode ) ;

			// Proxy Authentication Required
			if ( strStatusCode == _T("407") )
			{
				// 代理需要用户名密码校验！
				Log() << _T( "CWinInetHttpFecher::OpenUrl need proxy authentication username and password " ) 
					<< endl;
                this->SetHttpProxy( m_bUseProxy, m_strProxyIp.c_str(), m_nProxyPort, m_strProxyUsername.c_str(), m_strProxyPassword.c_str() );

                pHttpFile->QueryInfo( HTTP_QUERY_STATUS_CODE, strStatusCode );
			}
			if( strStatusCode != _T( "200" ) )
			{
				// 不是200 ok，说明页面打开失败。
				Log() << _T( "Http status code is not 200, is " ) << strStatusCode
					<< _T( " Open Url fail!!! " ) << endl;
				bResult = FALSE;
			}
            else
            {
                bResult = TRUE;
            }
		}

	}
	catch( CInternetException* pEx )
	{
		TCHAR sz[1024];
		pEx->GetErrorMessage(sz, 1024);
		_tprintf_s(_T("ERROR!  %s\n"), sz);
        Log() << _T( "Open url ") << strUrl << _T( " fail:" ) << sz << endl;
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
			// 创建目录。
			tstring strDir = CCommon::ParsePath( strLocFilePath ).m_strDirectory;
			CCommon::CreateDirRecurse( strDir.c_str() );

			ofstream of;
			of.open( strLocFilePath, ios::out | ios::binary );
			if( !of )
			{
				Log() << _T( "open file fail! path: " ) << strLocFilePath << endl;
//				_ASSERT( FALSE );
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
					bResult = TRUE;
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




BOOL CWinInetHttpFecher::GetMimeType( CMimeType& mimetype )
{
	CHttpFile *pHttpFile = dynamic_cast< CHttpFile * > ( this->m_pInetFile );
	if ( pHttpFile )
	{
		CString strType;
		BOOL bResult = pHttpFile->QueryInfo( HTTP_QUERY_CONTENT_TYPE, strType, NULL );
		Log() << _T( "GetFileType contenttype: " ) << (LPCTSTR)strType << _T( " ret " ) << bResult << endl;
		if ( bResult )
		{
				mimetype.Parse( pHttpFile->GetFileURL(), strType );
		}
		else
		{
			Log() << _T( "CWinInetHttpFecher::GetMimeType Fail!!!!! " ) << endl;
			mimetype.Parse( pHttpFile->GetFileURL(), strType );
			bResult = TRUE;	// 不失败。获取不到就用默认的。
		}

//		pHttpFile->QueryInfo( HTTP_QUERY_RAW_HEADERS_CRLF, strType );
//		Log() << _T( "All http head: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx********" ) << endl 
//			<<  (LPCTSTR)strType << endl << _T( "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx**********" ) << endl;

		return bResult;
	}

//	ASSERT( FALSE );
	return FALSE;;
}

void CWinInetHttpFecher::Release()
{
//	this->Close();
	if ( this->m_pInetFile )
	{
		m_pInetFile->Close();

	}
}
//void CWinInetHttpFecher::OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	/** 
//	INTERNET_STATUS_RESOLVING_NAME
//	Looking up the IP address of the name contained in lpvStatusInformation.
//
//	INTERNET_STATUS_NAME_RESOLVED
//	Successfully found the IP address of the name contained in lpvStatusInformation.
//
//	INTERNET_STATUS_CONNECTING_TO_SERVER
//	Connecting to the socket address (SOCKADDR) pointed to by lpvStatusInformation.
//
//	INTERNET_STATUS_CONNECTED_TO_SERVER
//	Successfully connected to the socket address (SOCKADDR) pointed to by lpvStatusInformation.
//
//	INTERNET_STATUS_SENDING_REQUEST
//	Sending the information request to the server. The lpvStatusInformation parameter is NULL.
//
//	INTERNET_STATUS_ REQUEST_SENT
//	Successfully sent the information request to the server. The lpvStatusInformation parameter is NULL.
//
//	INTERNET_STATUS_RECEIVING_RESPONSE
//	Waiting for the server to respond to a request. The lpvStatusInformation parameter is NULL.
//
//	INTERNET_STATUS_RESPONSE_RECEIVED
//	Successfully received a response from the server. The lpvStatusInformation parameter is NULL.
//
//	INTERNET_STATUS_CLOSING_CONNECTION
//	Closing the connection to the server. The lpvStatusInformation parameter is NULL.
//
//	INTERNET_STATUS_CONNECTION_CLOSED
//	Successfully closed the connection to the server. The lpvStatusInformation parameter is NULL.
//
//	INTERNET_STATUS_HANDLE_CREATED
//	Used by the Win32 API function InternetConnect to indicate that it has created the new handle. This lets the application call the Win32 function InternetCloseHandle from another thread if the connect is taking too long. See the Windows SDK for more information about these functions.
//
//	INTERNET_STATUS_HANDLE_CLOSING
//	Successfully terminated this handle value.
//	*/
//	tstring strMsg;
//	switch ( dwInternetStatus )
//	{
//	case INTERNET_STATUS_RESOLVING_NAME:
//		strMsg = _T( "INTERNET_STATUS_RESOLVING_NAME " ) ;//+ (LPCTSTR)lpvStatusInformation;
//		break;
//    case INTERNET_STATUS_NAME_RESOLVED           :
//		strMsg = _T( "INTERNET_STATUS_NAME_RESOLVED " ) ;//+ (LPCTSTR)lpvStatusInformation;
//		break;
//    case INTERNET_STATUS_CONNECTING_TO_SERVER    :
//		strMsg = _T( "INTERNET_STATUS_CONNECTING_TO_SERVER " ) ;
//		break;
//    case INTERNET_STATUS_CONNECTED_TO_SERVER     :
//		strMsg = _T( "INTERNET_STATUS_CONNECTED_TO_SERVER " );
//		break;
//    case INTERNET_STATUS_SENDING_REQUEST         :
//		strMsg = _T( "INTERNET_STATUS_SENDING_REQUEST " );
//		break;
//    case INTERNET_STATUS_REQUEST_SENT            :
//		strMsg = _T( "INTERNET_STATUS_REQUEST_SENT " );
//		break;
//    case INTERNET_STATUS_RECEIVING_RESPONSE      :
////		strMsg = _T( "INTERNET_STATUS_RECEIVING_RESPONSE " );
//		break;
//    case INTERNET_STATUS_RESPONSE_RECEIVED       :
////		strMsg = _T( "INTERNET_STATUS_RESPONSE_RECEIVED " );
//		break;
//    case INTERNET_STATUS_CTL_RESPONSE_RECEIVED   :
//		strMsg = _T( "INTERNET_STATUS_CTL_RESPONSE_RECEIVED " );
//		break;
//    case INTERNET_STATUS_PREFETCH                :
//		strMsg = _T( "INTERNET_STATUS_PREFETCH " );
//		break;
//    case INTERNET_STATUS_CLOSING_CONNECTION      :
//		strMsg = _T( "INTERNET_STATUS_CLOSING_CONNECTION " );
//		break;
//    case INTERNET_STATUS_CONNECTION_CLOSED       :
//		strMsg = _T( "INTERNET_STATUS_CONNECTION_CLOSED " );
//		break;
//    case INTERNET_STATUS_HANDLE_CREATED          :
//		strMsg = _T( "INTERNET_STATUS_HANDLE_CREATED " );
//		break;
//    case INTERNET_STATUS_HANDLE_CLOSING          :
//		strMsg = _T( "INTERNET_STATUS_HANDLE_CLOSING " );
//		break;
//    case INTERNET_STATUS_DETECTING_PROXY         :
//		strMsg = _T( "INTERNET_STATUS_DETECTING_PROXY " );
//		break;
//    case INTERNET_STATUS_REQUEST_COMPLETE        :
//		strMsg = _T( "INTERNET_STATUS_REQUEST_COMPLETE " );
//		break;
//    case INTERNET_STATUS_REDIRECT                :
//		strMsg = _T( "INTERNET_STATUS_REDIRECT " );
//		break;
//    case INTERNET_STATUS_INTERMEDIATE_RESPONSE   :
//		strMsg = _T( "INTERNET_STATUS_INTERMEDIATE_RESPONSE " );
//		break;
//    case INTERNET_STATUS_USER_INPUT_REQUIRED     :
//		strMsg = _T( "INTERNET_STATUS_USER_INPUT_REQUIRED " );
//		break;
//    case INTERNET_STATUS_STATE_CHANGE            :
//		strMsg = _T( "INTERNET_STATUS_STATE_CHANGE " );
//		break;
//    case INTERNET_STATUS_COOKIE_SENT             :
//		strMsg = _T( "INTERNET_STATUS_COOKIE_SENT " );
//		break;
//    case INTERNET_STATUS_COOKIE_RECEIVED         :
//		strMsg = _T( "INTERNET_STATUS_COOKIE_RECEIVED " );
//		break;
//    case INTERNET_STATUS_PRIVACY_IMPACTED        :
//		strMsg = _T( "INTERNET_STATUS_PRIVACY_IMPACTED " );
//		break;
//    case INTERNET_STATUS_P3P_HEADER              :
//		strMsg = _T( "INTERNET_STATUS_P3P_HEADER " );
//		break;
//    case INTERNET_STATUS_P3P_POLICYREF           :
//		strMsg = _T( "INTERNET_STATUS_P3P_POLICYREF " );
//		break;
//    case INTERNET_STATUS_COOKIE_HISTORY          :
//		strMsg = _T( "INTERNET_STATUS_COOKIE_HISTORY " );
//		break;
//	default:
//		strMsg = _T( "Unknown status!" );
//		ASSERT( FALSE );
//		break;
//	}
//
//	if ( !strMsg.empty() )
//	{
//		Log() << _T( "Internet session status: " ) << strMsg << endl;
//	}
//	
//
//	__super::OnStatusCallback(dwContext, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
//}

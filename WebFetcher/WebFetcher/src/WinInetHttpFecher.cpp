#include "WinInetHttpFecher.h"
#include "log.h"
#include "IConfig.h"
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
					CLog() << _T( "SetOption INTERNET_OPTION_PROXY_USERNAME Fail! er: " ) 
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
			ASSERT( FALSE );
			CLog() << _T( "Set http proxy when Not a http file!!!" ) << endl;
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
		DWORD       dwFlag = INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD ;
		CStdioFile *pFile = this->OpenURL( strUrl, 1, dwFlag  );
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
				CLog() << _T( "CWinInetHttpFecher::OpenUrl need proxy authentication username and password " ) 
					<< endl;
			}
			else if( strStatusCode != _T( "200" ) )
			{
				// 不是200 ok，说明页面打开失败。
				CLog() << _T( "Http status code is not 200, is " ) << strStatusCode
					<< _T( " Open Url fail!!! " ) << endl;
				return FALSE;
			}
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
			of.open( strLocFilePath, ios::out | ios::binary );
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

struct TMimeType
{
	IHttpDownloader::EMimeType m_eMimeType;
	tstring m_strContentTypeToken;
	tstring m_strFileExt;
	
	TMimeType(IHttpDownloader::EMimeType eMimeType, LPCTSTR strContentTypeToken, LPCTSTR strFileExt )
	{
		this->m_eMimeType = eMimeType;
		this->m_strContentTypeToken = strContentTypeToken;
		this->m_strFileExt = strFileExt;
	}
};

static const TMimeType s_arMimeTypeDic[]=
{
	TMimeType( IHttpDownloader::HttpMimeHtmlHtm, _T( "text/html" ), _T( "html" ) ),
	TMimeType( IHttpDownloader::HttpMimeHtmlHtm, _T( "text/html" ), _T( "htm" ) ),
	TMimeType( IHttpDownloader::HttpMimeHqx, _T( "application/mac-binhex40" ), _T( "hqx" ) ),
	TMimeType( IHttpDownloader::HttpMimeCpt, _T( "xxxxxxxxxx" ), _T( "cpt" ) ),
	TMimeType( IHttpDownloader::HttpMimeDoc, _T( "xxxxxxxxxx" ), _T( "doc" ) ),
	TMimeType( IHttpDownloader::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "bin" ) ),
	TMimeType( IHttpDownloader::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "dms" ) ),
	TMimeType( IHttpDownloader::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "lha" ) ),
	TMimeType( IHttpDownloader::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "lzh" ) ),
	TMimeType( IHttpDownloader::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "exe" ) ),
	TMimeType( IHttpDownloader::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "class" ) ),
	TMimeType( IHttpDownloader::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "so" ) ),
	TMimeType( IHttpDownloader::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "dll" ) ),
	TMimeType( IHttpDownloader::HttpMimeOda, _T( "application/oda" ), _T( "oda" ) ),
	TMimeType( IHttpDownloader::HttpMimePdf, _T( "application/pdf" ), _T( "pdf" ) ),
	TMimeType( IHttpDownloader::HttpMimeAiEpsPs, _T( "application/postscript" ), _T( "ai" ) ),
	TMimeType( IHttpDownloader::HttpMimeAiEpsPs, _T( "application/postscript" ), _T( "eps" ) ),
	TMimeType( IHttpDownloader::HttpMimeAiEpsPs, _T( "application/postscript" ), _T( "ps" ) ),
	TMimeType( IHttpDownloader::HttpMimeSmiSmil, _T( "application/smil" ), _T( "smi" ) ),
	TMimeType( IHttpDownloader::HttpMimeSmiSmil, _T( "application/smil" ), _T( "smil" ) ),
	TMimeType( IHttpDownloader::HttpMimeMif, _T( "application/vnd.mif" ), _T( "mif" ) ),
	TMimeType( IHttpDownloader::HttpMimeXls, _T( "pplication/vnd.ms-excel" ), _T( "xls" ) ),
	TMimeType( IHttpDownloader::HttpMimePpt, _T( "application/vnd.ms-powerpoint" ), _T( "ppt" ) ),
	TMimeType( IHttpDownloader::HttpMimeWbxml, _T( "application/vnd.wap.wbxml" ), _T( "wbxml" ) ),
	TMimeType( IHttpDownloader::HttpMimeWmlc, _T( "application/vnd.wap.wmlc" ), _T( "wmlc" ) ),
	TMimeType( IHttpDownloader::HttpMimeWmlsc, _T( "application/vnd.wap.wmlscriptc" ), _T( "wmlsc" ) ),
	TMimeType( IHttpDownloader::HttpMimeBcpio, _T( "application/x-bcpio" ), _T( "bcpio" ) ),
	TMimeType( IHttpDownloader::HttpMimeVcd, _T( "application/x-cdlink" ), _T( "vcd" ) ),
	TMimeType( IHttpDownloader::HttpMimePgn, _T( "application/x-chess-pgn" ), _T( "pgn" ) ),
	TMimeType( IHttpDownloader::HttpMimeCpio, _T( "application/x-cpio" ), _T( "cpio" ) ),
	TMimeType( IHttpDownloader::HttpMimeCsh, _T( "application/x-csh" ), _T( "csh" ) ),
	TMimeType( IHttpDownloader::HttpMimeDcrDirDxr, _T( "application/x-director" ), _T( "dcr" ) ),
	TMimeType( IHttpDownloader::HttpMimeDcrDirDxr, _T( "application/x-director" ), _T( "dir" ) ),
	TMimeType( IHttpDownloader::HttpMimeDcrDirDxr, _T( "application/x-director" ), _T( "dxr" ) ),
	TMimeType( IHttpDownloader::HttpMimeDvi, _T( "application/x-dvi" ), _T( "dvi" ) ),
	TMimeType( IHttpDownloader::HttpMimeSpl, _T( "application/x-futuresplash" ), _T( "spl" ) ),
	TMimeType( IHttpDownloader::HttpMimeGtar, _T( "application/x-gtar" ), _T( "gtar" ) ),
	TMimeType( IHttpDownloader::HttpMimeHdf, _T( "application/x-hdf" ), _T( "hdf" ) ),
	TMimeType( IHttpDownloader::HttpMimeJs, _T( "application/x-javascript" ), _T( "js" ) ),
	TMimeType( IHttpDownloader::HttpMimeSkpSkdSktSkm, _T( "application/x-koan" ), _T( "skp" ) ),
	TMimeType( IHttpDownloader::HttpMimeSkpSkdSktSkm, _T( "application/x-koan" ), _T( "skd" ) ),
	TMimeType( IHttpDownloader::HttpMimeSkpSkdSktSkm, _T( "application/x-koan" ), _T( "skt" ) ),
	TMimeType( IHttpDownloader::HttpMimeSkpSkdSktSkm, _T( "application/x-koan" ), _T( "skm" ) ),
	TMimeType( IHttpDownloader::HttpMimeLatex, _T( "application/x-latex" ), _T( "latex" ) ),
	TMimeType( IHttpDownloader::HttpMimeNcCdf, _T( "application/x-netcdf" ), _T( "nc" ) ),
	TMimeType( IHttpDownloader::HttpMimeNcCdf, _T( "application/x-netcdf" ), _T( "cdf" ) ),
	TMimeType( IHttpDownloader::HttpMimeSh, _T( "application/x-sh" ), _T( "sh" ) ),
	TMimeType( IHttpDownloader::HttpMimeShar, _T( "application/x-shar" ), _T( "shar" ) ),
	TMimeType( IHttpDownloader::HttpMimeSwf, _T( "application/x-shockwave-flash" ), _T( "swf" ) ),
	TMimeType( IHttpDownloader::HttpMimeSit, _T( "application/x-stuffit" ), _T( "sit" ) ),
	TMimeType( IHttpDownloader::HttpMimeSv4cpio, _T( "application/x-sv4cpio" ), _T( "sv4cpio" ) ),
	TMimeType( IHttpDownloader::HttpMimeSv4crc, _T( "application/x-sv4crc" ), _T( "sv4crc" ) ),
	TMimeType( IHttpDownloader::HttpMimeTar, _T( "application/x-tar" ), _T( "tar" ) ),
	TMimeType( IHttpDownloader::HttpMimeTcl, _T( "application/x-tcl" ), _T( "tcl" ) ),
	TMimeType( IHttpDownloader::HttpMimeTex, _T( "application/x-tex" ), _T( "tex" ) ),
	TMimeType( IHttpDownloader::HttpMimeTexinfoTexi, _T( "application/x-texinfo" ), _T( "texinfo" ) ),
	TMimeType( IHttpDownloader::HttpMimeTexinfoTexi, _T( "application/x-texinfo" ), _T( "texi" ) ),
	TMimeType( IHttpDownloader::HttpMimeT_TrRoff, _T( "application/x-troff" ), _T( "t" ) ),
	TMimeType( IHttpDownloader::HttpMimeT_TrRoff, _T( "application/x-troff" ), _T( "tr" ) ),
	TMimeType( IHttpDownloader::HttpMimeT_TrRoff, _T( "application/x-troff" ), _T( "roff" ) ),
	TMimeType( IHttpDownloader::HttpMimeMan, _T( "application/x-troff-man" ), _T( "man" ) ),
	TMimeType( IHttpDownloader::HttpMimeMe, _T( "application/x-troff-me" ), _T( "me" ) ),
	TMimeType( IHttpDownloader::HttpMimeMs, _T( "application/x-troff-ms" ), _T( "ms" ) ),
	TMimeType( IHttpDownloader::HttpMimeUstar, _T( "application/x-ustar" ), _T( "ustar" ) ),
	TMimeType( IHttpDownloader::HttpMimeSrc, _T( "application/x-wais-source" ), _T( "src" ) ),
	TMimeType( IHttpDownloader::HttpMimeXhtmlXht, _T( "application/xhtml+xml" ), _T( "xhtml" ) ),
	TMimeType( IHttpDownloader::HttpMimeXhtmlXht, _T( "application/xhtml+xml" ), _T( "xht" ) ),
	TMimeType( IHttpDownloader::HttpMimeZip, _T( "application/zip" ), _T( "zip" ) ),
	TMimeType( IHttpDownloader::HttpMimeAuSnd, _T( "audio/basic" ), _T( "au" ) ),
	TMimeType( IHttpDownloader::HttpMimeAuSnd, _T( "audio/basic" ), _T( "snd" ) ),
	TMimeType( IHttpDownloader::HttpMimeMidMidiKar, _T( "audio/midi" ), _T( "mid" ) ),
	TMimeType( IHttpDownloader::HttpMimeMidMidiKar, _T( "audio/midi" ), _T( "midi" ) ),
	TMimeType( IHttpDownloader::HttpMimeMidMidiKar, _T( "audio/midi" ), _T( "kar" ) ),
	TMimeType( IHttpDownloader::HttpMimeMpgaMp2Mp3, _T( "audio/mpeg" ), _T( "mpga" ) ),
	TMimeType( IHttpDownloader::HttpMimeMpgaMp2Mp3, _T( "audio/mpeg" ), _T( "mp2" ) ),
	TMimeType( IHttpDownloader::HttpMimeMpgaMp2Mp3, _T( "audio/mpeg" ), _T( "mp3" ) ),
	TMimeType( IHttpDownloader::HttpMimeAifAiffAifc, _T( "audio/x-aiff" ), _T( "aif" ) ),
	TMimeType( IHttpDownloader::HttpMimeAifAiffAifc, _T( "audio/x-aiff" ), _T( "aiff" ) ),
	TMimeType( IHttpDownloader::HttpMimeAifAiffAifc, _T( "audio/x-aiff" ), _T( "aifc" ) ),
	TMimeType( IHttpDownloader::HttpMimeM3u, _T( "audio/x-mpegurl" ), _T( "m3u" ) ),
	TMimeType( IHttpDownloader::HttpMimeRamRm, _T( "audio/x-pn-realaudio" ), _T( "ram" ) ),
	TMimeType( IHttpDownloader::HttpMimeRamRm, _T( "audio/x-pn-realaudio" ), _T( "rm" ) ),
	TMimeType( IHttpDownloader::HttpMimeRpm, _T( "audio/x-pn-realaudio-plugin" ), _T( "rpm" ) ),
	TMimeType( IHttpDownloader::HttpMimeRa, _T( "audio/x-realaudio" ), _T( "ra" ) ),
	TMimeType( IHttpDownloader::HttpMimeWav, _T( "audio/x-wav" ), _T( "wav" ) ),
	TMimeType( IHttpDownloader::HttpMimePdb, _T( "chemical/x-pdb" ), _T( "pdb" ) ),
	TMimeType( IHttpDownloader::HttpMimeXyz, _T( "chemical/x-xyz" ), _T( "xyz" ) ),
	TMimeType( IHttpDownloader::HttpMimeBmp, _T( "image/bmp" ), _T( "bmp" ) ),
	TMimeType( IHttpDownloader::HttpMimeGif, _T( "image/gif" ), _T( "gif" ) ),
	TMimeType( IHttpDownloader::HttpMimeIef, _T( "image/ief" ), _T( "ief" ) ),
	TMimeType( IHttpDownloader::HttpMimeJpegJpgJpe, _T( "image/jpeg" ), _T( "jpeg" ) ),
	TMimeType( IHttpDownloader::HttpMimeJpegJpgJpe, _T( "image/jpeg" ), _T( "jpg" ) ),
	TMimeType( IHttpDownloader::HttpMimeJpegJpgJpe, _T( "image/jpeg" ), _T( "jpe" ) ),
	TMimeType( IHttpDownloader::HttpMimePng, _T( "image/png" ), _T( "png" ) ),
	TMimeType( IHttpDownloader::HttpMimeTiffTif, _T( "image/tiff" ), _T( "tiff" ) ),
	TMimeType( IHttpDownloader::HttpMimeTiffTif, _T( "image/tiff" ), _T( "tif" ) ),
	TMimeType( IHttpDownloader::HttpMimeDjvuDjv, _T( "image/vnd.djvu" ), _T( "djvu" ) ),
	TMimeType( IHttpDownloader::HttpMimeDjvuDjv, _T( "image/vnd.djvu" ), _T( "djv" ) ),
	TMimeType( IHttpDownloader::HttpMimeWbmp, _T( "image/vnd.wap.wbmp" ), _T( "wbmp" ) ),
	TMimeType( IHttpDownloader::HttpMimeRas, _T( "image/x-cmu-raster" ), _T( "ras" ) ),
	TMimeType( IHttpDownloader::HttpMimePnm, _T( "image/x-portable-anymap" ), _T( "pnm" ) ),
	TMimeType( IHttpDownloader::HttpMimePbm, _T( "image/x-portable-bitmap" ), _T( "pbm" ) ),
	TMimeType( IHttpDownloader::HttpMimePgm, _T( "image/x-portable-graymap" ), _T( "pgm" ) ),
	TMimeType( IHttpDownloader::HttpMimePpm, _T( "image/x-portable-pixmap" ), _T( "ppm" ) ),
	TMimeType( IHttpDownloader::HttpMimeRgb, _T( "image/x-rgb" ), _T( "rgb" ) ),
	TMimeType( IHttpDownloader::HttpMimeXbmXpm, _T( "image/x-xpixmap" ), _T( "xbm" ) ),
	TMimeType( IHttpDownloader::HttpMimeXbmXpm, _T( "image/x-xpixmap" ), _T( "xpm" ) ),
	TMimeType( IHttpDownloader::HttpMimeXwd, _T( "image/x-xwindowdump" ), _T( "xwd" ) ),
	TMimeType( IHttpDownloader::HttpMimeIgsIges, _T( "model/iges" ), _T( "igs" ) ),
	TMimeType( IHttpDownloader::HttpMimeIgsIges, _T( "model/iges" ), _T( "iges" ) ),
	TMimeType( IHttpDownloader::HttpMimeMshMeshSilo, _T( "model/mesh" ), _T( "msh" ) ),
	TMimeType( IHttpDownloader::HttpMimeMshMeshSilo, _T( "model/mesh" ), _T( "mesh" ) ),
	TMimeType( IHttpDownloader::HttpMimeMshMeshSilo, _T( "model/mesh" ), _T( "silo" ) ),
	TMimeType( IHttpDownloader::HttpMimeWrlVrml, _T( "model/vrml" ), _T( "wrl" ) ),
	TMimeType( IHttpDownloader::HttpMimeWrlVrml, _T( "model/vrml" ), _T( "vrml" ) ),
	TMimeType( IHttpDownloader::HttpMimeCss, _T( "text/css" ), _T( "css" ) ),
	TMimeType( IHttpDownloader::HttpMimeAscTxt, _T( "text/plain" ), _T( "asc" ) ),
	TMimeType( IHttpDownloader::HttpMimeAscTxt, _T( "text/plain" ), _T( "txt" ) ),
	TMimeType( IHttpDownloader::HttpMimeRtx, _T( "text/richtext" ), _T( "rtx" ) ),
	TMimeType( IHttpDownloader::HttpMimeRtf, _T( "text/rtf" ), _T( "rtf" ) ),
	TMimeType( IHttpDownloader::HttpMimeSgmlSgm, _T( "text/sgml" ), _T( "sgml" ) ),
	TMimeType( IHttpDownloader::HttpMimeSgmlSgm, _T( "text/sgml" ), _T( "sgm" ) ),
	TMimeType( IHttpDownloader::HttpMimeTsv, _T( "text/tab-separated-values" ), _T( "tsv" ) ),
	TMimeType( IHttpDownloader::HttpMimeWml, _T( "text/vnd.wap.wml" ), _T( "wml" ) ),
	TMimeType( IHttpDownloader::HttpMimeWmlc, _T( "text/vnd.wap.wmlscript" ), _T( "wmls" ) ),
	TMimeType( IHttpDownloader::HttpMimeEtx, _T( "text/x-setext" ), _T( "etx" ) ),
	TMimeType( IHttpDownloader::HttpMimeXslXml, _T( "text/xml" ), _T( "xsl" ) ),
	TMimeType( IHttpDownloader::HttpMimeXslXml, _T( "text/xml" ), _T( "xml" ) ),
	TMimeType( IHttpDownloader::HttpMimeMpegMpgMpe, _T( "video/mpeg" ), _T( "mpeg" ) ),
	TMimeType( IHttpDownloader::HttpMimeMpegMpgMpe, _T( "video/mpeg" ), _T( "mpg" ) )	,
	TMimeType( IHttpDownloader::HttpMimeMpegMpgMpe, _T( "video/mpeg" ), _T( "mpe" ) ),
	TMimeType( IHttpDownloader::HttpMimeQtMov, _T( "video/quicktime" ), _T( "qt" ) ),
	TMimeType( IHttpDownloader::HttpMimeQtMov, _T( "video/quicktime" ), _T( "mov" ) ),
	TMimeType( IHttpDownloader::HttpMimeMxu, _T( "video/vnd.mpegurl" ), _T( "mxu" ) ),
	TMimeType( IHttpDownloader::HttpMimeAvi, _T( "video/x-msvideo" ), _T( "avi" ) ),
	TMimeType( IHttpDownloader::HttpMimeMovie, _T( "video/x-sgi-movie" ), _T( "movie" ) ),
	TMimeType( IHttpDownloader::HttpMimeIce, _T( "x-conference/x-cooltalk" ), _T( "ice" ) ),


	TMimeType( IHttpDownloader::HttpMimeOther, NULL, NULL )

};


BOOL CWinInetHttpFecher::GetFileType( EMimeType& eFileType )
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
					eFileType = HttpMimeHtmlHtm;
				}
				else
				{
					eFileType = HttpMimeOther;
				}
		}
		else
		{
			eFileType = HttpMimeOther;
		}

		pHttpFile->QueryInfo( HTTP_QUERY_RAW_HEADERS_CRLF, strType );
		CLog() << _T( "All http head: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx********" ) << endl 
			<<  (LPCTSTR)strType << endl << _T( "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx**********" ) << endl;
	}
	else
	{
		eFileType = HttpMimeOther;
	}
//	ASSERT( FALSE );
	return TRUE;
}

void CWinInetHttpFecher::Release()
{
	this->Close();
	m_pInetFile = NULL;
}
void CWinInetHttpFecher::OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	// TODO: 在此添加专用代码和/或调用基类

	/** 
	INTERNET_STATUS_RESOLVING_NAME
	Looking up the IP address of the name contained in lpvStatusInformation.

	INTERNET_STATUS_NAME_RESOLVED
	Successfully found the IP address of the name contained in lpvStatusInformation.

	INTERNET_STATUS_CONNECTING_TO_SERVER
	Connecting to the socket address (SOCKADDR) pointed to by lpvStatusInformation.

	INTERNET_STATUS_CONNECTED_TO_SERVER
	Successfully connected to the socket address (SOCKADDR) pointed to by lpvStatusInformation.

	INTERNET_STATUS_SENDING_REQUEST
	Sending the information request to the server. The lpvStatusInformation parameter is NULL.

	INTERNET_STATUS_ REQUEST_SENT
	Successfully sent the information request to the server. The lpvStatusInformation parameter is NULL.

	INTERNET_STATUS_RECEIVING_RESPONSE
	Waiting for the server to respond to a request. The lpvStatusInformation parameter is NULL.

	INTERNET_STATUS_RESPONSE_RECEIVED
	Successfully received a response from the server. The lpvStatusInformation parameter is NULL.

	INTERNET_STATUS_CLOSING_CONNECTION
	Closing the connection to the server. The lpvStatusInformation parameter is NULL.

	INTERNET_STATUS_CONNECTION_CLOSED
	Successfully closed the connection to the server. The lpvStatusInformation parameter is NULL.

	INTERNET_STATUS_HANDLE_CREATED
	Used by the Win32 API function InternetConnect to indicate that it has created the new handle. This lets the application call the Win32 function InternetCloseHandle from another thread if the connect is taking too long. See the Windows SDK for more information about these functions.

	INTERNET_STATUS_HANDLE_CLOSING
	Successfully terminated this handle value.
	*/
	tstring strMsg;
	switch ( dwInternetStatus )
	{
	case INTERNET_STATUS_RESOLVING_NAME:
		strMsg = _T( "INTERNET_STATUS_RESOLVING_NAME " ) ;//+ (LPCTSTR)lpvStatusInformation;
		break;
    case INTERNET_STATUS_NAME_RESOLVED           :
		strMsg = _T( "INTERNET_STATUS_NAME_RESOLVED " ) ;//+ (LPCTSTR)lpvStatusInformation;
		break;
    case INTERNET_STATUS_CONNECTING_TO_SERVER    :
		strMsg = _T( "INTERNET_STATUS_CONNECTING_TO_SERVER " ) ;
		break;
    case INTERNET_STATUS_CONNECTED_TO_SERVER     :
		strMsg = _T( "INTERNET_STATUS_CONNECTED_TO_SERVER " );
		break;
    case INTERNET_STATUS_SENDING_REQUEST         :
		strMsg = _T( "INTERNET_STATUS_SENDING_REQUEST " );
		break;
    case INTERNET_STATUS_REQUEST_SENT            :
		strMsg = _T( "INTERNET_STATUS_REQUEST_SENT " );
		break;
    case INTERNET_STATUS_RECEIVING_RESPONSE      :
		strMsg = _T( "INTERNET_STATUS_RECEIVING_RESPONSE " );
		break;
    case INTERNET_STATUS_RESPONSE_RECEIVED       :
		strMsg = _T( "INTERNET_STATUS_RESPONSE_RECEIVED " );
		break;
    case INTERNET_STATUS_CTL_RESPONSE_RECEIVED   :
		strMsg = _T( "INTERNET_STATUS_CTL_RESPONSE_RECEIVED " );
		break;
    case INTERNET_STATUS_PREFETCH                :
		strMsg = _T( "INTERNET_STATUS_PREFETCH " );
		break;
    case INTERNET_STATUS_CLOSING_CONNECTION      :
		strMsg = _T( "INTERNET_STATUS_CLOSING_CONNECTION " );
		break;
    case INTERNET_STATUS_CONNECTION_CLOSED       :
		strMsg = _T( "INTERNET_STATUS_CONNECTION_CLOSED " );
		break;
    case INTERNET_STATUS_HANDLE_CREATED          :
		strMsg = _T( "INTERNET_STATUS_HANDLE_CREATED " );
		break;
    case INTERNET_STATUS_HANDLE_CLOSING          :
		strMsg = _T( "INTERNET_STATUS_HANDLE_CLOSING " );
		break;
    case INTERNET_STATUS_DETECTING_PROXY         :
		strMsg = _T( "INTERNET_STATUS_DETECTING_PROXY " );
		break;
    case INTERNET_STATUS_REQUEST_COMPLETE        :
		strMsg = _T( "INTERNET_STATUS_REQUEST_COMPLETE " );
		break;
    case INTERNET_STATUS_REDIRECT                :
		strMsg = _T( "INTERNET_STATUS_REDIRECT " );
		break;
    case INTERNET_STATUS_INTERMEDIATE_RESPONSE   :
		strMsg = _T( "INTERNET_STATUS_INTERMEDIATE_RESPONSE " );
		break;
    case INTERNET_STATUS_USER_INPUT_REQUIRED     :
		strMsg = _T( "INTERNET_STATUS_USER_INPUT_REQUIRED " );
		break;
    case INTERNET_STATUS_STATE_CHANGE            :
		strMsg = _T( "INTERNET_STATUS_STATE_CHANGE " );
		break;
    case INTERNET_STATUS_COOKIE_SENT             :
		strMsg = _T( "INTERNET_STATUS_COOKIE_SENT " );
		break;
    case INTERNET_STATUS_COOKIE_RECEIVED         :
		strMsg = _T( "INTERNET_STATUS_COOKIE_RECEIVED " );
		break;
    case INTERNET_STATUS_PRIVACY_IMPACTED        :
		strMsg = _T( "INTERNET_STATUS_PRIVACY_IMPACTED " );
		break;
    case INTERNET_STATUS_P3P_HEADER              :
		strMsg = _T( "INTERNET_STATUS_P3P_HEADER " );
		break;
    case INTERNET_STATUS_P3P_POLICYREF           :
		strMsg = _T( "INTERNET_STATUS_P3P_POLICYREF " );
		break;
    case INTERNET_STATUS_COOKIE_HISTORY          :
		strMsg = _T( "INTERNET_STATUS_COOKIE_HISTORY " );
		break;
	default:
		strMsg = _T( "Unknown status!" );
		ASSERT( FALSE );
		break;
	}

	CLog() << _T( "Internet session status: " ) << strMsg << endl;

	__super::OnStatusCallback(dwContext, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
}

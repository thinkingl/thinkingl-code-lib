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

BOOL CWinInetHttpFecher::FecheFile( LPCTSTR strUrl, LPCTSTR strLocFilePath )
{
    return FALSE;
}
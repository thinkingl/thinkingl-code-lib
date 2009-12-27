#include "IniConfig.h"
#include "Common.h"


LPCTSTR CONFIG_INI_FILE = _T( "WEBFETCH.INI" );

LPCTSTR CONFIG_CFG_APP_NAME = _T( "CONFIG" );
LPCTSTR CONFIG_FILTER_APP = _T( "FETCHFILETER" );
LPCTSTR CONFIG_BAN_APP = _T( "BANURL" );

LPCTSTR CONFIG_SERVER_ROOT = _T( "SERVERROOT" );
LPCTSTR CONFIG_LOCAL_ROOT = _T( "LOCALROOT" );
LPCTSTR CONFIG_THREAD_COUNT = _T( "THREADCOUNT" );

LPCTSTR CONFIG_USE_PROXY = _T( "USEPROXY" );
LPCTSTR CONFIG_PROXY_IP = _T( "PROXYIP" );
LPCTSTR CONFIG_PROXY_PORT = _T( "PROXYPORT" );
LPCTSTR CONFIG_AUTHENTICATION = _T( "AUTHENTICATION" );
LPCTSTR COFNIG_PROXY_USER = _T( "PROXYUSER" );
LPCTSTR CONFIG_PROXY_PASSWORD = _T( "PROXYPASSWORD" );

CIniConfig::CIniConfig(void)
{
	BOOL bResult = this->Init();
	ASSERT( bResult );
}

CIniConfig::~CIniConfig(void)
{
}

tstring CIniConfig::GetRootFolder()
{
	tstring strRootFolder = this->ReadConfigStr( CONFIG_CFG_APP_NAME, CONFIG_LOCAL_ROOT );
	if ( strRootFolder.empty() )
	{
		strRootFolder = CCommon::GetAppDir();
	}

	if ( !strRootFolder.empty() && ( strRootFolder.at( strRootFolder.length() - 1 ) != '\\' ) )
	{
		strRootFolder += '\\';
	}

	return strRootFolder;
}

tstring CIniConfig::GetRootUrl()
{
	return this->ReadConfigStr( CONFIG_CFG_APP_NAME, CONFIG_SERVER_ROOT );
}

BOOL CIniConfig::IsUseProxy()
{
    return this->ReadConfigInt( CONFIG_CFG_APP_NAME, CONFIG_USE_PROXY );
}

BOOL CIniConfig::IsProxyAuthentication()
{
    return this->ReadConfigInt( CONFIG_CFG_APP_NAME, CONFIG_AUTHENTICATION );
}

tstring CIniConfig::GetProxyUserName()
{
	return this->ReadConfigStr( CONFIG_CFG_APP_NAME, COFNIG_PROXY_USER );
}

tstring CIniConfig::GetProxyIp()
{
	return this->ReadConfigStr( CONFIG_CFG_APP_NAME, CONFIG_PROXY_IP );
}

tstring CIniConfig::GetProxyPassword()
{
	return this->ReadConfigStr( CONFIG_CFG_APP_NAME, CONFIG_PROXY_PASSWORD );
}

uint16 CIniConfig::GetProxyPort()
{
	return (uint16)this->ReadConfigInt( CONFIG_CFG_APP_NAME, CONFIG_PROXY_PORT );
}

tstring CIniConfig::ReadConfigStr( LPCTSTR strApp, LPCTSTR strKey )
{
	const int nSize = 10000;
	TCHAR arBuf[ nSize ] = {0};
	::GetPrivateProfileString( strApp, strKey, NULL, arBuf, nSize, this->m_strIniFilePath.c_str() );

	return arBuf;
}

int32 CIniConfig::ReadConfigInt( LPCTSTR strApp, LPCTSTR strKey )
{
	tstring strCfg = this->ReadConfigStr( strApp, strKey );
	tstringstream ssTmp;
	ssTmp << strCfg;
	int32 nCfg = 0;
	ssTmp >> nCfg;
	return nCfg;
}

BOOL CIniConfig::Init()
{
	// 获取本App路径。
	this->m_strIniFilePath = CCommon::GetAppDir() + CONFIG_INI_FILE;
//	::WritePrivateProfileString( CONFIG_CFG_APP_NAME, _T( "TEST" ), _T( "VALUE" ), CONFIG_INI_FILE );

	// 使用相对路径。
//	this->m_strIniFilePath = CONFIG_INI_FILE;
	return TRUE;
}

uint32 CIniConfig::GetThreadCount()
{
	return this->ReadConfigInt( CONFIG_CFG_APP_NAME, CONFIG_THREAD_COUNT );
}

tstringarray CIniConfig::GetAllFetchFilter()
{
	return this->ReadKeys( CONFIG_FILTER_APP );
}

tstringarray CIniConfig::GetAllBanUrl()
{
	return this->ReadKeys( CONFIG_BAN_APP );
}

BOOL CIniConfig::IsUrlBan( LPCTSTR strUrl )
{
	return this->ReadConfigInt( CONFIG_BAN_APP, strUrl );
}

tstringarray CIniConfig::ReadKeys( LPCTSTR strApp )
{
	const int buflen = 100000;
	TCHAR buf[ buflen ] = { 0 };
	::GetPrivateProfileString( strApp, NULL, NULL, buf, buflen, this->m_strIniFilePath.c_str() );

	tstringarray tar;
	LPCTSTR strKey = buf;
	while ( *strKey )
	{
		tar.push_back( strKey );

		strKey += _tcslen( strKey );
		strKey ++;
	}

	return tar;
}

BOOL CIniConfig::IsUrlFilterFetch( LPCTSTR strFilter )
{
	return this->ReadConfigInt( CONFIG_FILTER_APP, strFilter );
}

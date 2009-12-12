#include "IniConfig.h"
#include "Common.h"


LPCTSTR CONFIG_INI_FILE = _T( "WEBFETCH.INI" );
LPCTSTR CONFIG_ENTRY_NAME = _T( "CONFIG" );

LPCTSTR CONFIG_SERVER_ROOT = _T( "SERVERROOT" );
LPCTSTR CONFIG_LOCAL_ROOT = _T( "LOCALROOT" );

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
	return this->ReadConfigStr( CONFIG_LOCAL_ROOT );
}

tstring CIniConfig::GetRootUrl()
{
	return this->ReadConfigStr( CONFIG_SERVER_ROOT );
}

tstring CIniConfig::GetProxyUserName()
{
	return _T( "" );
}

tstring CIniConfig::GetProxyIp()
{
	return _T( "" );
}

tstring CIniConfig::GetProxyPassword()
{
	return _T( "" );
}

uint16 CIniConfig::GetProxyPort()
{
	return 0;
}

tstring CIniConfig::ReadConfigStr( LPCTSTR strName )
{
	const int nSize = 10000;
	TCHAR arBuf[ nSize ] = {0};
	::GetPrivateProfileString( CONFIG_ENTRY_NAME, strName, NULL, arBuf, nSize, this->m_strIniFilePath.c_str() );

	return arBuf;
}

BOOL CIniConfig::Init()
{
	// 获取本App路径。
	this->m_strIniFilePath = CCommon::GetAppDir() + CONFIG_INI_FILE;
//	::WritePrivateProfileString( CONFIG_ENTRY_NAME, _T( "TEST" ), _T( "VALUE" ), CONFIG_INI_FILE );

	// 使用相对路径。
//	this->m_strIniFilePath = CONFIG_INI_FILE;
	return TRUE;
}

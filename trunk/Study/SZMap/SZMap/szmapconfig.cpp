#include "szmapconfig.h"
#include "Common.h"


CIniConfigFile CSZMapConfig::s_iniConfigFile( CCommon::GetAppDir() + _T( "szmap.ini" ) );

CSZMapConfig::CSZMapConfig(void)
{
}


CSZMapConfig::~CSZMapConfig(void)
{
}

tstring CSZMapConfig::GetMapImageDir()
{
	tstring strDefImgDir = CCommon::GetAppDir();
	return s_iniConfigFile.ReadString( _T("szmapcfg"), _T("mapdir"), strDefImgDir );
}

BOOL CSZMapConfig::SetMapImageDir( ctstring& imgDir )
{
	BOOL ret = s_iniConfigFile.Write( _T("szmapcfg"), _T("mapdir"), imgDir );
	return ret;	
}

tstring CSZMapConfig::GetMapSubDir()
{
	return s_iniConfigFile.ReadString( _T("szmapcfg"), _T("subdir"),_T("tiles") );
}

BOOL CSZMapConfig::SetMapSubDir( ctstring& subDir )
{
	BOOL ret = s_iniConfigFile.Write( _T("szmapcfg"), _T( "subdir" ), subDir );
	return ret;
}

tstring CSZMapConfig::GetDatabasePath()
{
	return s_iniConfigFile.ReadString( _T("szmapcfg"), _T("DatabasePath"),_T("") );
}

BOOL CSZMapConfig::SetDatabasePath( ctstring& databasePath )
{
	BOOL ret = s_iniConfigFile.Write( _T("szmapcfg"), _T("DatabasePath"), databasePath );
	return ret;
}
//end of file

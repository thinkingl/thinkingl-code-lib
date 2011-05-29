#include "IniConfigFile.h"


CIniConfigFile::CIniConfigFile( ctstring& iniFilePath )
{
	this->m_strIniFilePath = iniFilePath;
}


CIniConfigFile::~CIniConfigFile(void)
{
}

tstring CIniConfigFile::ReadString( ctstring& strApp, ctstring& strKey, ctstring& defaultValue )
{
	TCHAR szBuf[ MAX_PATH*2 ] = {0};
	::GetPrivateProfileString( strApp.c_str(), strKey.c_str(), defaultValue.c_str(), szBuf, ARRAYSIZE( szBuf ), m_strIniFilePath.c_str() );
	return szBuf;
}

BOOL CIniConfigFile::Write( ctstring& app, ctstring& key, ctstring& value )
{
	BOOL ret = ::WritePrivateProfileString( app.c_str(), key.c_str(), value.c_str(), this->m_strIniFilePath.c_str() );
	return ret;
}

// end of file

#pragma once

#include "portabledefine.h"

class CIniConfigFile
{
public:
	CIniConfigFile( ctstring& iniFileFullPath );
	virtual ~CIniConfigFile(void);

	// ¶ÁÈ¡
	tstring ReadString(  ctstring& strApp, ctstring& strKey, ctstring& defaultValue );
	int32 ReadInt( ctstring& strApp, ctstring& strKey, int32 defaultValue );

	tstringarray ReadKeys( ctstring& strApp );

	/** ±£´æ. */
//	BOOL WriteString( ctstring& app, ctstring& key, ctstring& value );
	BOOL Write( ctstring& app, ctstring& key, ctstring& value );
	BOOL Write(ctstring& app, ctstring& key, int32 value );

	/** É¾³ý */
	BOOL DelKey( ctstring& app, ctstring& key );
	BOOL DelApp( ctstring& app );

private:
	tstring m_strIniFilePath;
};


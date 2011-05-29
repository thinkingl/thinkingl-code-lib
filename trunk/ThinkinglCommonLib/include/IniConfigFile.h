#pragma once

#include "portabledefine.h"

class CIniConfigFile
{
public:
	CIniConfigFile( ctstring& iniFileFullPath );
	virtual ~CIniConfigFile(void);

	// ��ȡ
	tstring ReadString(  ctstring& strApp, ctstring& strKey, ctstring& defaultValue );
	int32 ReadInt( ctstring& strApp, ctstring& strKey, int32 defaultValue );

	tstringarray ReadKeys( ctstring& strApp );

	/** ����. */
//	BOOL WriteString( ctstring& app, ctstring& key, ctstring& value );
	BOOL Write( ctstring& app, ctstring& key, ctstring& value );
	BOOL Write(ctstring& app, ctstring& key, int32 value );

	/** ɾ�� */
	BOOL DelKey( ctstring& app, ctstring& key );
	BOOL DelApp( ctstring& app );

private:
	tstring m_strIniFilePath;
};


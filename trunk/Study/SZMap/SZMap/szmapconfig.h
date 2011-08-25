#pragma once

#include "IniConfigFile.h"

class CSZMapConfig
{

private:
	CSZMapConfig(void);
	virtual ~CSZMapConfig(void);

public:
	static tstring GetMapImageDir();
	static BOOL SetMapImageDir( ctstring& imgDir );

	static tstring GetMapSubDir();
	static BOOL SetMapSubDir( ctstring& subDir );

	/** ���ݿ��ļ�·��. */
	static tstring GetDatabasePath();
	static BOOL SetDatabasePath( ctstring& databasePath );

private:
	/** �����ļ�. */
	static 	CIniConfigFile s_iniConfigFile;
};


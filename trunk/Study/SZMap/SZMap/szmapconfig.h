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

	/** 数据库文件路径. */
	static tstring GetDatabasePath();
	static BOOL SetDatabasePath( ctstring& databasePath );

private:
	/** 配置文件. */
	static 	CIniConfigFile s_iniConfigFile;
};


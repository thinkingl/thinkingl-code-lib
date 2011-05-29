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
private:
	/** ≈‰÷√Œƒº˛. */
	static 	CIniConfigFile s_iniConfigFile;
};


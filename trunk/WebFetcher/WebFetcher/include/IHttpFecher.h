#pragma once

#include "portabledefine.h"

class IHttpFecher
{
public:

    /** 下载文件。 */
    virtual BOOL FecheFile( LPCTSTR strUrl, LPCTSTR strLocFilePath ) = 0;

	/** 设置http代理。 */
	virtual BOOL SetHttpProxy( BOOL bUseProxy,
		LPCTSTR strIp, 
		uint16 strPort, 
		LPCTSTR strUser, 
		LPCTSTR strPassword ) = 0;
};

#pragma once

#include "portabledefine.h"

class IHttpDownloader
{
public:

    /** 下载文件。 */
    virtual BOOL DownloadFile( LPCTSTR strUrl, LPCTSTR strLocFilePath ) = 0;

	/** 设置http代理。 */
	virtual BOOL SetHttpProxy( BOOL bUseProxy,
		LPCTSTR strIp, 
		uint16 strPort, 
		LPCTSTR strUser, 
		LPCTSTR strPassword ) = 0;

    /** 网络测试。 
    *   连接抓取目标主页测试。
    */
    virtual BOOL TestNetwork() = 0;
};

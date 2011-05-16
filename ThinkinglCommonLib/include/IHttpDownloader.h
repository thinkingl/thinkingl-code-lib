#pragma once

#include "portabledefine.h"
#include "MimeType.h"

class IHttpDownloader
{
public:
    virtual ~IHttpDownloader(){};

	typedef std::vector< char > TDataBuffer;


	/** 打开Url*/
	virtual BOOL OpenUrl( LPCTSTR strUrl ) = 0;
	/** 获取文件类型。 */
	virtual BOOL GetMimeType( CMimeType& mimetype ) = 0;

    /** 下载文件。 */
    virtual BOOL DownloadFile( LPCTSTR strLocFilePath ) = 0;
	
	/** 打开http文件. */
	virtual BOOL OpenFile( TDataBuffer & dataBuf ) = 0;

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

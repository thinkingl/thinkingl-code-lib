#pragma once
#include "ihttpdownloader.h"

class CWinInetHttpFecher :
    public IHttpDownloader
{
public:
    CWinInetHttpFecher(void);
    virtual ~CWinInetHttpFecher(void);

    /** 下载文件。 */
    virtual BOOL DownloadFile( LPCTSTR strUrl, LPCTSTR strLocFilePath ) ;

    /** 设置http代理。 */
    virtual BOOL SetHttpProxy( BOOL bUseProxy,
        LPCTSTR strIp, 
        uint16 strPort, 
        LPCTSTR strUser, 
        LPCTSTR strPassword ) ;

    /** 网络测试。 
    *   连接抓取目标主页测试。
    */
    virtual BOOL TestNetwork();

private:
    CInternetSession m_wininetSession;
};

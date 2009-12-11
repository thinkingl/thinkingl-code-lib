#pragma once
#include "ihttpfecher.h"

class CWinInetHttpFecher :
    public IHttpFecher
{
public:
    CWinInetHttpFecher(void);
    virtual ~CWinInetHttpFecher(void);

    /** 下载文件。 */
    virtual BOOL FecheFile( LPCTSTR strUrl, LPCTSTR strLocFilePath ) ;

    /** 设置http代理。 */
    virtual BOOL SetHttpProxy( BOOL bUseProxy,
        LPCTSTR strIp, 
        uint16 strPort, 
        LPCTSTR strUser, 
        LPCTSTR strPassword ) ;

private:
    CInternetSession m_wininetSession;
};

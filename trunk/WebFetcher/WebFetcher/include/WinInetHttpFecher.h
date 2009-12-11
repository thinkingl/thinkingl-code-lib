#pragma once
#include "ihttpfecher.h"

class CWinInetHttpFecher :
    public IHttpFecher
{
public:
    CWinInetHttpFecher(void);
    virtual ~CWinInetHttpFecher(void);

    /** �����ļ��� */
    virtual BOOL FecheFile( LPCTSTR strUrl, LPCTSTR strLocFilePath ) ;

    /** ����http���� */
    virtual BOOL SetHttpProxy( BOOL bUseProxy,
        LPCTSTR strIp, 
        uint16 strPort, 
        LPCTSTR strUser, 
        LPCTSTR strPassword ) ;

private:
    CInternetSession m_wininetSession;
};

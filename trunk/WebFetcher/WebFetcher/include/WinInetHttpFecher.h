#pragma once
#include "ihttpdownloader.h"

class CWinInetHttpFecher :
    public IHttpDownloader
{
public:
    CWinInetHttpFecher(void);
    virtual ~CWinInetHttpFecher(void);

    /** �����ļ��� */
    virtual BOOL DownloadFile( LPCTSTR strUrl, LPCTSTR strLocFilePath ) ;

    /** ����http���� */
    virtual BOOL SetHttpProxy( BOOL bUseProxy,
        LPCTSTR strIp, 
        uint16 strPort, 
        LPCTSTR strUser, 
        LPCTSTR strPassword ) ;

    /** ������ԡ� 
    *   ����ץȡĿ����ҳ���ԡ�
    */
    virtual BOOL TestNetwork();

private:
    CInternetSession m_wininetSession;
};

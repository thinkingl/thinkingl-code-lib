#pragma once
#include "ihttpdownloader.h"

class CWinInetHttpFecher :
    public IHttpDownloader,
	public CInternetSession
{
public:
    CWinInetHttpFecher(void);
    virtual ~CWinInetHttpFecher(void);

	/** ��Url*/
	virtual BOOL OpenUrl( LPCTSTR strUrl ) ;
	/** ��ȡ�ļ����͡� */
	virtual BOOL GetMimeType( CMimeType& mimetype );

	/** �����ļ��� */
	virtual BOOL DownloadFile( LPCTSTR strLocFilePath ) ;

	/** ��http�ļ�. */
	virtual BOOL OpenFile( TDataBuffer & dataBuf );

    /** ����http���� */
    virtual BOOL SetHttpProxy( BOOL bUseProxy,
        LPCTSTR strIp, 
        uint16 strPort, 
        LPCTSTR strUser, 
        LPCTSTR strPassword ) ;

    /** ������ԡ� 
    *   ����ץȡĿ����ҳ���ԡ�
    */
    virtual BOOL TestNetwork( );

private:
	void Release();
private:
//    CInternetSession m_wininetSession;

	CStdioFile *m_pInetFile;

    BOOL m_bUseProxy;
    tstring m_strProxyIp;
    uint16 m_nProxyPort;
    tstring m_strProxyUsername;
    tstring m_strProxyPassword;

public:
//	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
};

#pragma once
#include "ihttpdownloader.h"

class CWinInetHttpFecher :
    public IHttpDownloader,
	public CInternetSession
{
public:
    CWinInetHttpFecher(void);
    virtual ~CWinInetHttpFecher(void);

	/** 打开Url*/
	virtual BOOL OpenUrl( LPCTSTR strUrl ) ;
	/** 获取文件类型。 */
	virtual BOOL GetMimeType( CMimeType& mimetype );

	/** 下载文件。 */
	virtual BOOL DownloadFile( LPCTSTR strLocFilePath ) ;

	/** 打开http文件. */
	virtual BOOL OpenFile( TDataBuffer & dataBuf );

    /** 设置http代理。 */
    virtual BOOL SetHttpProxy( BOOL bUseProxy,
        LPCTSTR strIp, 
        uint16 strPort, 
        LPCTSTR strUser, 
        LPCTSTR strPassword ) ;

    /** 网络测试。 
    *   连接抓取目标主页测试。
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

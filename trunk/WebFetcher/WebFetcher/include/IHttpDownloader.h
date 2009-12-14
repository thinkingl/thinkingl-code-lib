#pragma once

#include "portabledefine.h"

class IHttpDownloader
{
public:

    /** �����ļ��� */
    virtual BOOL DownloadFile( LPCTSTR strUrl, LPCTSTR strLocFilePath ) = 0;

	/** ����http���� */
	virtual BOOL SetHttpProxy( BOOL bUseProxy,
		LPCTSTR strIp, 
		uint16 strPort, 
		LPCTSTR strUser, 
		LPCTSTR strPassword ) = 0;

    /** ������ԡ� 
    *   ����ץȡĿ����ҳ���ԡ�
    */
    virtual BOOL TestNetwork() = 0;
};

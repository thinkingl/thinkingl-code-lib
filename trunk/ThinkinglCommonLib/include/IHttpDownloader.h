#pragma once

#include "portabledefine.h"
#include "MimeType.h"

class IHttpDownloader
{
public:
    virtual ~IHttpDownloader(){};




	/** ��Url*/
	virtual BOOL OpenUrl( LPCTSTR strUrl ) = 0;
	/** ��ȡ�ļ����͡� */
	virtual BOOL GetMimeType( CMimeType& mimetype ) = 0;

    /** �����ļ��� */
    virtual BOOL DownloadFile( LPCTSTR strLocFilePath ) = 0;
	

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

#pragma once

#include "portabledefine.h"

class IHttpFecher
{
public:

    /** �����ļ��� */
    virtual BOOL FecheFile( LPCTSTR strUrl, LPCTSTR strLocFilePath ) = 0;

	/** ����http���� */
	virtual BOOL SetHttpProxy( BOOL bUseProxy,
		LPCTSTR strIp, 
		uint16 strPort, 
		LPCTSTR strUser, 
		LPCTSTR strPassword ) = 0;
};

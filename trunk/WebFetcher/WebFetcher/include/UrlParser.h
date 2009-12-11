#pragma once

#include "portabledefine.h"

class CUrlParser
{
public:
	CUrlParser(void);
	CUrlParser( LPCTSTR strUrl );
	virtual ~CUrlParser(void);


	/** ��ȡ�������� */
	BOOL GetSubDomain( tstring& strSubDomain );

	/** ��ȡ�����վ��Ŀ¼·���� */
	BOOL GetWebDir( tstring& strWebDir );

	/** ��ȡ�ļ���չ���� */
	BOOL GetFileExt( tstring strExt );

private:
	tstring m_strUrl;
	
};

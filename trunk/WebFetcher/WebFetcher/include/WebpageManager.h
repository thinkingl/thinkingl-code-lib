#pragma once
#include "iwebpagemanager.h"

class CWebpageManager :
	public IWebpageManager
{

protected:
	friend class IWebpageManager;

	CWebpageManager(void);
	virtual ~CWebpageManager(void);

public:
	/** ��ȡһ���ȴ�����Ļ�����ҳ�� */
	virtual BOOL GetCachedPage( tstring& strUrl, tstring& strLocalPath ) ;

	/** Ԥ�Ȼ�ȡһ��URL��Ӧ�Ļ����ļ�·���ͱ����ļ�·���� */
	virtual BOOL GetUrlPagePrePath( LPCTSTR strUrl, tstring& strCachePath, tstring& strSavePath );

	/** ��ȡURL�ڱ��صı���·���� */
	virtual BOOL GetPageLocalFilePath( LPCTSTR strUrl, tstring& strLocalPath ) ;

	/** ����һ�������url��¼�� */
	virtual BOOL CachePageUrl( LPCTSTR strUrl, LPCTSTR strLocalPath ) ;

	/** ����url��Ϊ����õ���ҳ�� */
	virtual BOOL CachedPageToSavedPage( LPCTSTR strUrl ) ;

	/** ��¼һ����ȡʧ�ܵ���ҳ�� 
	*	�ڻ�ȡ strBaseUrl ʱ���޷���ȡ�� strFailUrl��
	*/
	virtual BOOL AddFailUrl( LPCTSTR strBaseUrl, LPCTSTR strFailUrl ) ;

private:

};

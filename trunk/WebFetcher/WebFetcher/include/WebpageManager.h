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

	

	/** ��ȡURL�ڱ��صı���·���� */
	virtual BOOL GetLocalPath( LPCTSTR strUrl, tstring& strLocalPath ) ;

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

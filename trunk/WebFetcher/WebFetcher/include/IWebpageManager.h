#pragma once

#include "portabledefine.h"

/** url table*/

class IWebpageManager
{
public:
	static IWebpageManager *Instance();
	static void Release();
private:
	static IWebpageManager *s_instance;
public:
	enum EUrlState
	{
		UrlStateSaved,		// �Ѿ����档
		UrlStateCached,		// �Ѿ������档
		UrlStateFeching,	// ���ڻ�ȡ��
	};

	/** ��ȡһ���ȴ�����Ļ�����ҳ�� */
	virtual BOOL GetCachedPage( tstring& strUrl, tstring& strLocalPath ) = 0;

	/** ��ȡһ�������ļ��������ڻ���ҳ�档 */

	/** ��ȡURL�ڱ��صı���·���� */
	virtual BOOL GetLocalPath( LPCTSTR strUrl, tstring& strLocalPath ) = 0;

	/** ����һ�������url��¼�� */
	virtual BOOL CachePageUrl( LPCTSTR strUrl, LPCTSTR strLocalPath ) = 0;

	/** ����url��Ϊ����õ���ҳ�� */
	virtual BOOL CachedPageToSavedPage( LPCTSTR strUrl ) = 0;

	/** ��¼һ����ȡʧ�ܵ���ҳ�� 
	*	�ڻ�ȡ strBaseUrl ʱ���޷���ȡ�� strFailUrl��
	*/
	virtual BOOL AddFailUrl( LPCTSTR strBaseUrl, LPCTSTR strFailUrl ) = 0;
};

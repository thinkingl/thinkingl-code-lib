#pragma once

#include "portabledefine.h"

/** url table*/

class IUrlTable
{
public:
	enum EUrlState
	{
		UrlStateSaved,		// �Ѿ����档
		UrlStateCached,		// �Ѿ������档
		UrlStateFeching,	// ���ڻ�ȡ��
	};

	/** ��ȡURL�ڱ��صı���·���� */
	virtual BOOL GetLocalPath( LPCTSTR strUrl, tstring& strLocalPath ) = 0;

	/** ����һ�������url��¼�� */
	virtual BOOL CachePageUrl( LPCTSTR strUrl, LPCTSTR strLocalPath ) = 0;

	/** ����url��Ϊ����õ���ҳ�� */
	virtual BOOL SavePageUrl( LPCTSTR strUrl, LPCTSTR strLocalPath ) = 0;

	/** ����һ�������url������״̬��Ϊfeching�� */
	virtual BOOL FechingCachedUrl( tstring& strUrl ) = 0;

	/** ��¼һ����ȡʧ�ܵ���ҳ�� 
	*	�ڻ�ȡ strBaseUrl ʱ���޷���ȡ�� strFailUrl��
	*/
	virtual BOOL AddFailUrl( LPCTSTR strBaseUrl, LPCTSTR strFailUrl ) = 0;
};

#pragma once

#include "portabledefine.h"

/** url table*/

class IUrlTable
{
public:
	enum EUrlState
	{
		UrlStateSaved,		// 已经保存。
		UrlStateCached,		// 已经被缓存。
		UrlStateFeching,	// 正在获取。
	};

	/** 获取URL在本地的保存路径。 */
	virtual BOOL GetLocalPath( LPCTSTR strUrl, tstring& strLocalPath ) = 0;

	/** 增加一个缓存的url记录。 */
	virtual BOOL CachePageUrl( LPCTSTR strUrl, LPCTSTR strLocalPath ) = 0;

	/** 缓存url变为保存好的网页。 */
	virtual BOOL SavePageUrl( LPCTSTR strUrl, LPCTSTR strLocalPath ) = 0;

	/** 处理一个缓存的url。它的状态变为feching。 */
	virtual BOOL FechingCachedUrl( tstring& strUrl ) = 0;

	/** 记录一个获取失败的网页。 
	*	在获取 strBaseUrl 时，无法获取到 strFailUrl。
	*/
	virtual BOOL AddFailUrl( LPCTSTR strBaseUrl, LPCTSTR strFailUrl ) = 0;
};

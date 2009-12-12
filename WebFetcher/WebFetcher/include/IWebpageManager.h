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
	//enum EUrlState
	//{
	//	UrlStateSaved,		// 已经保存。
	//	UrlStateCached,		// 已经被缓存。
	//	UrlStateFeching,	// 正在获取。
	//};

	/** 获取一个等待处理的缓存网页。 */
	virtual BOOL GetCachedPage( tstring& strUrl, tstring& strLocalPath ) = 0;

	/** 预先获取一个URL对应的缓存文件路径和保存文件路径。 */
	virtual BOOL GetUrlPagePrePath( LPCTSTR strUrl, tstring& strCachePath, tstring& strSavePath ) = 0;

	/** 获取URL在本地的保存路径。 */
	virtual BOOL GetPageLocalFilePath( LPCTSTR strUrl, tstring& strLocalPath ) = 0;

	/** 增加一个缓存的url记录。 */
	virtual BOOL CachePageUrl( LPCTSTR strUrl, LPCTSTR strLocalPath ) = 0;

	/** 缓存url变为保存好的网页。 */
	virtual BOOL CachedPageToSavedPage( LPCTSTR strUrl ) = 0;

	/** 记录一个获取失败的网页。 
	*	在获取 strBaseUrl 时，无法获取到 strFailUrl。
	*/
	virtual BOOL AddFailUrl( LPCTSTR strBaseUrl, LPCTSTR strFailUrl ) = 0;
};

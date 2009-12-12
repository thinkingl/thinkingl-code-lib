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
	/** 获取一个等待处理的缓存网页。 */
	virtual BOOL GetCachedPage( tstring& strUrl, tstring& strLocalPath ) ;

	

	/** 获取URL在本地的保存路径。 */
	virtual BOOL GetLocalPath( LPCTSTR strUrl, tstring& strLocalPath ) ;

	/** 增加一个缓存的url记录。 */
	virtual BOOL CachePageUrl( LPCTSTR strUrl, LPCTSTR strLocalPath ) ;

	/** 缓存url变为保存好的网页。 */
	virtual BOOL CachedPageToSavedPage( LPCTSTR strUrl ) ;

	/** 记录一个获取失败的网页。 
	*	在获取 strBaseUrl 时，无法获取到 strFailUrl。
	*/
	virtual BOOL AddFailUrl( LPCTSTR strBaseUrl, LPCTSTR strFailUrl ) ;

private:

};

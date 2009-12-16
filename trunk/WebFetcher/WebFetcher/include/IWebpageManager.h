#pragma once

#include "portabledefine.h"
#include "IHttpDownloader.h"

/** url table*/

class IWebpageManager
{
public:
	static IWebpageManager *Instance();
	static void Release();
private:
	static IWebpageManager *s_instance;
protected:
    virtual ~IWebpageManager(){};
public:

	virtual BOOL Init() = 0;

	/** 获取一个等待处理的缓存网页。 */
	virtual BOOL GetCachedPage( tstring& strUrl, tstring& strLocalPath ) = 0;

	/** 预先获取一个URL对应的缓存文件路径和保存文件路径。
	*	如果存在，返回已经存在的。
	*/
	virtual BOOL PreAllocateFilePath( LPCTSTR strUrl, 
		IHttpDownloader::EMimeType eFileType,
		tstring& strCachePath, 
		tstring& strSavePath ) = 0;

	/** 获取URL在本地的保存路径。 */
	virtual BOOL GetPageLocalFilePath( LPCTSTR strUrl, tstring& strLocalPath ) = 0;

	/** 增加一个缓存的url记录。 */
	virtual BOOL CachePageUrl( LPCTSTR strUrl ) = 0;

	/** 缓存url变为保存好的网页。 */
	virtual BOOL CachedPageToSavedPage( LPCTSTR strUrl ) = 0;

	/** 记录一个获取失败的网页。 
	*	在获取 strBaseUrl 时，无法获取到 strFailUrl。
	*/
	virtual BOOL AddFailUrl( LPCTSTR strBaseUrl, LPCTSTR strFailUrl ) = 0;
};

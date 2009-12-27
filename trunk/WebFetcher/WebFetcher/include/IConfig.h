#pragma once
#include "portabledefine.h"
#include "IMutex.h"
class IConfig
{
public:
	static IConfig *Instance();
	static void Release();
private:
	static IConfig *s_instance;

	static IMutex *s_SingletonLock;

public:
	// 获取根目录。
	virtual tstring GetRootFolder() = 0;

	virtual tstring GetRootUrl() = 0;

    virtual BOOL IsUseProxy() = 0;

    virtual BOOL IsProxyAuthentication() = 0;

	virtual tstring GetProxyUserName() = 0;

	virtual tstring GetProxyPassword() = 0;

	virtual tstring GetProxyIp() = 0;
	
	virtual uint16 GetProxyPort() = 0;

	virtual uint32 GetThreadCount() = 0;

	/** 获取所有的URL Filter。 
	*	获取到的所有网站(或目录)都是需要下载页面的网站.
	*/
	virtual tstringarray GetAllFetchFilter() = 0;

	/** 获取所有被屏蔽的网站.
	*	被屏蔽的网站不去下载.
	*/
	virtual tstringarray GetAllBanUrl() = 0;

	/** Url 是不是被屏蔽了。 */
	virtual BOOL IsUrlBan( LPCTSTR strUrl ) = 0;

	/** 这个UrL filter项是否要获取。 */
	virtual BOOL IsUrlFilterFetch( LPCTSTR strFilter ) = 0;

protected:
	IConfig(void);
	virtual ~IConfig(void);
};

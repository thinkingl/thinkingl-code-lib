#pragma once

#include "portabledefine.h"
#include "IConfig.h"
#include "IniConfigFile.h"

class CIniConfig : public IConfig
{
    
public:
	// 获取根目录。
	virtual tstring GetRootFolder() ;

	virtual tstring GetRootUrl() ;

    virtual BOOL IsUseProxy();

    virtual BOOL IsProxyAuthentication();

	virtual tstring GetProxyUserName() ;

	virtual tstring GetProxyPassword();

	virtual tstring GetProxyIp() ;

	virtual uint16 GetProxyPort() ;

	virtual uint32 GetThreadCount() ;

	/** 获取所有的URL Filter。 
	*	获取到的所有网站(或目录)都是需要下载页面的网站.
	*/
	virtual tstringarray GetAllFetchFilter() ;

	/** 获取所有被屏蔽的网站.
	*	被屏蔽的网站不去下载.
	*/
	virtual tstringarray GetAllBanUrl();

	/** Url 是不是被屏蔽了。 */
	virtual BOOL IsUrlBan( LPCTSTR strUrl );

	/** 这个UrL filter项是否要获取。 */
	virtual BOOL IsUrlFilterFetch( LPCTSTR strFilter );

public:
	friend class IConfig;
protected:
    CIniConfig(void);
    virtual ~CIniConfig(void);

private:
	/**  */
	BOOL Init();



private:
	CIniConfigFile m_iniFile;
};

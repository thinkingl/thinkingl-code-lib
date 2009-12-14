#pragma once

#include "portabledefine.h"
#include "IConfig.h"

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

	/** 获取所有的URL Filter。 */
	virtual tstringarray GetAllFetchFilter();

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

	tstring ReadConfigStr(  LPCTSTR strApp, LPCTSTR strKey );
	int32 ReadConfigInt( LPCTSTR strApp, LPCTSTR strKey );

	tstringarray ReadKeys( LPCTSTR strApp );

private:
	tstring m_strIniFilePath;
};

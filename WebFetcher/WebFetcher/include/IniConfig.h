#pragma once

#include "portabledefine.h"
#include "IConfig.h"

class CIniConfig : public IConfig
{
    
public:
	// 获取根目录。
	virtual tstring GetRootFolder() ;

	virtual tstring GetRootUrl() ;

	virtual tstring GetProxyUserName() ;

	virtual tstring GetProxyPassword();

	virtual tstring GetProxyIp() ;

	virtual uint16 GetProxyPort() ;
public:
	friend class IConfig;
protected:
    CIniConfig(void);
    virtual ~CIniConfig(void);

private:
	/**  */
	BOOL Init();

	tstring ReadConfigStr( LPCTSTR strName );
	int32 ReadConfigInt( LPCTSTR strName );

private:
	tstring m_strIniFilePath;
};

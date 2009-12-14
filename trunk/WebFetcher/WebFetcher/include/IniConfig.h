#pragma once

#include "portabledefine.h"
#include "IConfig.h"

class CIniConfig : public IConfig
{
    
public:
	// ��ȡ��Ŀ¼��
	virtual tstring GetRootFolder() ;

	virtual tstring GetRootUrl() ;

    virtual BOOL IsUseProxy();

    virtual BOOL IsProxyAuthentication();

	virtual tstring GetProxyUserName() ;

	virtual tstring GetProxyPassword();

	virtual tstring GetProxyIp() ;

	virtual uint16 GetProxyPort() ;

	virtual uint32 GetThreadCount() ;

	/** ��ȡ���е�URL Filter�� */
	virtual tstringarray GetAllFetchFilter();

	/** ���UrL filter���Ƿ�Ҫ��ȡ�� */
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

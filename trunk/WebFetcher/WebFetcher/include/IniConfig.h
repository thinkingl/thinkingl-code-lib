#pragma once

#include "portabledefine.h"
#include "IConfig.h"
#include "IniConfigFile.h"

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

	/** ��ȡ���е�URL Filter�� 
	*	��ȡ����������վ(��Ŀ¼)������Ҫ����ҳ�����վ.
	*/
	virtual tstringarray GetAllFetchFilter() ;

	/** ��ȡ���б����ε���վ.
	*	�����ε���վ��ȥ����.
	*/
	virtual tstringarray GetAllBanUrl();

	/** Url �ǲ��Ǳ������ˡ� */
	virtual BOOL IsUrlBan( LPCTSTR strUrl );

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



private:
	CIniConfigFile m_iniFile;
};

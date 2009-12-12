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
	// ��ȡ��Ŀ¼��
	virtual tstring GetRootFolder() = 0;

	virtual tstring GetRootUrl() = 0;

	virtual tstring GetProxyUserName() = 0;

	virtual tstring GetProxyPassword() = 0;

	virtual tstring GetProxyIp() = 0;
	
	virtual uint16 GetProxyPort() = 0;

	virtual uint32 GetThreadCount() = 0;

	/** ��ȡ���е�URL Filter�� */
	virtual tstringarray GetAllFetchFilter() = 0;

	/** ���UrL filter���Ƿ�Ҫ��ȡ�� */
	virtual BOOL IsUrlFilterFetch( LPCTSTR strFilter ) = 0;

protected:
	IConfig(void);
	virtual ~IConfig(void);
};

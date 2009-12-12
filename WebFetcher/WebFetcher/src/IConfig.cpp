#include "IConfig.h"
#include "ScopeLock.h"
#include "Classfactory.h"
#include "IniConfig.h"

IConfig::IConfig(void)
{
}

IConfig::~IConfig(void)
{
}

IConfig *IConfig::s_instance = NULL;
IMutex *IConfig::s_SingletonLock = CClassFactory::CreateMutex();

IConfig *IConfig::Instance()
{
	if ( s_instance == NULL )
	{
		SCOPE_LOCK( *s_SingletonLock );
		if ( s_instance == NULL )
		{
			s_instance = new CIniConfig();
		}
	}
	return s_instance;
}

void IConfig::Release()
{
	if ( s_instance )
	{
		SCOPE_LOCK( *s_SingletonLock );
		if ( s_instance )
		{
			delete s_instance;
			s_instance = NULL;
		}
	}
}

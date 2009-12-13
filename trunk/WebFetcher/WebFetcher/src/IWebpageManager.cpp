#include "IWebpageManager.h"
#include "WebpageManager.h"
#include "Log.h"

IWebpageManager *IWebpageManager::s_instance = NULL;

IWebpageManager * IWebpageManager::Instance()
{
	if ( s_instance == NULL )
	{
		s_instance = new CWebpageManager();
		BOOL bInit = s_instance->Init();
		CLog() << _T( "Init WebpageManager : " ) << bInit << endl;
		ASSERT( bInit );
	}
	return s_instance;
}

void IWebpageManager::Release()
{
	if ( s_instance )
	{
		delete s_instance;
		s_instance = NULL;
	}
}



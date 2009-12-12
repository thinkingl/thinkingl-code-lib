#include "IWebpageManager.h"
#include "WebpageManager.h"

IWebpageManager *IWebpageManager::s_instance = NULL;

IWebpageManager * IWebpageManager::Instance()
{
	if ( s_instance == NULL )
	{
		s_instance = new CWebpageManager();
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



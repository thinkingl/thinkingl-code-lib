#include "Win32PageFetchThread.h"
#include "Log.h"
#include "ClassFactory.h"

CWin32PageFetchThread::CWin32PageFetchThread(void)
{
	this->m_pPageFecher = NULL;
}

CWin32PageFetchThread::~CWin32PageFetchThread(void)
{
}

int CWin32PageFetchThread::DoPieceWork( BOOL& bExit )
{
	bExit = FALSE;

	if ( m_pPageFecher == NULL )
	{
		m_pPageFecher = CClassFactory::CreatePageFetcher();
	}

	if ( m_pPageFecher->HasPageWaiting() )
	{
		m_pPageFecher->FetchOnePage();
	}
	else
	{
		bExit = TRUE;
		return 0;
	}



	return 0;
}
#include "Win32Thread.h"

CWin32Thread::CWin32Thread(void)
{
	this->m_hThread = NULL;
	this->m_bRun = FALSE;

	// 默认一分钟。
	this->m_nTimeoutMilliseconds = 60 * 1000;
}

CWin32Thread::~CWin32Thread(void)
{
}

BOOL CWin32Thread::Start()
{
	if ( m_hThread )
	{
		CLog() << _T( "Thread Already Run!!!!!!" ) << endl;
		ASSERT( FALSE );
		return TRUE;
	}
	this->m_bRun = TRUE;
	DWORD dwThreadId;
	this->m_hThread = ::CreateThread( NULL, NULL, CWin32Thread::FunThreadS, this, NULL, &dwThreadId );
	if ( m_hThread == NULL )
	{
		CLog() << _T( "Thread Create fail!!! er: " ) << ::GetLastError() << endl;
	}
	else
	{
		CLog() << _T( "Create Thread! handle: " )<< m_hThread << _T( "ThreadId" ) << dwThreadId << endl;
	}
	return NULL != m_hThread;
}

BOOL CWin32Thread::Stop()
{
	if ( m_hThread )
	{
		this->SetStopFlag();

		// 等待。
		DWORD dwRet = ::WaitForSingleObject( m_hThread, this->m_nTimeoutMilliseconds );
		if ( WAIT_TIMEOUT == dwRet )
		{
			CLog() << _T( "Stop thread Timeout!!!!" ) << endl;
			BOOL bRet = ::TerminateThread( m_hThread, -1 );
			CLog() << _T( "Terminate thread, handle: " ) << m_hThread << _T( " ret: " ) << bRet << endl;
		}
		CloseHandle( m_hThread );
		this->m_hThread = NULL;
	}
	else
	{
		CLog() << _T( "Thread Not Run!!!!!!" ) << endl;
		return FALSE;
	}
	return FALSE;
}

int CWin32Thread::FunThreadS( void * param )
{
	CWin32Thread *pThis = ( CWin32Thread* )param;
	int nExitCode = -1;
	if ( pThis )
	{
		nExitCode = pThis->FunThread();
	}
	return nExitCode;
}


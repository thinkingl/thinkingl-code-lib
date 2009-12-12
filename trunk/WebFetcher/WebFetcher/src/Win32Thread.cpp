#include "Win32Thread.h"
#include "Log.h"
#include "scopelock.h"

CWin32Thread::CWin32Thread(void)
{
	this->m_hThread = NULL;
	this->m_bRun = FALSE;

	// Ĭ��һ���ӡ�
	this->m_nTimeoutMilliseconds = 60 * 1000;
}

CWin32Thread::~CWin32Thread(void)
{
	if ( this->IsRunning() )
	{
		this->Stop();
	}
}

BOOL CWin32Thread::Start()
{
	SCOPE_LOCK( m_threadSafeLock );

	if ( this->IsRunning() )
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
		CLog() << _T( "Create Thread! handle: " )<< m_hThread << _T( " ThreadId " ) << dwThreadId << endl;
	}
	return NULL != m_hThread;
}

BOOL CWin32Thread::Stop()
{
	// ��ʱ����������ֹ�������⡣
//	SCOPE_LOCK( m_threadSafeLock );

	if ( IsRunning() )
	{
		this->SetStopFlag();

		// �ȴ���
		DWORD dwRet = ::WaitForSingleObject( m_hThread, this->m_nTimeoutMilliseconds );
		if ( WAIT_TIMEOUT == dwRet )
		{
			CLog() << _T( "Stop thread Timeout!!!!" ) << endl;
			BOOL bRet = ::TerminateThread( m_hThread, -1 );
			CLog() << _T( "Terminate thread, handle: " ) << m_hThread << _T( " ret: " ) << bRet << endl;
		}
		BOOL bRet = CloseHandle( m_hThread );
		CLog() << _T( "Win32 Thread Close Handle ret: " ) << bRet << endl;
		this->m_hThread = NULL;

		return TRUE;
	}
	else
	{
		CLog() << _T( "Thread Not Run!!!!!!" ) << endl;
		return FALSE;
	}
}

DWORD CWin32Thread::FunThreadS( void * param )
{
	CWin32Thread *pThis = ( CWin32Thread* )param;
	int nExitCode = -1;
	if ( pThis )
	{
		nExitCode = pThis->FunThread();
	}
	return nExitCode;
}

int CWin32Thread::FunThread()
{
	int nExitCode = 0;
	while ( !this->GetStopFlag() )
	{
		BOOL bExit = FALSE;
		nExitCode = this->DoPieceWork( bExit );
		if ( bExit )
		{
			break;
		}
	}
	return nExitCode;
}

void CWin32Thread::SetTimeOut( uint32 nMilliSeconds )
{
	SCOPE_LOCK( m_threadSafeLock );

	this->m_nTimeoutMilliseconds = nMilliSeconds;
}

void CWin32Thread::SetStopFlag()
{
	SCOPE_LOCK( m_threadSafeLock );

	this->m_bRun = FALSE;
}

BOOL CWin32Thread::IsRunning()
{
	SCOPE_LOCK( m_threadSafeLock );
//	ASSERT( FALSE );
	BOOL bResult = FALSE;
	if ( m_hThread )
	{
		DWORD dwExitCode = 0;
		BOOL bRet = ::GetExitCodeThread( m_hThread, &dwExitCode );
		if ( bRet )
		{
			if ( dwExitCode == STILL_ACTIVE )
			{
				bResult = TRUE;
			}
		}
	}
	return bResult;
}

BOOL CWin32Thread::GetStopFlag()
{
	SCOPE_LOCK( m_threadSafeLock );

	return !m_bRun;
}




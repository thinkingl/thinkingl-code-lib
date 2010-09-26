#include "Win32Thread.h"
#include "Log.h"
#include "scopelock.h"



CWin32Thread::CWin32Thread(void)
{
	this->m_hThread = NULL;
	this->m_bRun = FALSE;

	// 默认10分钟。
	this->m_nTimeoutMilliseconds = 600 * 1000;
}

CWin32Thread::~CWin32Thread(void)
{
	if ( this->IsRunning() )
	{
		this->Stop();
	}
}

BOOL CWin32Thread::Start( LPCTSTR lpstrLogFolder /*= NULL*/ )
{
	SCOPE_LOCK( m_threadSafeLock );

	if ( this->IsRunning() )
	{
		Log() << _T( "Thread Already Run!!!!!!" ) << endl;
		ASSERT( FALSE );
		return TRUE;
	}

	// 保存日志路径。
	if( lpstrLogFolder )
	{
		this->m_strLogFolder = lpstrLogFolder;
	}

	this->m_bRun = TRUE;
	DWORD dwThreadId;
	this->m_hThread = ::CreateThread( NULL, NULL, CWin32Thread::FunThreadS, this, NULL, &dwThreadId );
	if ( m_hThread == NULL )
	{
		Log() << _T( "Thread Create fail!!! er: " ) << ::GetLastError() << endl;
	}
	else
	{
		Log() << _T( "Create Thread! handle: " )<< m_hThread << _T( " ThreadId " ) << dwThreadId << endl;
	}
	return NULL != m_hThread;
}

BOOL CWin32Thread::Stop()
{
	// 暂时不加锁，防止死锁问题。
//	SCOPE_LOCK( m_threadSafeLock );

	if ( IsRunning() )
	{
		this->SetStopFlag();

		// 等待。
		DWORD dwRet = ::WaitForSingleObject( m_hThread, this->m_nTimeoutMilliseconds );
		if ( WAIT_TIMEOUT == dwRet )
		{
			Log() << _T( "Stop thread Timeout!!!!" ) << endl;
			BOOL bRet = ::TerminateThread( m_hThread, -1 );
			Log() << _T( "Terminate thread, handle: " ) << m_hThread << _T( " ret: " ) << bRet << endl;
		}
		BOOL bRet = CloseHandle( m_hThread );
		Log() << _T( "Win32 Thread Close Handle ret: " ) << bRet << endl;
		this->m_hThread = NULL;

		return TRUE;
	}
	else
	{
		Log() << _T( "Thread Not Run!!!!!!" ) << endl;
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
	// 初始化日志。
	tstring strDir = this->m_strLogFolder;

	strDir += _T( "log\\" );
	uint32 uThreadId = ::GetCurrentThreadId();
	tstringstream ssThreadId;
	ssThreadId << _T( "thread" ) << uThreadId;
	Log().SetLogFileDir( strDir.c_str(), ssThreadId.str().c_str() );

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

tstring CWin32Thread::GetLogFolder() const
{
	SCOPE_LOCK( m_threadSafeLock );

	return this->m_strLogFolder;
}



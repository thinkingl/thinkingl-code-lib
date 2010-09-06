#include "LogManager.h"
#include "ScopeLock.h"
#include "ClassFactory.h"

CLogManager *CLogManager::s_instance = NULL;

static IMutex *s_mutexSingleton = CThinkinglClassFactory::CreateMutex();

CLogManager::CLogManager(void)
{
	m_pThreadSafeLock = CThinkinglClassFactory::CreateMutex();
}

CLogManager::~CLogManager(void)
{
	TThreadLogTable::iterator iter = this->m_tThreadLogTable.begin();
	for ( ; iter != this->m_tThreadLogTable.end(); iter++ )
	{
		if ( iter->second )
		{
			delete iter->second;
			iter->second = NULL;
		}
	}
}

CLogManager *CLogManager::Instance()
{
	if ( s_instance == NULL )
	{
		SCOPE_LOCK( *s_mutexSingleton );
		if ( s_instance == NULL )
		{
			s_instance = new CLogManager();
		}
	}
	return s_instance;
}

void CLogManager::Release()
{
	if ( s_instance )
	{
		SCOPE_LOCK( *s_mutexSingleton );
		if ( s_instance )
		{
			delete s_instance;
			s_instance = NULL;
		}
	}
}

CLog *CLogManager::GetLog( uint32 nThreadId )
{
	SCOPE_LOCK( *m_pThreadSafeLock );

	TThreadLogTable::iterator iter = this->m_tThreadLogTable.find( nThreadId );
	if ( iter == m_tThreadLogTable.end() )
	{
		CLog *pNewLog = new CLog();
		this->m_tThreadLogTable[ nThreadId ] = pNewLog;

		return pNewLog;
	}
	else
	{
		return iter->second;
	}
	

}

CLog *CLogManager::GetCurThreadLog()
{
	SCOPE_LOCK( *m_pThreadSafeLock );

	uint32 uThreadId = ::GetCurrentThreadId();
	CLog *pLog = this->GetLog( uThreadId );

	return pLog;
}



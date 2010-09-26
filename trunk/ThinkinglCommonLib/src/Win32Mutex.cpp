#include "Win32Mutex.h"

CWin32Mutex::CWin32Mutex(void)
{
}

CWin32Mutex::~CWin32Mutex(void)
{
}

BOOL CWin32Mutex::Lock( const uint32 dwTimeOut /* = INFINITE */ )
{
	return this->m_mutex.Lock( dwTimeOut );
}

BOOL CWin32Mutex::Unlock()
{
	return this->m_mutex.Unlock();
}
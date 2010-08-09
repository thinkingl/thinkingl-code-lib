#include "ScopeLock.h"

CScopeLock::CScopeLock( IMutex& mutex )
{
	this->m_pMutex = &mutex;

	m_pMutex->Lock();
}

CScopeLock::~CScopeLock(void)
{
	m_pMutex->Unlock();
}



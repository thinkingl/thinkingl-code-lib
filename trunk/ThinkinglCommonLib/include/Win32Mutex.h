#pragma once
#include "imutex.h"
#include <afxmt.h>

class CWin32Mutex :
	public IMutex
{
public:
	CWin32Mutex(void);
	virtual ~CWin32Mutex(void);

	virtual BOOL Lock( const uint32 dwTimeOut = INFINITE );
	virtual BOOL Unlock();
private:
	CMutex m_mutex;
};

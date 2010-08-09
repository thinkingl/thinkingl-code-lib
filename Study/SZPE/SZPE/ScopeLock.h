#pragma once

#include "IMutex.h"

#define SCOPE_LOCK( x ) CScopeLock lock_wleonb503012l3jgnonvdo230( x ) 


class CScopeLock
{
public:
	CScopeLock( IMutex& mutex );
	virtual ~CScopeLock(void);

private:
	IMutex *m_pMutex;
};

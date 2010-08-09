#pragma once
#include "portabledef.h"
#include "INoCopy.h"


#ifndef INFINITE
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif

class IMutex : public INoCopy
{
public:	
    virtual ~IMutex(){};

	virtual BOOL Lock( const uint32 dwTimeOut = INFINITE ) = 0;
	virtual BOOL Unlock() = 0;
};

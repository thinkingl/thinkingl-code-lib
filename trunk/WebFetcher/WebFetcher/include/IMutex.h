#pragma once
#include "portabledefine.h"
#include "INoCopy.h"


#ifndef INFINITE
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif

class IMutex : public INoCopy
{
public:	

	virtual BOOL Lock( const uint32 dwTimeOut = INFINITE ) = 0;
	virtual BOOL Unlock() = 0;
};

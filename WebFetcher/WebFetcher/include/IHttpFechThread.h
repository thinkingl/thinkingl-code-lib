#pragma once

#include "portabledefine.h"

class IHttpFechThread
{
public:
	
	virtual BOOL Start() = 0;

	virtual BOOL Stop() = 0;


};

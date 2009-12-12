#pragma once

#include "portabledefine.h"

class IDatabase
{
public:
	IDatabase(void);
	virtual ~IDatabase(void);

	virtual BOOL CreateTable() = 0;


};

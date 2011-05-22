#pragma once
#include "idatabaseinterface.h"
class CDatabaseSqlite :
	public IDatabaseInterface
{
public:
	CDatabaseSqlite(void);
	virtual ~CDatabaseSqlite(void);


private:
};


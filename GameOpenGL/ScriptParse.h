#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include "Control.h"

class COrder
{
public:
	EOrderCmd m_cmd;
	std::string m_param;
	int m_time;
};
typedef std::vector< COrder > COrderList;

class CScriptParse
{
public:
	CScriptParse(void);
	~CScriptParse(void);

	COrderList GetOrderList( int curTickCount );
	void Reset();
private:
	int m_finishedTickcount;

	DWORD m_startTickCount;
private:
	std::ifstream m_scriptFile;
};


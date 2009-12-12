#pragma once
#include "win32thread.h"

class CPageFetchThread :
	public CWin32Thread
{
public:
	CPageFetchThread(void);
	virtual ~CPageFetchThread(void);

	virtual int DoPieceWork();
};

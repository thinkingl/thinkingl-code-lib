#pragma once

#include "Win32Thread.h"

class CMapDownloadThread :
	public CWin32Thread
{
public:
	CMapDownloadThread(void);
	virtual ~CMapDownloadThread(void);

	/** ���ûص�, ��Ҫ����. */
};


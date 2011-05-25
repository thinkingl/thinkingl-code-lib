#pragma once

#include "Win32Thread.h"

class CMapDownloadThread :
	public CWin32Thread
{
public:
	CMapDownloadThread(void);
	virtual ~CMapDownloadThread(void);

	/** 设置回调, 索要任务. */
};


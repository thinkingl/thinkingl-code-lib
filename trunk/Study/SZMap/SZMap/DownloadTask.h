#pragma once
#include "itask.h"

/** 下载任务. */

class CDownloadTask :
	public ITask
{
public:
	CDownloadTask(void);
	virtual ~CDownloadTask(void);

	/** 完成这个任务. */
	virtual bool Do();

private:
};


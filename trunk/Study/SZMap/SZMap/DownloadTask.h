#pragma once
#include "itask.h"

/** ��������. */

class CDownloadTask :
	public ITask
{
public:
	CDownloadTask(void);
	virtual ~CDownloadTask(void);

	/** ����������. */
	virtual bool Do();

private:
};


#pragma once

#include "portabledefine.h"
/** 任务基类, 用于派发任务给多个线程时方便. */

class ITask
{
public:
	ITask(void);
	virtual ~ITask(void);

	/** 完成工作. */
	virtual BOOL Do() = 0;

};


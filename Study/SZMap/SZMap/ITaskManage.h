#pragma once

#include "ITask.h"

/** 任务管理的接口类. 
*	任务管理.
*
*/

class ITaskManage
{
public:
	ITaskManage(void);
	virtual ~ITaskManage(void);

	/** 获得/请求分配一个新的任务. */
	virtual ITask * GetTask() = 0;

	/** 任务完成通知. */
	virtual void TaskFinishNotify( ITask* pTask ) = 0;

};


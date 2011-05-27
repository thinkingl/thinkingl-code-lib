#pragma once

#include "ITask.h"

/** �������Ľӿ���. 
*	�������.
*
*/

class ITaskManage
{
public:
	ITaskManage(void);
	virtual ~ITaskManage(void);

	/** ���/�������һ���µ�����. */
	virtual ITask * GetTask() = 0;

	/** �������֪ͨ. */
	virtual void TaskFinishNotify( ITask* pTask ) = 0;

};


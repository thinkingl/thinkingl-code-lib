#pragma once

#include "Win32Thread.h"
#include "ITaskManage.h"
/** 完成任务的线程.  */

#ifdef WIN32
typedef CWin32Thread COsIndependThread;
#endif

class CTaskThread :
	public COsIndependThread
{
public:
	CTaskThread( ITaskManage* pTaskManager );
	virtual ~CTaskThread(void);

protected:
	/** 工作函数，线程调用，子类实现完成工作。
	*	必须限制函数时间，要可以在短时间内返回。
	*	否则无法停止函数。
	*	如果子类将bExit置为TRUE，退出线程。
	*/
	virtual int DoPieceWork( BOOL& bExit );

private:
	ITaskManage *m_pTaskManager;
};


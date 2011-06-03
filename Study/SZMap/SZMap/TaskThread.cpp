#include "TaskThread.h"
#include "Common.h"

CTaskThread::CTaskThread( ITaskManage* pTaskManager )
{
	m_pTaskManager = pTaskManager;
}


CTaskThread::~CTaskThread(void)
{
}

int CTaskThread::DoPieceWork( BOOL& bExit )
{
	ITask *pTask = m_pTaskManager->GetTask();
	if ( pTask )
	{
		BOOL bRet = pTask->Do();

		// 任务完成.
		m_pTaskManager->TaskFinishNotify( pTask );
	}
	else
	{
		CCommon::Sleep( 1 );
	}

	return 0;
}
// end of file


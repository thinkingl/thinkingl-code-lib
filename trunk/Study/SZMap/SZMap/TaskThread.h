#pragma once

#include "Win32Thread.h"
#include "ITaskManage.h"
/** ���������߳�.  */

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
	/** �����������̵߳��ã�����ʵ����ɹ�����
	*	�������ƺ���ʱ�䣬Ҫ�����ڶ�ʱ���ڷ��ء�
	*	�����޷�ֹͣ������
	*	������ཫbExit��ΪTRUE���˳��̡߳�
	*/
	virtual int DoPieceWork( BOOL& bExit );

private:
	ITaskManage *m_pTaskManager;
};


#pragma once
#include "win32thread.h"
#include "Win32Mutex.h"
#include "IPageFecher.h"


class CWin32PageFetchThread :
	public CWin32Thread
{
public:
	CWin32PageFetchThread(void);
	virtual ~CWin32PageFetchThread(void);

	/** �����������̵߳��ã�����ʵ����ɹ�����
	*	�������ƺ���ʱ�䣬Ҫ�����ڶ�ʱ���ڷ��ء�
	*	�����޷�ֹͣ������
	*	������ཫbExit��ΪTRUE���˳��̡߳�
	*/
	virtual int DoPieceWork( BOOL& bExit );

private:
//	CWin32Mutex m_threadSafeLock;
	IPageFecher *m_pPageFecher;
};

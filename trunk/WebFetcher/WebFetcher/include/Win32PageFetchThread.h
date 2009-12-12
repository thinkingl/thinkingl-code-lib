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

	/** 工作函数，线程调用，子类实现完成工作。
	*	必须限制函数时间，要可以在短时间内返回。
	*	否则无法停止函数。
	*	如果子类将bExit置为TRUE，退出线程。
	*/
	virtual int DoPieceWork( BOOL& bExit );

private:
//	CWin32Mutex m_threadSafeLock;
	IPageFecher *m_pPageFecher;
};

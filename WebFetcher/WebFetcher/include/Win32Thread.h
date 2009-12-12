#pragma once

#include "IThread.h"

class CWin32Thread :
	public IThread
{
public:
	CWin32Thread(void);
	virtual ~CWin32Thread(void);

	/** 停止线程时的超时时间。 */
	virtual void SetTimeOut( uint32 nMilliSeconds ) ;

	/** 运行。 */
	virtual BOOL Start() ;
	/** 同步等待停止。*/
	virtual BOOL Stop() ;

	/** 只设置停止标志。 */
	virtual void SetStopFlag() ;

	virtual BOOL IsRunning() ;

protected:
	virtual BOOL IsStopCmd();
private:
	static int FunThreadS( void * param );

	int FunThread();

	HANDLE m_hThread;

	volatile BOOL m_bRun;

	uint32 m_nTimeoutMilliseconds;

};

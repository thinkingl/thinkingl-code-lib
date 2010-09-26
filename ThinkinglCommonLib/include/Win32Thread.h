#pragma once

#include "IThread.h"
#include "Win32Mutex.h"

class CWin32Thread :
	public IThread
{
public:
	CWin32Thread(void);
	virtual ~CWin32Thread(void);

	/** 停止线程时的超时时间。 */
	virtual void SetTimeOut( uint32 nMilliSeconds ) ;

	/** 运行。 
	*	LPCTSTR lpstrLogFolder : 线程日志路径，如果为NULL，则使用程序的当前目录。
	*/
	virtual BOOL Start( LPCTSTR lpstrLogFolder = NULL ) ;
	/** 同步等待停止。*/
	virtual BOOL Stop() ;

	/** 只设置停止标志。 */
	virtual void SetStopFlag() ;

	virtual BOOL IsRunning() ;

protected:
	virtual BOOL GetStopFlag();
private:
	/** 获取日志目录。 */
	tstring GetLogFolder() const;

	static DWORD WINAPI FunThreadS( void * param );

	int FunThread();

private:
	HANDLE m_hThread;

	volatile BOOL m_bRun;

	uint32 m_nTimeoutMilliseconds;

	mutable CWin32Mutex m_threadSafeLock;

	/** 日志文件夹路径。 */
	tstring m_strLogFolder;
};

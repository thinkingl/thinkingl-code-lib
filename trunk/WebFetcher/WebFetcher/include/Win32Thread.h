#pragma once

#include "IThread.h"
#include "Win32Mutex.h"

class CWin32Thread :
	public IThread
{
public:
	CWin32Thread(void);
	virtual ~CWin32Thread(void);

	/** ֹͣ�߳�ʱ�ĳ�ʱʱ�䡣 */
	virtual void SetTimeOut( uint32 nMilliSeconds ) ;

	/** ���С� 
	*	LPCTSTR lpstrLogFolder : �߳���־·�������ΪNULL����ʹ�ó���ĵ�ǰĿ¼��
	*/
	virtual BOOL Start( LPCTSTR lpstrLogFolder = NULL ) ;
	/** ͬ���ȴ�ֹͣ��*/
	virtual BOOL Stop() ;

	/** ֻ����ֹͣ��־�� */
	virtual void SetStopFlag() ;

	virtual BOOL IsRunning() ;

protected:
	virtual BOOL GetStopFlag();
private:
	/** ��ȡ��־Ŀ¼�� */
	tstring GetLogFolder() const;

	static DWORD WINAPI FunThreadS( void * param );

	int FunThread();

private:
	HANDLE m_hThread;

	volatile BOOL m_bRun;

	uint32 m_nTimeoutMilliseconds;

	mutable CWin32Mutex m_threadSafeLock;

	/** ��־�ļ���·���� */
	tstring m_strLogFolder;
};

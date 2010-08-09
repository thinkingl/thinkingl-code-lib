#pragma once

#include "Process.h"

class CProcessManage
{
public:
	CProcessManage(void);
	virtual ~CProcessManage(void);

public:
	
	/** 开始自动刷新更新。 */
	void StartAutoUpdate( int nMillisecond = 1000 );

	/** 停止自动刷新。 */
	void StopAutoUpdate();

	/** 更新信息。 */
	virtual BOOL Update();

	/** 获取所有进程列表。 */
	void GetAllProcessId( TProcessIdList& tProcessId ) const;

	/** 获取进程信息。 */
	BOOL GetProcess( TProcessId proId, CProcess& process ) const;

	/** 进程操作。 */

private:
	/** 所有进程列表。 */
	typedef std::map< TProcessId, CProcess > TProcessTable;
	TProcessTable m_processesTable;

	/** 刷新线程。 */
	static DWORD WINAPI UpdateThread( LPVOID pThis );

	/** 线程句柄。 */
	HANDLE m_hUpdateThreadHandle;

	/** 线程安全锁。 */
	CCriticalSection m_threadSafeLock;

	/** 是否自动刷新线程状态。 */
	BOOL m_bAutoUpdateProcessState;

	/** 自动刷新间隔时间。 */
	int m_nAutoUpdateElapseMillisec;
};


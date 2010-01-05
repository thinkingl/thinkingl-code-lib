#pragma once

#include "Log.h"

/**
*	主要的问题，不同线程一起写日志会造成日志混乱。这里将日志分开。
*/

class CLogManager
{
public:
	static CLogManager *Instance();
	static void Release();

	typedef std::vector< uint32 > TThreadIdList;
	
	TThreadIdList GetAllThread();

	/** 通过线程id查找对应的日志。 */
	CLog *GetLog( uint32 nThreadId );

	/** 获取当前线程对应的日志类。*/
	CLog *GetCurThreadLog();

protected:
	CLogManager(void);
	virtual ~CLogManager(void);
private:
	static CLogManager *s_instance;

private:
	typedef std::map< uint32, CLog* > TThreadLogTable;

	TThreadLogTable m_tThreadLogTable;

	IMutex *m_pThreadSafeLock;
};

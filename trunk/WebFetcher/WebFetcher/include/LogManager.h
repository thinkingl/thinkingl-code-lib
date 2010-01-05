#pragma once

#include "Log.h"

/**
*	��Ҫ�����⣬��ͬ�߳�һ��д��־�������־���ҡ����ｫ��־�ֿ���
*/

class CLogManager
{
public:
	static CLogManager *Instance();
	static void Release();

	typedef std::vector< uint32 > TThreadIdList;
	
	TThreadIdList GetAllThread();

	/** ͨ���߳�id���Ҷ�Ӧ����־�� */
	CLog *GetLog( uint32 nThreadId );

	/** ��ȡ��ǰ�̶߳�Ӧ����־�ࡣ*/
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

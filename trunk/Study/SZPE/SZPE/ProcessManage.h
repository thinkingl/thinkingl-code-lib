#pragma once

#include "Process.h"

class CProcessManage
{
public:
	CProcessManage(void);
	virtual ~CProcessManage(void);

public:
	
	/** ��ʼ�Զ�ˢ�¸��¡� */
	void StartAutoUpdate( int nMillisecond = 1000 );

	/** ֹͣ�Զ�ˢ�¡� */
	void StopAutoUpdate();

	/** ������Ϣ�� */
	virtual BOOL Update();

	/** ��ȡ���н����б� */
	void GetAllProcessId( TProcessIdList& tProcessId ) const;

	/** ��ȡ������Ϣ�� */
	BOOL GetProcess( TProcessId proId, CProcess& process ) const;

	/** ���̲����� */

private:
	/** ���н����б� */
	typedef std::map< TProcessId, CProcess > TProcessTable;
	TProcessTable m_processesTable;

	/** ˢ���̡߳� */
	static DWORD WINAPI UpdateThread( LPVOID pThis );

	/** �߳̾���� */
	HANDLE m_hUpdateThreadHandle;

	/** �̰߳�ȫ���� */
	CCriticalSection m_threadSafeLock;

	/** �Ƿ��Զ�ˢ���߳�״̬�� */
	BOOL m_bAutoUpdateProcessState;

	/** �Զ�ˢ�¼��ʱ�䡣 */
	int m_nAutoUpdateElapseMillisec;
};


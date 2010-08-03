#pragma once

#include "Process.h"

class CProcessManage
{
public:
	CProcessManage(void);
	virtual ~CProcessManage(void);

public:
	

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

};


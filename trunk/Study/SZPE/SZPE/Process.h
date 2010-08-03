#pragma once

#include <vector>
#include "portabledef.h"

typedef DWORD TProcessId;
typedef std::vector< TProcessId > TProcessIdList;


class CProcess
{
public:
	CProcess( TProcessId processId );
	CProcess();

	virtual ~CProcess(void);

	/** ���̲����� */

	/** ��ȡ������Ϣ�� */

	/** ����ID�� */
	TProcessId GetPID() const;

	/** �������� */
	tstring GetName() const;

	/** ���½�����Ϣ�� */
	void Update();

private:
	/** ����ID */
	TProcessId m_processId;

	/** �������� */
	tstring m_processName;
};


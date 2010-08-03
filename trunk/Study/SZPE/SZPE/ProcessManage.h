#pragma once

#include "Process.h"

class CProcessManage
{
public:
	CProcessManage(void);
	virtual ~CProcessManage(void);

public:
	

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

};


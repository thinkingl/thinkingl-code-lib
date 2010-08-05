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

	/** 进程操作。 */

	/** 获取进程信息。 */

	/** 进程ID。 */
	TProcessId GetPID() const;

	/** 进程名。 */
	tstring GetName() const;

	/** CPU */
	int GetCPUPerfor() const;

	/** 内存。 */
	int GetMemPrivate() const;

	int GetMemWorkingSet() const;

	/** 更新进程信息。 */
	void Update();

private:
	/** 进程ID */
	TProcessId m_processId;

	/** 进程名。 */
	tstring m_processName;

	/** */
	SIZE_T m_memoryPrivate;

	SIZE_T m_memoryWorkingSet;


};


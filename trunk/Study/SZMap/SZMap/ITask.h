#pragma once

#include "portabledefine.h"
/** �������, �����ɷ����������߳�ʱ����. */

class ITask
{
public:
	ITask(void);
	virtual ~ITask(void);

	/** ��ɹ���. */
	virtual BOOL Do() = 0;

};


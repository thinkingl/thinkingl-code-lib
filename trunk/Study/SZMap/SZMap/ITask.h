#pragma once

/** �������, �����ɷ����������߳�ʱ����. */

class ITask
{
public:
	ITask(void);
	virtual ~ITask(void);

	/** ��ɹ���. */
	virtual bool Do() = 0;

};


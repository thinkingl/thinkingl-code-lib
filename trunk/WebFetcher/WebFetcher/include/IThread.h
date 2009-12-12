#pragma once

#include "portabledefine.h"

class IThread
{
public:
	/** ֹͣ�߳�ʱ�ĳ�ʱʱ�䡣 */
	virtual void SetTimeOut( uint32 nMilliSeconds ) = 0;

	/** ���С� */
	virtual BOOL Start() = 0;

	/** ͬ���ȴ�ֹͣ��*/
	virtual BOOL Stop() = 0;

	/** ֻ����ֹͣ��־�� */
	virtual void SetStopFlag() = 0;

	virtual BOOL IsRunning() = 0;


	/** �����������̵߳��ã�����ʵ����ɹ�����
	*	�������ƺ���ʱ�䣬Ҫ�����ڶ�ʱ���ڷ��ء�
	*	�����޷�ֹͣ������
	*/
	virtual int DoPieceWork() = 0;
};

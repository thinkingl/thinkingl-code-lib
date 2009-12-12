#pragma once

#include "portabledefine.h"

class IThread
{
public:
	/** 停止线程时的超时时间。 */
	virtual void SetTimeOut( uint32 nMilliSeconds ) = 0;

	/** 运行。 */
	virtual BOOL Start() = 0;

	/** 同步等待停止。*/
	virtual BOOL Stop() = 0;

	/** 只设置停止标志。 */
	virtual void SetStopFlag() = 0;

	virtual BOOL IsRunning() = 0;


	/** 工作函数，线程调用，子类实现完成工作。
	*	必须限制函数时间，要可以在短时间内返回。
	*	否则无法停止函数。
	*/
	virtual int DoPieceWork() = 0;
};

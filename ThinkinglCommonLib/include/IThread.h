#pragma once

#include "portabledefine.h"

class IThread
{
public:
    virtual ~IThread(){};

	/** 停止线程时的超时时间。 */
	virtual void SetTimeOut( uint32 nMilliSeconds ) = 0;

	/** 运行。 */
	virtual BOOL Start( LPCTSTR lpstrLogFolder /*= NULL*/ ) = 0;

	/** 同步等待停止。*/
	virtual BOOL Stop() = 0;

	/** 只设置停止标志。 */
	virtual void SetStopFlag() = 0;

	virtual BOOL IsRunning() = 0;


	/** 工作函数，线程调用，子类实现完成工作。
	*	必须限制函数时间，要可以在短时间内返回。
	*	否则无法停止函数。
	*	如果子类将bExit置为TRUE，退出线程。
	*/
	virtual int DoPieceWork( BOOL& bExit ) = 0;
};

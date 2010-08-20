#pragma once

#include "stdafx.h"
#include "Coord.h"
#include <stack>

/** 当前显示的图片和位置表。 */
class CImageIndex : public CPoint
{
public:
	CImageIndex()
	{
		this->z = this->y = this->x = 0;
	}
public:
	int z;
public:
	bool operator<( const CImageIndex& another ) const
	{
		if( this->z != another.z )
		{
			return this->z < another.z;
		}
		else
		{
			if( this->x == another.x )
			{
				return this->y  < another.y;
			}
			else
			{
				return this->x < another.x;
			}
		}
	}
};

class IImageCallback
{
public:
	IImageCallback()
	{
	}

	virtual ~IImageCallback()
	{
	}

	virtual void operator()() = 0;


public:
	CImageIndex m_imgIndex;
	CImage * m_pImgDrawer;

};

class CImageFactory
{
public:
	CImageFactory(void);
	virtual ~CImageFactory(void);

	

	/** 需要的图片。 相当于下订单。 */
	void OrderImage( IImageCallback * pImgCallback );

	/** 提供图片时的回调。 */
//	void SetImageCallBack( IImageCallback * pImgCallback );

private:

	/** 图片下载/加载线程。 */
//	::CreateThread(
	DWORD WINAPI ImageLoadThread( void * param );

private:
	/** 图片下载线程句柄。 */
	HANDLE m_hImageLoadThread;
	
	/** 多线程安全锁。 */
	CCriticalSection m_threadSafeLock;

	/** 图片任务。 */
	typedef std::stack< IImageCallback * > TImageMissionStack;
	TImageMissionStack m_imageMissionsStack;

};


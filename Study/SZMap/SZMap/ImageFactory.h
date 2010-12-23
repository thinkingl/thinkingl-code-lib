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



enum EImageLoadingStatus
{
	ImageStatusOk,		// 已经成功的载入了图片。
	ImageStatusFail,	// 图片载入失败了。
	ImageStatusLoading,	// 正在载入。
};

class CImageFactory
{
public:
	CImageFactory(void);
	virtual ~CImageFactory(void);

	/** 获取某图片。
	*	如果本地没有缓存此图片，会将其 
	*	返回的是图片状态。
	*/
	EImageLoadingStatus GetImageStatus( const CImageIndex& imgIndex );

	/** 获取某图片。 
	*	返回图片指针。
	*	只有当图片状态为ImageStatusOk时才能
	*/
	CImage *GetImage( const CImageIndex& imgIndex );

	/** 设置获取图片的根目录。 
	*	支持http，ftp，本地磁盘路径。
	*/
	void SetRootUrl( LPCTSTR strUrlRoot );

	/** 设置缓存文件的目录。
	*	通过网络获取的图片需要缓存。
	*/

	
private:
	/** 下载一张图片。 */

private:

	/** 图片下载/加载线程。 */
	static DWORD WINAPI ImageLoadThread( void * param );

private:
	/** 图片下载线程句柄。 */
	HANDLE m_hImageLoadThread;
	
	/** 多线程安全锁。 */
	CCriticalSection m_threadSafeLock;

	/** 图片任务。 */
	typedef std::stack< CImageIndex > TImageMissionStack;
	TImageMissionStack m_imageMissionsStack;

};


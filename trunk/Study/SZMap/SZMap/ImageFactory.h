#pragma once

#include "stdafx.h"
#include "Coord.h"
#include <stack>

/** ��ǰ��ʾ��ͼƬ��λ�ñ� */
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

	

	/** ��Ҫ��ͼƬ�� �൱���¶����� */
	void OrderImage( IImageCallback * pImgCallback );

	/** �ṩͼƬʱ�Ļص��� */
//	void SetImageCallBack( IImageCallback * pImgCallback );

private:

	/** ͼƬ����/�����̡߳� */
//	::CreateThread(
	DWORD WINAPI ImageLoadThread( void * param );

private:
	/** ͼƬ�����߳̾���� */
	HANDLE m_hImageLoadThread;
	
	/** ���̰߳�ȫ���� */
	CCriticalSection m_threadSafeLock;

	/** ͼƬ���� */
	typedef std::stack< IImageCallback * > TImageMissionStack;
	TImageMissionStack m_imageMissionsStack;

};


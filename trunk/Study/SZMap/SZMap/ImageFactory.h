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



enum EImageLoadingStatus
{
	ImageStatusOk,		// �Ѿ��ɹ���������ͼƬ��
	ImageStatusFail,	// ͼƬ����ʧ���ˡ�
	ImageStatusLoading,	// �������롣
};

class CImageFactory
{
public:
	CImageFactory(void);
	virtual ~CImageFactory(void);

	/** ��ȡĳͼƬ��
	*	�������û�л����ͼƬ���Ὣ�� 
	*	���ص���ͼƬ״̬��
	*/
	EImageLoadingStatus GetImageStatus( const CImageIndex& imgIndex );

	/** ��ȡĳͼƬ�� 
	*	����ͼƬָ�롣
	*	ֻ�е�ͼƬ״̬ΪImageStatusOkʱ����
	*/
	CImage *GetImage( const CImageIndex& imgIndex );

	/** ���û�ȡͼƬ�ĸ�Ŀ¼�� 
	*	֧��http��ftp�����ش���·����
	*/
	void SetRootUrl( LPCTSTR strUrlRoot );

	/** ���û����ļ���Ŀ¼��
	*	ͨ�������ȡ��ͼƬ��Ҫ���档
	*/

	
private:
	/** ����һ��ͼƬ�� */

private:

	/** ͼƬ����/�����̡߳� */
	static DWORD WINAPI ImageLoadThread( void * param );

private:
	/** ͼƬ�����߳̾���� */
	HANDLE m_hImageLoadThread;
	
	/** ���̰߳�ȫ���� */
	CCriticalSection m_threadSafeLock;

	/** ͼƬ���� */
	typedef std::stack< CImageIndex > TImageMissionStack;
	TImageMissionStack m_imageMissionsStack;

};


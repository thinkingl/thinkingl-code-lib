#pragma once

#include "portabledefine.h"
#include "ITask.h"
#include "ImageFactory.h"

/** ��ͼ�������Ľӿ�. */

class IMapServer
{
public:
	IMapServer(void);
	virtual ~IMapServer(void);

	/** �Ƿ�ɹ���������Google��ͼ��URL��ʽ. */
	virtual BOOL IsKnowUrlTemplete() const = 0;

	/** ��ȡ������ͼͼƬURL���������. */
	virtual ITask* GetMapPicUrlParseTask() = 0;

	/** �������. */
	virtual void FinishMapPicUrlParseTask( ITask * pTask ) = 0;

	/** ����Index��ȡ����URL. */
	virtual ctstring GetUrl( const CImageIndex& imgIndex )const = 0; 
};


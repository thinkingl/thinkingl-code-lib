#pragma once

#include "portabledefine.h"
#include "ITask.h"
#include "ImageFactory.h"

/** 地图服务器的接口. */

class IMapServer
{
public:
	IMapServer(void);
	virtual ~IMapServer(void);

	/** 是否成功解析出了Google地图的URL格式. */
	virtual BOOL IsKnowUrlTemplete() const = 0;

	/** 获取解析地图图片URL规则的任务. */
	virtual ITask* GetMapPicUrlParseTask() = 0;

	/** 完成任务. */
	virtual void FinishMapPicUrlParseTask( ITask * pTask ) = 0;

	/** 根据Index获取下载URL. */
	virtual ctstring GetUrl( const CImageIndex& imgIndex )const = 0; 
};


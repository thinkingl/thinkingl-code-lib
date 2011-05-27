#pragma once

#include "IMapServer.h"
#include "DownloadTask.h"

class CGoogleMapServer : public IMapServer
{
public:
	CGoogleMapServer( ctstring& localSaveDir );
	~CGoogleMapServer(void);

	/** 是否成功解析出了Google地图的URL格式. */
	virtual BOOL IsKnowUrlTemplete() const ;

	/** 获取解析地图图片URL规则的任务. */
	virtual ITask* GetMapPicUrlParseTask() ;

	/** 完成任务. */
	virtual void FinishMapPicUrlParseTask( ITask * pTask ) ;

	/** 根据Index获取下载URL. */
	virtual ctstring GetUrl( const CImageIndex& imgIndex ) const ; 

private:
	tstring m_localSaveDir;

	/** 下载主页的任务.*/
	CDownloadTask *m_pMainpageDownloadTask;
};


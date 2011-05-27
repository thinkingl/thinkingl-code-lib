#pragma once

/** 地图下载总管, 负责完成整个的一个地图下载. 
*/

#include "portabledefine.h"
#include "Coord.h"
#include "szmapdefine.h"

#include "ITaskManage.h"
#include "IMapServer.h"
#include "TaskThread.h"

/** 整个地图下载 . 
*	
*/

class CMapDownloadMission : public ITaskManage
{
public:
	/** 构造函数. 
	*	
	*/
	CMapDownloadMission( const CCoord& leftTopMinZLev, const CCoord& rightBottomMaxZLev, 
		ctstring strLocalSaveDir, EMapServer mapServer, EMapType mapType );
	virtual ~CMapDownloadMission(void);

public:	// 继承自接口 ITaskManage
	/** 获得/请求分配一个新的任务. */
	virtual ITask * GetTask() ;

	/** 任务完成通知. */
	virtual void TaskFinishNotify( ITask* pTask ) ;

private:
	IMapServer *CreateMapServer( EMapServer mapServer );
private:

	/** 下载任务的边界. */
	CCoord m_coordLeftTopMinZLev;
	CCoord m_coordRightBottomMaxZLev;

	/** 下载任务的保存目录. */
	tstring m_localMapDir;

	/** 要下载的地图类型. */
	EMapType m_mapType;

	/** 地图服务器. */
//	EMapServer m_mapServer;

	IMapServer *m_pMapServer;

	/** 线程池. */
	typedef std::vector< CTaskThread * > TTaskThreadList;
	TTaskThreadList m_taskThreadList;

};


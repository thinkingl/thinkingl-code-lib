#include "MapDownloadMission.h"
#include "GoogleMapServer.h"

CMapDownloadMission::CMapDownloadMission( const CCoord& leftTop, const CCoord& rightBottom, 
	ctstring strLocalSaveDir, EMapServer mapServer, EMapType mapType )
{
	this->m_coordLeftTopMinZLev = leftTop;
	this->m_coordRightBottomMaxZLev = rightBottom;
	this->m_localMapDir = strLocalSaveDir;
//	this->m_mapServer = mapServer;
	this->m_mapType = mapType;

	this->m_pMapServer = this->CreateMapServer( mapServer );

	// 初始化线程池. 
	for ( int i=0; i<1; ++i )
	{
		CTaskThread *pNewThread = new CTaskThread( this );
		pNewThread->Start();
		m_taskThreadList.push_back( pNewThread );
	}

}


CMapDownloadMission::~CMapDownloadMission(void)
{
	// 销毁线程池..
	for ( size_t i=0; i<m_taskThreadList.size(); ++i )
	{
		if ( m_taskThreadList[i] )
		{
			m_taskThreadList[i]->Stop();
			delete m_taskThreadList[i];
			m_taskThreadList[i] = NULL;
		}		
	}
}

ITask * CMapDownloadMission::GetTask()
{
	if ( m_pMapServer == NULL )
	{
		return 0;
	}

	ITask *pNewTask = NULL;
	// 地图图片的URL的模式是否已经解析出来? 是否有线程正在进行解析?
	// 如果都没有, 那么解析图片URL.
	// 模式解析只用一个线程. 只解析一次.
	if ( !m_pMapServer->IsKnowUrlTemplete() )
	{
		pNewTask = m_pMapServer->GetMapPicUrlParseTask();
	}

	// 缺失图片队列是否已满? 是否有线程正在进行搜索? 如果都没有, 那么查找缺失的图片.
	// 缺失图片搜索只用一个线程.

	// 从队列中拿出一个下载任务.
	// 下载任务不限制同时进行的线程数目.

	return pNewTask;
}

void CMapDownloadMission::TaskFinishNotify( ITask* pTask )
{
	// 是否是地图图片URL解析任务?

	// 是否是缺失图片搜索任务?

	// 是否是下载任务.

}

IMapServer * CMapDownloadMission::CreateMapServer( EMapServer mapServer )
{
	IMapServer * pServer = NULL;
	switch( mapServer )
	{
	case GoogleMaps:
		pServer = new CGoogleMapServer( m_localMapDir );
		break;
	default:
	    break;
	}

	return pServer;
}

// end of file


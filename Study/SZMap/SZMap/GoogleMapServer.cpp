#include "GoogleMapServer.h"
#include "DownloadTask.h"
#include "szmapdefine.h"

CGoogleMapServer::CGoogleMapServer( ctstring& localSaveDir )
{
	m_localSaveDir = localSaveDir;
	this->m_pMainpageDownloadTask = NULL;
}


CGoogleMapServer::~CGoogleMapServer(void)
{
}

ITask * CGoogleMapServer::GetMapPicUrlParseTask()
{
	if ( m_pMainpageDownloadTask )
	{
		_ASSERT( FALSE );	// 已经有这么个任务了...
		return 0;
	}

	// Google 地图服务器的解析, 需要先把主页取下来.

	tstring googleMapUrl = ServerUrl::GoogleMaps;

	tstring localFilePath = m_localSaveDir;
	localFilePath += _T( "googlemapmain.html" );

	CDownloadTask *pNewTask = new CDownloadTask( googleMapUrl, localFilePath );
	m_pMainpageDownloadTask = pNewTask;
	return pNewTask;
}

void CGoogleMapServer::FinishMapPicUrlParseTask( ITask * pTask )
{
	_ASSERT( pTask == m_pMainpageDownloadTask );

	if ( m_pMainpageDownloadTask == pTask)
	{
		tstring mainPage = m_pMainpageDownloadTask->GetLocalPath();

		// 解析这个文件...

	}
}

BOOL CGoogleMapServer::IsKnowUrlTemplete() const
{	

	return FALSE;
}

ctstring CGoogleMapServer::GetUrl( const CImageIndex& imgIndex ) const
{
	_ASSERT( FALSE );
	return _T("");
}
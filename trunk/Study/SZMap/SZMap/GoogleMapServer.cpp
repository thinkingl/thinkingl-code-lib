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
		_ASSERT( FALSE );	// �Ѿ�����ô��������...
		return 0;
	}

	// Google ��ͼ�������Ľ���, ��Ҫ�Ȱ���ҳȡ����.

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

		// ��������ļ�...

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
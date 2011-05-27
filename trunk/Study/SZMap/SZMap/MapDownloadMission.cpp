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

	// ��ʼ���̳߳�. 
	for ( int i=0; i<1; ++i )
	{
		CTaskThread *pNewThread = new CTaskThread( this );
		pNewThread->Start();
		m_taskThreadList.push_back( pNewThread );
	}

}


CMapDownloadMission::~CMapDownloadMission(void)
{
	// �����̳߳�..
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
	// ��ͼͼƬ��URL��ģʽ�Ƿ��Ѿ���������? �Ƿ����߳����ڽ��н���?
	// �����û��, ��ô����ͼƬURL.
	// ģʽ����ֻ��һ���߳�. ֻ����һ��.
	if ( !m_pMapServer->IsKnowUrlTemplete() )
	{
		pNewTask = m_pMapServer->GetMapPicUrlParseTask();
	}

	// ȱʧͼƬ�����Ƿ�����? �Ƿ����߳����ڽ�������? �����û��, ��ô����ȱʧ��ͼƬ.
	// ȱʧͼƬ����ֻ��һ���߳�.

	// �Ӷ������ó�һ����������.
	// ������������ͬʱ���е��߳���Ŀ.

	return pNewTask;
}

void CMapDownloadMission::TaskFinishNotify( ITask* pTask )
{
	// �Ƿ��ǵ�ͼͼƬURL��������?

	// �Ƿ���ȱʧͼƬ��������?

	// �Ƿ�����������.

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


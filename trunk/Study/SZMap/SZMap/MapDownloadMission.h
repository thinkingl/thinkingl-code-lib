#pragma once

/** ��ͼ�����ܹ�, �������������һ����ͼ����. 
*/

#include "portabledefine.h"
#include "Coord.h"
#include "szmapdefine.h"

#include "ITaskManage.h"
#include "IMapServer.h"
#include "TaskThread.h"

/** ������ͼ���� . 
*	
*/

class CMapDownloadMission : public ITaskManage
{
public:
	/** ���캯��. 
	*	
	*/
	CMapDownloadMission( const CCoord& leftTopMinZLev, const CCoord& rightBottomMaxZLev, 
		ctstring strLocalSaveDir, EMapServer mapServer, EMapType mapType );
	virtual ~CMapDownloadMission(void);

public:	// �̳��Խӿ� ITaskManage
	/** ���/�������һ���µ�����. */
	virtual ITask * GetTask() ;

	/** �������֪ͨ. */
	virtual void TaskFinishNotify( ITask* pTask ) ;

private:
	IMapServer *CreateMapServer( EMapServer mapServer );
private:

	/** ��������ı߽�. */
	CCoord m_coordLeftTopMinZLev;
	CCoord m_coordRightBottomMaxZLev;

	/** ��������ı���Ŀ¼. */
	tstring m_localMapDir;

	/** Ҫ���صĵ�ͼ����. */
	EMapType m_mapType;

	/** ��ͼ������. */
//	EMapServer m_mapServer;

	IMapServer *m_pMapServer;

	/** �̳߳�. */
	typedef std::vector< CTaskThread * > TTaskThreadList;
	TTaskThreadList m_taskThreadList;

};


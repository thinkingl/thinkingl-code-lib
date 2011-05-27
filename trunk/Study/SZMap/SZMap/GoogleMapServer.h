#pragma once

#include "IMapServer.h"
#include "DownloadTask.h"

class CGoogleMapServer : public IMapServer
{
public:
	CGoogleMapServer( ctstring& localSaveDir );
	~CGoogleMapServer(void);

	/** �Ƿ�ɹ���������Google��ͼ��URL��ʽ. */
	virtual BOOL IsKnowUrlTemplete() const ;

	/** ��ȡ������ͼͼƬURL���������. */
	virtual ITask* GetMapPicUrlParseTask() ;

	/** �������. */
	virtual void FinishMapPicUrlParseTask( ITask * pTask ) ;

	/** ����Index��ȡ����URL. */
	virtual ctstring GetUrl( const CImageIndex& imgIndex ) const ; 

private:
	tstring m_localSaveDir;

	/** ������ҳ������.*/
	CDownloadTask *m_pMainpageDownloadTask;
};


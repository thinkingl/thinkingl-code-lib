#pragma once

#include <QRunnable>

/**
*	用户连接的任务类.
*	+by lizhixing@2014-11-23
*/
#include "VModel.h"

class CUserConnectTask :
	public QRunnable
{
public:
	CUserConnectTask( CVModel* pVModel, int userInternalId );
	virtual ~CUserConnectTask(void);

	virtual void run();

private:
	int m_userInternalId;

	CVModel* m_pVModel;
};


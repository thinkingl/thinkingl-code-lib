#pragma once

#include <QRunnable>

/**
*	�û����ӵ�������.
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


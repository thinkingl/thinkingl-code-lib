#pragma once

#include <QRunnable>

/**
*	用户连接的任务类.
*	+by lizhixing@2014-11-23
*/

class CUserConnectTask :
	public QRunnable
{
public:
	CUserConnectTask(void);
	virtual ~CUserConnectTask(void);


};


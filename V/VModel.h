#pragma once

/**
*	程序业务模型类. 同UI分离.
*	+by lizhixing@2014-11-23
*/

#include "User.h"

#include <string>
using namespace std;

#include <QRunnable>
#include <QObject>

class CVModel : public QObject
{
	Q_OBJECT
public:
	CVModel(void);
	~CVModel(void);

public:
	// 添加一个用户.
	bool AddUser( const string& ipAddr, int port );

	// 获取用户.
	bool GetUser( int userInternalId, CUser& userInfo );

private slots:
	// 定时任务,遍历用户列表.
	void PollUserList();

private:
	// 获取一个连接用户的Task.
	QRunnable* GetUserConnectTask();

private:
	// 用户列表.
	CUserList m_userList;
	// 累加的用户内部ID.
	int m_maxUserInternalId;

	// 遍历用户列表用的当前游标.
	int m_curUserListPollIndex;
};


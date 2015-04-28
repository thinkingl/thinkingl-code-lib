#pragma once

/**
*	程序业务模型类. 同UI分离.
*	+by lizhixing@2014-11-23
*/

#include "friend.h"

#include <string>
using namespace std;

#include <QRunnable>
#include <QObject>
#include <QtNetwork/QTcpServer>

class CVModel : public QObject
{
	Q_OBJECT
public:
	CVModel(void);
	~CVModel(void);

public:
	// 用户登录.
	bool Login(const QString& email, const QString& password);

	// 添加一个用户.
	bool AddUser( const string& ipAddr, int port );

	// 获取用户.
//	bool GetUser( int userInternalId, CUser& userInfo );

	// 监听本地端口,等待其它用户的连接.
	bool StartTcpService();
private slots:
	// 定时任务,遍历用户列表.
	void PollFriendsList();



	//////////////////////////////////////////////////////////////////////////
	// 有用户连接了.
	void OnUserConnect();
	// 连接上有数据可以读了.
	//void OnUserConnectReadyRead();
private:
	// 获取一个连接用户的Task.
	//QRunnable* GetUserConnectTask();

private:
	// 用户列表.
	CFriendList m_userList;

	// 接收到的连接, 对应的用户.
	CFriendList m_inConnectingUserList;

	// 累加的用户内部ID.
	int m_maxUserInternalId;

	// 遍历用户列表用的当前游标.
	int m_curUserListPollIndex;

	// 监听本地服务端口的TCP Server对象.
	QTcpServer m_tcpServer;
};


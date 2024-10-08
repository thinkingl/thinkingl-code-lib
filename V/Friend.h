#pragma once

/**
*	一个好友的类.
*	+by lizhixing@2014-11-23
*/

#include <string>
#include <vector>

#include <QObject>
#include <QTcpSocket>

using namespace std;

class CFriend : public QObject
{
	Q_OBJECT

public:
	enum{
		INVALID_USER_INTERNAL_ID = 0,
	};
public:
	CFriend(void);
	CFriend( int id, const string& ip, int port );
	virtual ~CFriend(void);

	// 从一个连接初始化一个用户.
	void FromConnect( QAbstractSocket* pSock );
public:
	// 唯一ID.
	int GetTempInternalId() const;
	void SetTempInternalId( int id );

	// 设置用户的地址.
	void SetNetAddr( const string& ip, int port );

	// 获取IP
	string GetIP() const;
	int GetPort() const;

public:
	void Work();

private slots:
	// 处理连接的信号.
	void OnConnect();
	void OnError( QAbstractSocket::SocketError err );
	void OnStateChanged(QAbstractSocket::SocketState);
	void OnReadyRead();

private:
	// 用户的唯一临时ID. 用于程序内部标识/查找用户.
	int m_tempInternalId;

	// 用户的IP.
	string m_ip;
	// 用户的网络端口.
	int m_port;

	// 用户的连接.
	QAbstractSocket* m_pUserSocket;
};

typedef std::vector< CFriend* > CFriendList;

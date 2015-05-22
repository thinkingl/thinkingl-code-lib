#pragma once

/**
*	һ���û�����.
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

	// ��һ�����ӳ�ʼ��һ���û�.
	void FromConnect( QAbstractSocket* pSock );
public:
	// ΨһID.
	int GetTempInternalId() const;
	void SetTempInternalId( int id );

	// �����û��ĵ�ַ.
	void SetNetAddr( const string& ip, int port );

	// ��ȡIP
	string GetIP() const;
	int GetPort() const;

public:
	void Work();

private slots:
	// �������ӵ��ź�.
	void OnConnect();
	void OnError( QAbstractSocket::SocketError err );
	void OnStateChanged(QAbstractSocket::SocketState);
	void OnReadyRead();

private:
	// �û���Ψһ��ʱID. ���ڳ����ڲ���ʶ/�����û�.
	int m_tempInternalId;

	// �û���IP.
	string m_ip;
	// �û�������˿�.
	int m_port;

	// �û�������.
	QAbstractSocket* m_pUserSocket;
};

typedef std::vector< CFriend* > CFriendList;
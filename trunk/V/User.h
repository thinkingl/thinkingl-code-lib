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

class CUser : public QObject
{
	Q_OBJECT

public:
	enum{
		INVALID_USER_INTERNAL_ID = 0,
	};
public:
	CUser(void);
	CUser( int id, const string& ip, int port );
	virtual ~CUser(void);

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

private:
	// �û���Ψһ��ʱID. ���ڳ����ڲ���ʶ/�����û�.
	int m_tempInternalId;

	// �û���IP.
	string m_ip;
	// �û�������˿�.
	int m_port;

	// �û�������.
	QTcpSocket* m_pUserSocket;
};

typedef std::vector< CUser* > CUserList;

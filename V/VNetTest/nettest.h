#ifndef NETTEST_H
#define NETTEST_H

#include <QObject>
#include <QTcpServer>

class NetTest : public QObject
{
	Q_OBJECT

public:
	NetTest(QObject *parent, QTcpServer* server );
    ~NetTest();

public slots:
	void Proc();

	// new connection.
	void OnNewConnection();

	void OnReadyRead();
private:
	QTcpServer* m_server;
	QTcpSocket* m_sock;
};

#endif // NETTEST_H

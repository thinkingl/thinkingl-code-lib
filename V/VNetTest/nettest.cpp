#include "nettest.h"
#include <QTextStream>
#include <QThread>
#include <iostream>
#include <QTcpSocket>


using namespace std;

NetTest::NetTest(QObject *parent, QTcpServer* server)
	: QObject(parent)
	, m_server( server )
{
//	QThread* myThread = new QThread(this);
//	myThread->start();
//	this->moveToThread(myThread);

	connect(m_server, SIGNAL(newConnection()), SLOT(OnNewConnection()));

}

NetTest::~NetTest()
{

}

void NetTest::Proc()
{
	QTextStream qout(stdout);
	QTextStream qin(stdin);
// 
// 	QString kk;
// 	qin >> kk;
// 	qout << kk << endl;
}

void NetTest::OnNewConnection()
{
	cout << "OnNewConnection!" << endl;
	QTcpSocket* pSock = m_server->nextPendingConnection();
	if ( pSock )
	{
		m_sock = pSock;
		//cout << "connected, input text to send";

		//QTextStream qin(stdin);
		connect(pSock, SIGNAL(readyRead()), SLOT(OnReadyRead()));


	}
}

void NetTest::OnReadyRead()
{
	QByteArray data = m_sock->readAll();
	qDebug() << "Read data: " << data;

	cout << data.toStdString();

	// Ð´»ØÈ¥.
	m_sock->write(data);
}

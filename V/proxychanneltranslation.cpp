#include "proxychanneltranslation.h"
#include <QTcpSocket>

ProxyChannelTranslation::ProxyChannelTranslation(QObject *parent, QTcpSocket* localSock, QString remoteAddr, int remotePort)
	: QObject(parent)
	, m_localSock(localSock)
	, m_remoteSock(0)
{
	m_remoteSock = new QTcpSocket(this);

	m_remoteSock->connectToHost(remoteAddr, remotePort);

	qDebug() << "Construct of ProxyChannelTranslation";
	connect(localSock, SIGNAL(readyRead()), SLOT(OnLocalReadyRead()));
	connect(localSock, SIGNAL(disconnected()), SLOT(OnLocalDisconnected()));
	connect(localSock, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnLocalError(QAbstractSocket::SocketError)));

	connect(m_remoteSock, SIGNAL(connected()), SLOT(OnRemoteConnected()));
	connect(m_remoteSock, SIGNAL(readyRead()), SLOT(OnRemoteReadyRead()));
	connect(m_remoteSock, SIGNAL(disconnected()), SLOT(OnRemoteDisconnected()));
	connect(m_remoteSock, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnRemoteError(QAbstractSocket::SocketError)));

}

ProxyChannelTranslation::~ProxyChannelTranslation()
{
	qDebug() << "ProxyChannelTranslation Disconstruct!";

	if ( m_remoteSock )
	{
		m_remoteSock->deleteLater();
	}
}

void ProxyChannelTranslation::OnLocalReadyRead()
{
	qDebug() << "Local Ready Read!";

	QByteArray data = m_localSock->readAll();

	//qDebug() << data;

	// 加密。
	for (int i = 0; i < data.size(); ++i )
	{
		data[i] = data[i] + 128;
	}
	qDebug() << "Local rcv size:" << data.size();

	// 发送出去。
	m_remoteSock->write( data );
}

void ProxyChannelTranslation::OnRemoteReadyRead()
{
	qDebug() << "Remote Ready Read!";

	QByteArray data = m_remoteSock->readAll();

	//qDebug() << data;

	// 加密。
	for (int i = 0; i < data.size(); ++i)
	{
		data[i] = data[i] + 128;
	}

	qDebug() << "remote rcv size: " << data.size();


	// 发送出去。
	m_localSock->write(data);
}

void ProxyChannelTranslation::OnRemoteConnected()
{
	qDebug() << "Remote Connected!";
}

void ProxyChannelTranslation::OnRemoteDisconnected()
{
	qDebug() << "Remote Disconnectd!";

	this->deleteLater();
}

void ProxyChannelTranslation::OnLocalDisconnected()
{
	qDebug() << "Local Disconnectd";

	this->deleteLater();
}

void ProxyChannelTranslation::OnLocalError(QAbstractSocket::SocketError er)
{
	qDebug() << "Local Error:" << er;

	this->deleteLater();
}

void ProxyChannelTranslation::OnRemoteError(QAbstractSocket::SocketError er)
{
	qDebug() << "Remote Error!" << er;

	this->deleteLater();
}

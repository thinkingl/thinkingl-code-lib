#include "proxychanneltranslation.h"
#include <QTcpSocket>

ProxyChannelTranslation::ProxyChannelTranslation(QObject *parent, QTcpSocket* localSock, QTcpSocket* remoteSock)
	: QObject(parent)
	, m_localSock(localSock)
	, m_remoteSock(remoteSock)
{
	qDebug() << "Construct of ProxyChannelTranslation";
	connect(localSock, SIGNAL(readyRead()), SLOT(OnLocalReadyRead()));
	connect(localSock, SIGNAL(disconnected()), SLOT(OnLocalDisconnected()));
	connect(localSock, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnLocalError(QAbstractSocket::SocketError)));

	connect(remoteSock, SIGNAL(connected()), SLOT(OnRemoteConnected()));
	connect(remoteSock, SIGNAL(readyRead()), SLOT(OnRemoteReadyRead()));
	connect(remoteSock, SIGNAL(disconnected()), SLOT(OnRemoteDisconnected()));
	connect(remoteSock, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnRemoteError(QAbstractSocket::SocketError)));

}

ProxyChannelTranslation::~ProxyChannelTranslation()
{
	qDebug() << "Disconstruct!";
}

void ProxyChannelTranslation::OnLocalReadyRead()
{
	qDebug() << "Local Ready Read!";
	QByteArray data = m_localSock->readAll();

	qDebug() << data;

	// 加密。
	for (int i = 0; i < data.size(); ++i )
	{
		data[i] = data[i] + 128;
	}

	// 发送出去。
	m_remoteSock->write( data );
}

void ProxyChannelTranslation::OnRemoteReadyRead()
{
	qDebug() << "Remote Ready Read!";

	QByteArray data = m_remoteSock->readAll();

	qDebug() << data;

	// 加密。
	for (int i = 0; i < data.size(); ++i)
	{
		data[i] = data[i] + 128;
	}

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
}

void ProxyChannelTranslation::OnLocalDisconnected()
{
	qDebug() << "Local Disconnectd";
}

void ProxyChannelTranslation::OnLocalError(QAbstractSocket::SocketError er)
{
	qDebug() << "Local Error:" << er;
}

void ProxyChannelTranslation::OnRemoteError(QAbstractSocket::SocketError er)
{
	qDebug() << "Remote Error!" << er;
}

#include "ethproxychannel.h"


ETHProxyChannel::ETHProxyChannel(QObject *parent, QTcpSocket* localSock, QString remoteAddr, int remotePort)
    : QObject(parent)
    , m_localSock(localSock)
    , m_remoteSock(0)
{
    m_remoteSock = new QTcpSocket(this);

    m_remoteSock->connectToHost(remoteAddr, remotePort);

    qDebug() << "Construct of ETHProxyChannel";
    connect(localSock, SIGNAL(readyRead()), SLOT(OnLocalReadyRead()));
    connect(localSock, SIGNAL(disconnected()), SLOT(OnLocalDisconnected()));
    connect(localSock, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnLocalError(QAbstractSocket::SocketError)));

    connect(m_remoteSock, SIGNAL(connected()), SLOT(OnRemoteConnected()));
    connect(m_remoteSock, SIGNAL(readyRead()), SLOT(OnRemoteReadyRead()));
    connect(m_remoteSock, SIGNAL(disconnected()), SLOT(OnRemoteDisconnected()));
    connect(m_remoteSock, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnRemoteError(QAbstractSocket::SocketError)));

}

ETHProxyChannel::~ETHProxyChannel()
{
    qDebug() << "ETHProxyChannel Disconstruct!";

    if ( m_remoteSock )
    {
        m_remoteSock->disconnectFromHost();
        m_remoteSock->close();
        m_remoteSock->deleteLater();
    }

    if( m_localSock )
    {
        m_localSock->close();
        m_localSock->deleteLater();
    }
}

void ETHProxyChannel::OnLocalReadyRead()
{
    qDebug() << "Local Ready Read!";

    QByteArray data = m_localSock->readAll();

    qDebug() << "local rcv: " << data;

    // 发送出去。
    m_remoteSock->write( data );
}

void ETHProxyChannel::OnRemoteReadyRead()
{
    qDebug() << "Remote Ready Read!";

    QByteArray data = m_remoteSock->readAll();

    qDebug() << "remote read: " << data;

    // 发送出去。
    m_localSock->write(data);
}

void ETHProxyChannel::OnRemoteConnected()
{
    qDebug() << "Remote Connected!";
}

void ETHProxyChannel::OnRemoteDisconnected()
{
    qDebug() << "Remote Disconnectd!";

    this->deleteLater();
}

void ETHProxyChannel::OnLocalDisconnected()
{
    qDebug() << "Local Disconnectd";

    this->deleteLater();
}

void ETHProxyChannel::OnLocalError(QAbstractSocket::SocketError er)
{
    qDebug() << "Local Error:" << er;

    this->deleteLater();
}

void ETHProxyChannel::OnRemoteError(QAbstractSocket::SocketError er)
{
    qDebug() << "Remote Error!" << er;

    this->deleteLater();
}

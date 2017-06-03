#include "echotcpsocket.h"

EchoTcpSocket::EchoTcpSocket( QObject* parent, QTcpSocket* socket )
    :QObject( parent )
    , m_pSocket( socket )
{
    qDebug() << "Construct of ProxyChannelTranslation";
    connect(m_pSocket, SIGNAL(readyRead()), SLOT(OnLocalReadyRead()));
    connect(m_pSocket, SIGNAL(disconnected()), SLOT(OnLocalDisconnected()));
    connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnLocalError(QAbstractSocket::SocketError)));
}

void EchoTcpSocket::OnLocalReadyRead()
{
    //qDebug() << "Rcv data!";
    QByteArray data = m_pSocket->readAll();
    qDebug() << "Echo recv data:[" << data << "]";
    m_pSocket->write( data );
}

void EchoTcpSocket::OnLocalDisconnected()
{
    qDebug() << "Echo recv disconnect signal!";
    this->deleteLater();
}

void EchoTcpSocket::OnLocalError(QAbstractSocket::SocketError er)
{
    qDebug() << "Echo recv socket error signal:[" << er << "]";
    this->deleteLater();
}

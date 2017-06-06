#include "tcpsource.h"
#include <QTcpSocket>


TCPSource::TCPSource( QObject* parent, QTcpSocket* localSock)
    :IDataTrans( parent )
    ,m_localSock( localSock )
{
    connect(localSock, SIGNAL(readyRead()), SLOT(OnLocalReadyRead()));
    connect(localSock, SIGNAL(disconnected()), SLOT(OnLocalDisconnected()));
    connect(localSock, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnLocalError(QAbstractSocket::SocketError)));
}

TCPSource::~TCPSource()
{
    qDebug() << "TCP Source begin to destruct";
    if( m_localSock )
    {
        m_localSock->close();
    }
}

bool TCPSource::TransDataDown(const QByteArray &dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    // TCP 源只管收发, 不修改数据.
    dataOutForward.push_back( dataIn );

    qDebug() << "TCP source trasn data forward:[" << dataIn << "]";

    return true;
}

bool TCPSource::TransDataUp(const QByteArray &dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    // 将数据发回, 不需要再回传.

    // 发回到socket.
    if( this->m_localSock )
    {
        qint64 wLen = m_localSock->write( dataIn );

        qDebug() << "TCPSource trans data back to socket:[" << dataIn << "] wlen:[" << wLen << "]";
    }
    return true;
}

void TCPSource::CloseUp()
{
    qDebug() << "TCPSource closeup, delete self later!";
    this->deleteLater();
}


void TCPSource::OnLocalReadyRead()
{
    qDebug() << "Local Ready Read!";

    QByteArray data = m_localSock->readAll();
    this->InputDataDown( this, data );
}

void TCPSource::OnLocalDisconnected()
{
    qDebug() << "Local Disconnectd";

    this->deleteLater();
}

void TCPSource::OnLocalError(QAbstractSocket::SocketError er)
{
    qDebug() << "Local Error:" << er;

    this->deleteLater();
}

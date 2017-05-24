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

}

bool TCPSource::TransDataForward(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    // TCP 源只管收发, 不修改数据.
    dataOut.clear();;
    dataOut.push_back( dataIn );
    return true;
}

bool TCPSource::TransDataBack(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    // 将数据发回, 不需要再回传.
    dataOut.clear();

    // 发回到socket.
    if( this->m_localSock )
    {
        m_localSock->write( dataIn );
    }
    return true;
}


void TCPSource::OnLocalReadyRead()
{
    qDebug() << "Local Ready Read!";

    QByteArray data = m_localSock->readAll();
    this->InputDataForward( data );
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

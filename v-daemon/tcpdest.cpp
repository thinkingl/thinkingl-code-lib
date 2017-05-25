#include "tcpdest.h"

TCPDest::TCPDest(QObject *parent, QString remoteAddr, int remotePort)
    : IDataTrans(parent)
    , m_socks5ServerAddr( remoteAddr )
    , m_socks5ServerPort( remotePort )
{

}

bool TCPDest::TransDataForward(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    // 将数据发出去.
    bool bOk = true;
    if( m_socks5TCPSocket == 0 )
    {
        m_socks5TCPSocket = new QTcpSocket(this);

        connect(m_socks5TCPSocket, SIGNAL(connected()), SLOT(OnRemoteConnected()));
        connect(m_socks5TCPSocket, SIGNAL(readyRead()), SLOT(OnRemoteReadyRead()));
        connect(m_socks5TCPSocket, SIGNAL(disconnected()), SLOT(OnRemoteDisconnected()));
        connect(m_socks5TCPSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnRemoteError(QAbstractSocket::SocketError)));

        m_socks5TCPSocket->connectToHost( this->m_socks5ServerAddr, this->m_socks5ServerPort );

    }

    if( bOk )
    {
        qint64 writeLen = m_socks5TCPSocket->write( dataIn );
        bOk = (writeLen == dataIn.length());
    }

    return bOk;
}

bool TCPDest::TransDataBack(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    // 将socks5代理服务器发送的数据发回.
    dataOut.clear();
    dataOut.push_back( dataIn );
    return true;
}

void TCPDest::OnSocks5ServerReadyRead()
{
    qDebug() << "Socks5Server Ready Read!";

    QByteArray data = m_socks5TCPSocket->readAll();
    this->InputDataBack( this, data );
}

void TCPDest::OnSocks5ServerConnected()
{
    qDebug() << "Socks5Server Connected!";
}

void TCPDest::OnSocks5ServerDisconnected()
{
    qDebug() << "Socks5Server Disconnectd!";

    this->CloseBack();
}


void TCPDest::OnSocks5ServerError(QAbstractSocket::SocketError er)
{
    qDebug() << "Socks5Server Error!" << er;

    this->CloseBack();
}

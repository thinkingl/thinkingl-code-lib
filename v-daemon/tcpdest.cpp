#include "tcpdest.h"

TCPDest::TCPDest(QObject *parent, QString remoteAddr, int remotePort)
    : IDataTrans(parent)
    , m_socks5ServerAddr( remoteAddr )
    , m_socks5ServerPort( remotePort )
    , m_socks5TCPSocket( 0 )
{
    m_socks5TCPSocket = new QTcpSocket(this);

    connect(m_socks5TCPSocket, SIGNAL(connected()), SLOT(OnSocks5ServerConnected()));
    connect(m_socks5TCPSocket, SIGNAL(readyRead()), SLOT(OnSocks5ServerReadyRead()));
    connect(m_socks5TCPSocket, SIGNAL(disconnected()), SLOT(OnSocks5ServerDisconnected()));
    connect(m_socks5TCPSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnSocks5ServerError(QAbstractSocket::SocketError)));

    m_socks5TCPSocket->connectToHost( this->m_socks5ServerAddr, this->m_socks5ServerPort );
}

TCPDest::~TCPDest()
{
    qDebug() << "TCPDest destruct!";
    if( m_socks5TCPSocket )
    {
        m_socks5TCPSocket->close();
        m_socks5TCPSocket->deleteLater();
    }
}

bool TCPDest::TransDataDown(const QByteArray &dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    // 将socks5代理服务器发送的数据发向下层协议.
    dataOutForward.push_back( dataIn );

    if( s_logRawData )
    {
        qDebug() << "TCPDst trans data up:[" << dataIn << "]";
    }

    return true;
}

bool TCPDest::TransDataUp(const QByteArray &dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    // 将数据发出去.
    bool bOk = true;
    if( m_socks5TCPSocket == 0 )
    {
        return false;

    }

    if( bOk )
    {
        qint64 writeLen = m_socks5TCPSocket->write( dataIn );
        bOk = (writeLen == dataIn.length());

        if( s_logRawData )
        {
            qDebug() << "TCPDst write data:[" << dataIn << "] wlen:[" << writeLen << "]";
        }
    }
    else
    {
        qDebug() << "Error! TCPDst trans data fail!";
    }

    return bOk;

}

void TCPDest::CloseUp()
{
    qDebug() << "TCPDest close up";
    this->deleteLater();
}

void TCPDest::OnSocks5ServerReadyRead()
{
    qDebug() << "Socks5Server Ready Read!";

    QByteArray data = m_socks5TCPSocket->readAll();
    this->InputDataDown( this, data );
}

void TCPDest::OnSocks5ServerConnected()
{
    qDebug() << "Socks5Server Connected!";
}

void TCPDest::OnSocks5ServerDisconnected()
{
    qDebug() << "Socks5Server Disconnectd!";

    this->CloseUp();
}


void TCPDest::OnSocks5ServerError(QAbstractSocket::SocketError er)
{
    qDebug() << "Socks5Server Error!" << er;

    this->CloseUp();
}

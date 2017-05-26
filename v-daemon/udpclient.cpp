#include "udpclient.h"
#include <QUdpSocket>
#include <QDateTime>
#include "vdef.h"

UDPClient::UDPClient(QObject *parent, QHostAddress serverAddr, int remotePort)
    : IDataTrans(parent)
    , m_remoteAddr( serverAddr )
    , m_remotePort( remotePort )
    , m_udpSocket( 0 )
    , m_lastActiveTime( QDateTime::currentDateTime().toTime_t() )
{

}

bool UDPClient::TransDataForward(const QByteArray &dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    // 将数据通过UDP发出.

    bool bOk = true;

    if( 0 == m_udpSocket )
    {
        m_udpSocket = new QUdpSocket(this);
        //m_udpSocket->connectToHost( m_remoteAddr, m_remotePort);
        bOk = m_udpSocket->bind();

        connect( m_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    }

    //m_udpSocket->write( dataIn );

    qint64 writedLen = m_udpSocket->writeDatagram( dataIn, m_remoteAddr, m_remotePort );
    if( -1 == writedLen )
    {
        QUdpSocket::SocketError er = m_udpSocket->error();

        qDebug() << "UDP write error:[" << er << "]-[" << m_udpSocket->errorString() << "]"
                 << " Addr:[" << m_remoteAddr.toString() << "] port: [" << m_remotePort << "]";
    }
    qDebug() << "UDP write len:[" << writedLen << "]";

    m_lastActiveTime = QDateTime::currentDateTime().toTime_t();

    return bOk;
}

bool UDPClient::TransDataBack(const QByteArray &dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    //  收到的数据发回.
    dataOutBack.push_back( dataIn );
    return false;
}

void UDPClient::readPendingDatagrams()
{
    while ( m_udpSocket->hasPendingDatagrams()) {
              QByteArray datagram;
              datagram.resize( m_udpSocket->pendingDatagramSize());
              QHostAddress sender;
              quint16 senderPort;

              m_udpSocket->readDatagram(datagram.data(), datagram.size(),
                                      &sender, &senderPort);

              this->InputDataBack( this, datagram );

              this->m_lastActiveTime = QDateTime::currentDateTime().toTime_t();
          }
}

void UDPClient::checkTimeout()
{
    if( QDateTime::currentDateTime().toTime_t() - m_lastActiveTime > UDP_TIMEOUT_MSEC/1000 )
    {
        qDebug() << "UDP Client time out!";
        this->CloseBack();
    }
}

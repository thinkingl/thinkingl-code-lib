#include "udpclient.h"
#include <QUdpSocket>

UDPClient::UDPClient(QObject *parent, QString remoteAddr, int remotePort)
    : IDataTrans(parent)
    , m_remoteAddr( remoteAddr )
    , m_remotePort( remotePort )
    , m_udpSocket( 0 )
{

}

bool UDPClient::TransDataForward(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    // 将数据通过UDP发出.
    dataOut.clear();

    bool bOk = true;

    if( 0 == m_udpSocket )
    {
        m_udpSocket = new QUdpSocket(this);
        m_udpSocket->connectToHost( m_remoteAddr, m_remotePort);
        bOk = m_udpSocket->bind();

        connect( m_udpSocket, SIGNAL(readyRead()),
                      this, SLOT(readPendingDatagrams()));
    }

    m_udpSocket->write( dataIn );

    return bOk;
}

bool UDPClient::TransDataBack(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    //  收到的数据发回.
    dataOut.clear();;
    dataOut.push_back( dataIn );
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

              this->InputDataBack( datagram );
          }
}

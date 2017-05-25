#include "udpserver.h"
#include <QDateTime>
#include <QTimer>
#include "vdef.h"
#include "datamerge.h"
#include "tcpdest.h"

UDPServer::UDPServer(QObject *parent, int localPort
                     , QString socks5ServerAddr, quint16 socks5ServerPort)
    : IDataTrans(parent)
    , m_socks5ServerAddr( socks5ServerAddr )
    , m_socks5ServerPort( socks5ServerPort )
{
    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind( localPort );

    connect( m_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    connect( &m_checkTimer, SIGNAL(timeout()), this, SLOT(CheckTableTimeout()) );
    m_checkTimer.start( 10* 1000 );

}

bool UDPServer::TransDataForward(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    dataOut.clear();
    dataOut.push_back(dataIn);
    return true;
}

bool UDPServer::TransDataBack(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    // not needed.
    return false;
}

//bool UDPServer::TransDataForward(const QByteArray &dataIn, QByteArrayList &dataOut)
//{
//    // 传下去.
//    dataOut.clear();
//    dataOut.push_back( dataIn );
//    return true;
//}

//bool UDPServer::TransDataBack(const QByteArray &dataIn, QByteArrayList &dataOut)
//{
//    return false;
//}

void UDPServer::readPendingDatagrams()
{
    while ( m_udpSocket->hasPendingDatagrams()) {
              QByteArray datagram;
              datagram.resize( m_udpSocket->pendingDatagramSize());
              QHostAddress sender;
              quint16 senderPort;

              m_udpSocket->readDatagram(datagram.data(), datagram.size(),
                                      &sender, &senderPort);

              UDPSenderAndDataTransTableItem* pItem = this->findItem( sender, senderPort );
              if( pItem == 0 )
              {
                  // 创建新的.
                  UDPSenderAndDataTransTableItem newItem;
                  newItem.dataTrans = this->CreateDataTrans();
                  newItem.senderAddr = sender;
                  newItem.senderPort = senderPort;
                  newItem.lastActiveTime = QDateTime::currentDateTime().toTime_t();
                  this->m_udpSenderAndDataTransTable.push_back( newItem );
              }

              pItem->dataTrans->InputDataForward( this, datagram );
              pItem->lastActiveTime = QDateTime::currentDateTime().toTime_t();
    }
}

void UDPServer::CheckTableTimeout()
{
    for( UDPSenderAndDataTransTable::iterator it = m_udpSenderAndDataTransTable.begin(); it != m_udpSenderAndDataTransTable.end(); ++it )
    {
        time_t now = QDateTime::currentDateTime().toTime_t();
        if( now - it->lastActiveTime > UDP_TIMEOUT_MSEC/1000 )
        {
            // 超时.
            qDebug() << "UDP Server time out! dst ip:["
                     << it->senderAddr << ":" << it->senderPort << "]";
            it->dataTrans->deleteLater();


            it = m_udpSenderAndDataTransTable.erase( it );
        }
    }
}

IDataTrans *UDPServer::CreateDataTrans()
{
    DataMerge* pDataMerge = new DataMerge( this );
    TCPDest* pTCPDest = new TCPDest( this, m_socks5ServerAddr, m_socks5ServerPort );

    pDataMerge->SetNextDataTrans( this, pTCPDest );
    pTCPDest->SetNextDataTrans( pDataMerge, 0 );
    return pDataMerge;
}

UDPServer::UDPSenderAndDataTransTableItem *UDPServer::findItem(const QHostAddress &senderAddr, quint16 senderPort)
{
    for( UDPSenderAndDataTransTable::iterator it = m_udpSenderAndDataTransTable.begin(); it != m_udpSenderAndDataTransTable.end(); ++it )
    {
        if( it->senderAddr == senderAddr && it->senderPort == senderPort )
        {
            return &(*it);
        }
    }
//    foreach ( UDPServer::UDPSenderAndDataTransTableItem& item, m_udpSenderAndDataTransTable) {
//        if( item.senderAddr == senderAddr && item.senderPort == senderPort )
//        {
//            return &item;
//        }
//    }
    return 0;
}

UDPServer::UDPSenderAndDataTransTableItem *UDPServer::findItem(IDataTrans *pTrans)
{
    for( UDPSenderAndDataTransTable::iterator it = m_udpSenderAndDataTransTable.begin(); it != m_udpSenderAndDataTransTable.end(); ++it )
    {
        if( it->dataTrans == pTrans )
        {
            return &(*it);
        }
    }
    return 0;
}

bool UDPServer::InputDataBack(IDataTrans *pNextItem, const QByteArray &data)
{
    if( 0 == m_udpSocket )
    {
        return false;
    }

    UDPSenderAndDataTransTableItem* item = this->findItem( pNextItem );
    if( item )
    {
        // 发回给UDP客户端.
        m_udpSocket->writeDatagram( data, item->senderAddr, item->senderPort );
        item->lastActiveTime = QDateTime::currentDateTime().toTime_t();
    }
    else
    {
        qDebug() << "Can't find the item!";
        return false;
    }
    return true;
}

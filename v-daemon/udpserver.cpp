#include "udpserver.h"
#include <QDateTime>
#include <QTimer>
#include "vdef.h"
#include "tcpdest.h"
#include "datahashcheck.h"
#include "transprotocol.h"
#include "datasplit.h"

UDPServer::UDPServer(QObject *parent)
    : IDataTrans(parent)    
    , m_udpSocket( 0 )
{
}

bool UDPServer::Start(int localPort, QString socks5ServerAddr, quint16 socks5ServerPort)
{
    if( m_udpSocket )
    {
        m_udpSocket->deleteLater();
        m_udpSocket = 0;
    }

    m_socks5ServerAddr = socks5ServerAddr;
    m_socks5ServerPort = socks5ServerPort;

    m_udpSocket = new QUdpSocket(this);
    connect( m_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    bool bOk = m_udpSocket->bind( localPort );

    connect( &m_checkTimer, SIGNAL(timeout()), this, SLOT(CheckTableTimeout()) );
    m_checkTimer.start( 10* 1000 );

    return bOk;
}

bool UDPServer::TransDataDown(const QByteArray &dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    qDebug() << "Shouldn't call UDPServer::TransDataDown function!";
    return true;
}

bool UDPServer::TransDataUp(const QByteArray &dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    qDebug() << "Shouldn't call UDPServer::TransDataUp function!";
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

              if( datagram.isEmpty() )
              {
                  continue;
              }

              qDebug() << "UDPServer read datagram:[" << datagram << "]";

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

                  pItem = this->findItem( sender, senderPort );
              }            

              // 服务端接收到的 数据向上解析.
              //pItem->dataTrans->InputDataForward( this, datagram );
              pItem->dataTrans->InputDataUp( this, datagram );



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
            //it->dataTrans->deleteLater();

            it->dataTrans->CloseUp();

            it = m_udpSenderAndDataTransTable.erase( it );
        }
    }
}

IDataTrans *UDPServer::CreateDataTrans()
{
    // Down -> Up,  UDPServer -> Socks5Proxy
    // UDPServer -> Trans Protocol -> DataSplit -> DataHashCheck  -> TCPDest -> socks5Proxy

    TCPDest* pTCPDest = new TCPDest( this, m_socks5ServerAddr, m_socks5ServerPort );
    DataHashCheck* pDataHashCheck = new DataHashCheck( this );
    DataSplit* pDataSplit = new DataSplit(this);
    TransProtocol* pTransProtocol = new TransProtocol( this );

    // 连接.
    pTCPDest->SetNextDataTrans(0, pDataHashCheck );
    pDataHashCheck->SetNextDataTrans( pTCPDest, pDataSplit );
    pDataSplit->SetNextDataTrans( pDataHashCheck, pTransProtocol );
    pTransProtocol->SetNextDataTrans( pDataSplit, this );

    return pTransProtocol;
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

bool UDPServer::InputDataDown(IDataTrans *pNextItem, const QByteArray &data)
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

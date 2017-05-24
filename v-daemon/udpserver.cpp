#include "udpserver.h"

UDPServer::UDPServer(QObject *parent, int localPort)
    : IDataTrans(parent)
{
    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind( localPort );
}

bool UDPServer::TransDataForward(const QByteArray &, QByteArrayList &)
{
    return false;
}

bool UDPServer::TransDataBack(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    return false;
}

void UDPServer::readPendingDatagrams()
{

}

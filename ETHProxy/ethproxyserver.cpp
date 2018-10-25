#include "ethproxyserver.h"
#include "ethproxychannel.h"
#include <QHostInfo>


ETHProxyServer::ETHProxyServer(QObject *parent)
    : QObject(parent)
{
    connect(&m_server, SIGNAL(newConnection()), SLOT(OnNewConnection()));
}

ETHProxyServer::~ETHProxyServer()
{
    this->Stop();
}

void ETHProxyServer::Start(int localPort, const QString& remoteAddr, int remotePort)
{
    qDebug() << "local port:[" << localPort << "] remote addr:[" << remoteAddr << "] port:[" << remotePort << "]";
    this->Stop();

//    QHostInfo info = QHostInfo::fromName( remoteAddr );

//    if( info.addresses().empty() )
//    {
//        qDebug() << "Failed to find the host [" << remoteAddr << "]";
//        return;
//    }
//    else
//    {
//        qDebug() << "Resolve the host:[" << remoteAddr << "]-["
//                 << info.addresses().at(0).toString() << "]";
//    }
//    this->m_remoteAddr = info.addresses().at(0);

    this->m_remoteAddr = remoteAddr;
    this->m_remotePort = remotePort;

    bool bOk = m_server.listen(QHostAddress::Any, localPort);
    if( !bOk )
    {
        qDebug() << "Error! Failed to listen the port[" << localPort << "]";
    }


}

void ETHProxyServer::Stop()
{
    m_server.close();
}

void ETHProxyServer::OnNewConnection()
{
    QTcpSocket* pendingSocket = m_server.nextPendingConnection();


    // 建立一个新连接。
    if( pendingSocket )
    {
        ETHProxyChannel* pChn = new ETHProxyChannel( this, pendingSocket, m_remoteAddr, m_remotePort );
        qDebug() << "New pending socket created!!!";
    }
    else
    {
        qDebug() << "No pending socket!!!!!!!!!!!!!!!!!";
    }
//	QTcpSocket* remoteSock = new QTcpSocket(this);
//	remoteSock->connectToHost(m_remoteAddr, m_remotePort);

//	ProxyChannelTranslation * pTrans = new ProxyChannelTranslation(this, pendingSocket, m_remoteAddr, m_remotePort );

//	connect(pendingSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnError(QAbstractSocket::SocketError)));
//	connect(pendingSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(OnStateChanged(QAbstractSocket::SocketState)));
//	connect(pendingSocket, SIGNAL(readyRead()), SLOT(OnReadyRead()));


}

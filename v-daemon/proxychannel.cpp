#include "proxychannel.h"
#include "proxychanneltranslation.h"
#include "tcpsource.h"
#include "datasplit.h"
#include "udpclient.h"
#include "datahashcheck.h"
#include "transprotocol.h"
#include <QHostInfo>

ProxyChannel::ProxyChannel(QObject *parent)
	: QObject(parent)
{
	connect(&m_server, SIGNAL(newConnection()), SLOT(OnNewConnection()));
}

ProxyChannel::~ProxyChannel()
{
    this->Stop();
}

void ProxyChannel::Start(int localPort, const QString& remoteAddr, int remotePort)
{
	qDebug() << "local port:[" << localPort << "] remote addr:[" << remoteAddr << "] port:[" << remotePort << "]";
	this->Stop();

    QHostInfo info = QHostInfo::fromName( remoteAddr );

    if( info.addresses().empty() )
    {
        qDebug() << "Failed to find the host [" << remoteAddr << "]";
        return;
    }
    else
    {
        qDebug() << "Resolve the host:[" << remoteAddr << "]-["
                 << info.addresses().at(0).toString() << "]";
    }
    this->m_remoteAddr = info.addresses().at(0);
	this->m_remotePort = remotePort;

	bool bOk = m_server.listen(QHostAddress::Any, localPort);
    if( !bOk )
    {
        qDebug() << "Error! Failed to listen the port[" << localPort << "]";
    }


}

void ProxyChannel::Stop()
{
	m_server.close();

	foreach( QTcpSocket* sock, m_localTcpSocketTable)
	{
		sock->close();
	}
	m_localTcpSocketTable.clear();
}

void ProxyChannel::OnNewConnection()
{
	QTcpSocket* pendingSocket = m_server.nextPendingConnection();

	// 建立一个新连接。
//	QTcpSocket* remoteSock = new QTcpSocket(this);
//	remoteSock->connectToHost(m_remoteAddr, m_remotePort);

//	ProxyChannelTranslation * pTrans = new ProxyChannelTranslation(this, pendingSocket, m_remoteAddr, m_remotePort );

//	connect(pendingSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnError(QAbstractSocket::SocketError)));
//	connect(pendingSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(OnStateChanged(QAbstractSocket::SocketState)));
//	connect(pendingSocket, SIGNAL(readyRead()), SLOT(OnReadyRead()));

    // UDP方式.
    TCPSource* pTCPSource = new TCPSource( this, pendingSocket );
    DataHashCheck* pDataHashCheck = new DataHashCheck( this );
    DataSplit* pDataSplit = new DataSplit(this);
    TransProtocol* pTransProtocol = new TransProtocol( this );
    UDPClient* pUDPClient = new UDPClient(this, this->m_remoteAddr, this->m_remotePort );

    // 连接起来. TCPSource -> DataHashCheck -> DataSplit -> TransProtocol -> UDPClient.
    pTCPSource->SetNextDataTrans(0, pDataHashCheck );
    pDataHashCheck->SetNextDataTrans( pTCPSource, pDataSplit );
    pDataSplit->SetNextDataTrans(pDataHashCheck, pTransProtocol);
    pTransProtocol->SetNextDataTrans( pDataSplit, pUDPClient );
    pUDPClient->SetNextDataTrans(pTransProtocol, 0);
}

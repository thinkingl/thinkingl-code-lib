#include "proxychannel.h"
#include "proxychanneltranslation.h"

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

	this->m_remoteAddr = remoteAddr;
	this->m_remotePort = remotePort;

	bool bOk = m_server.listen(QHostAddress::Any, localPort);
	Q_ASSERT(bOk);

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

	ProxyChannelTranslation * pTrans = new ProxyChannelTranslation(this, pendingSocket, m_remoteAddr, m_remotePort );

//	connect(pendingSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(OnError(QAbstractSocket::SocketError)));
//	connect(pendingSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(OnStateChanged(QAbstractSocket::SocketState)));
//	connect(pendingSocket, SIGNAL(readyRead()), SLOT(OnReadyRead()));
}

#include "echotcpserver.h"
#include "echotcpsocket.h"

EchoTCPServer::EchoTCPServer(QObject *parent) : QObject(parent)
{
    connect(&m_server, SIGNAL(newConnection()), SLOT(OnNewConnection()));
}

void EchoTCPServer::Start(int localPort)
{
    bool bOk = m_server.listen(QHostAddress::Any, localPort);
    Q_ASSERT(bOk);
    if( !bOk )
    {
        qDebug() << "Failed to listen the port[" << localPort << "]";
    }
}

void EchoTCPServer::OnNewConnection()
{
    QTcpSocket* pendingSocket = m_server.nextPendingConnection();

    EchoTcpSocket * echoSocket = new EchoTcpSocket( this, pendingSocket );
}

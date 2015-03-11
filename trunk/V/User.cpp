#include "User.h"
#include "protocol.h"
#include <QXmlStreamWriter>

CUser::CUser(void)
	: m_tempInternalId( INVALID_USER_INTERNAL_ID )
	, m_port( 0 )
	, m_pUserSocket( 0 )
{
}

CUser::CUser( int id, const string& ip, int port )
	: m_tempInternalId( id )
	, m_ip( ip )
	, m_port( port )
	, m_pUserSocket( 0 )
{
}


CUser::~CUser(void)
{
}

std::string CUser::GetIP() const
{
	return m_ip;
}

int CUser::GetPort() const
{
	return m_port;
}

int CUser::GetTempInternalId() const
{
	return m_tempInternalId;
}

void CUser::SetTempInternalId( int id )
{
	m_tempInternalId = id;
}

void CUser::Work()
{
	if ( NULL == m_pUserSocket )
	{
		m_pUserSocket = new QTcpSocket( this );

		connect( m_pUserSocket, SIGNAL( error(QAbstractSocket::SocketError) ), SLOT( OnError( QAbstractSocket::SocketError ) ) );
		connect( m_pUserSocket, SIGNAL( connected() ), SLOT( OnConnect()) );
		connect( m_pUserSocket, SIGNAL( stateChanged( QAbstractSocket::SocketState ) ), SLOT( OnStateChanged( QAbstractSocket::SocketState ) ) ) ;
		connect( m_pUserSocket, SIGNAL( readyRead() ), SLOT( OnReadyRead() ) );
		m_pUserSocket->connectToHost( this->GetIP().c_str(), this->GetPort() );
	}
	
}

void CUser::OnConnect()
{
	qDebug() << "Connect Ok!";

	// 给这个用户发送hello.
	CHelloReq hello;
	hello.SetVersion( PROTOCOL_CUR_VERSION );
	hello.Id() = "ID";
	hello.Name() = "Name";
	hello.Email() = "Email";
	hello.UrlList();

	QXmlStreamWriter writer( m_pUserSocket );
	hello.ToXML( writer );

	//writer.

	//m_pUserSocket->write( "Hello u!" );
}

void CUser::OnError( QAbstractSocket::SocketError err )
{
	qDebug() << "Connect err! er: " << err;

	delete m_pUserSocket;
	m_pUserSocket = NULL;

}

void CUser::OnStateChanged( QAbstractSocket::SocketState st )
{
	qDebug() << "Socket state changed! st:" << st;
}

void CUser::OnReadyRead()
{
	qDebug() << "Socket ready read.";

	char zBuff[ 10*1024 ] = {0};
	qint64 readLen = m_pUserSocket->read( zBuff, sizeof( zBuff ) );

	qDebug() << "Socket read len:" << readLen << " data: " << zBuff;
}

void CUser::FromConnect( QAbstractSocket* pSock )
{
	m_pUserSocket = pSock;

	connect( m_pUserSocket, SIGNAL( error(QAbstractSocket::SocketError) ), SLOT( OnError( QAbstractSocket::SocketError ) ) );
	connect( m_pUserSocket, SIGNAL( connected() ), SLOT( OnConnect()) );
	connect( m_pUserSocket, SIGNAL( stateChanged( QAbstractSocket::SocketState ) ), SLOT( OnStateChanged( QAbstractSocket::SocketState ) ) ) ;
	connect( m_pUserSocket, SIGNAL( readyRead() ), SLOT( OnReadyRead() ) );
}

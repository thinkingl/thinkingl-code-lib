#include "User.h"


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

		m_pUserSocket->connectToHost( this->GetIP().c_str(), this->GetPort() );
	}
	
}

void CUser::OnConnect()
{
	qDebug() << "Connect Ok!";

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

#include "User.h"


CUser::CUser(void)
	: m_tempInternalId( INVALID_USER_INTERNAL_ID )
	, m_port( 0 )
{
}

CUser::CUser( int id, const string& ip, int port )
	: m_tempInternalId( id )
	, m_ip( ip )
	, m_port( port )
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

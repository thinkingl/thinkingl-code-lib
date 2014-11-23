#include "User.h"


CUser::CUser(void)
{
}

CUser::CUser( const string& ip, int port )
	: m_ip( ip )
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

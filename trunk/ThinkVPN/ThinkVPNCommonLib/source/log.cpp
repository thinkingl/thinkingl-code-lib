
#include "log.h"

using namespace vpn;

MU_DECLSPEC  CLog  vpn::tlog;
//__declspec(dllexport) CLog   tlog;

CLog::CLog(void)
{
}

CLog::~CLog(void)
{
}

CLog& CLog::operator <<( const wchar_t * strMsg)
{

#ifdef _WIN32_WCE
	OutputDebugString( strMsg );
#else	
#ifndef __CYGWIN__	// cygwin ²»Ö§³Öunicode!
	std::wcout << strMsg;
#else
	cout << ( "CLog << fail! cygwin don't support unicode!" ) << endl;
#endif
#endif
	
	return *this;
}

CLog& CLog::operator <<( const char * strMsg )
{
	cout << strMsg;
	return *this;
}

CLog& CLog::operator <<( const string& strMsg )
{	
	return ( (*this) << strMsg.c_str() );
}

CLog& CLog::operator <<( const wstring& strMsg )
{	
	return ( (*this) << strMsg.c_str() );
}

CLog& CLog::operator <<( long nNum )
{
	tcout << nNum;
	return *this;
}
/**
CLog& CLog::operator << ( FunWEndl _Pfn )
{
	tcout << _Pfn;
	return *this;
}/storage card/

CLog& CLog::operator << ( FunAEndl _Pfn )
{
	tcout << _Pfn;
	return *this;
}
*/
CLog& CLog::operator << ( FunTEndl _Pfn )
{
	tcout << _Pfn;
	return *this;
}




CLog& CLog::operator <<( int _Val)
{
	tcout << _Val;
	return *this;
}

CLog& CLog::operator <<(const void *_Val)
{
	tcout << _Val;
	return *this;
}

CLog& CLog::operator <<( unsigned int _Val)
{
	tcout << _Val;
	return *this;
}

CLog& CLog::operator <<( unsigned long _Val)
{
	tcout << _Val;
	return *this;
}

CLog& CLog::operator<<( mu_int64 _val )
{
	tcout << _val;
	return *this;
}

CLog& CLog::operator<<( mu_uint64 _val )
{
	tcout << _val;
	return *this;
}

CLog& CLog::operator <<(double _Val)
{
	tcout << _Val;
	return *this;
}

CLog& CLog::operator <<(short _var )
{
	tcout << _var;
	return *this;
}

CLog& CLog::operator <<( const float _var )
{
	tcout << _var;
	return *this;
}



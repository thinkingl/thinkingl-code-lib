
#include "log.h"
#include "common.h"

using namespace vpn;

MU_DECLSPEC  CLog  vpn::log;
//__declspec(dllexport) CLog   log;

CLog::CLog(void)
{

}

CLog::~CLog(void)
{
}

void CLog::SetLogFileDir( LPCTSTR strLogDir, LPCTSTR strPrefix )
{
    if ( m_strLogDir != strLogDir )
    {
        m_strLogDir = strLogDir;

        tstring strLogFilePath = strLogDir;

        NormalizeDir( strLogFilePath );

        strLogFilePath += strPrefix;
        strLogFilePath += _T( "-" );

        strLogFilePath += TimeToStr( GetCurTime() );
        strLogFilePath += _T( ".log" );

        this->m_fLog.open( strLogFilePath.c_str() );

    }
    else
    {
        // already¡£
    }
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

    if ( this->m_fLog )
    {
        m_fLog << strMsg ;
        m_fLog.flush();
    }
	
	return *this;
}

CLog& CLog::operator <<( const char * strMsg )
{
	cout << strMsg;

    if ( this->m_fLog )
    {
        m_fLog << strMsg ;
        m_fLog.flush();
    }

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
    tstringstream ssTmp;
    ssTmp << nNum;
	
	return ( *this << ssTmp.str() );
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
    tstringstream ssTmp;
    ssTmp << _Pfn;
    
	return ( *this << ssTmp.str() );
}




CLog& CLog::operator <<( int _Val)
{
    tstringstream ssTmp;
    ssTmp << _Val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<(const void *_Val)
{
    tstringstream ssTmp;
    ssTmp << _Val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<( unsigned int _Val)
{
    tstringstream ssTmp;
    ssTmp << _Val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<( unsigned long _Val)
{
    tstringstream ssTmp;
    ssTmp << _Val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator<<( mu_int64 _val )
{
    tstringstream ssTmp;
    ssTmp << _val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator<<( mu_uint64 _val )
{
    tstringstream ssTmp;
    ssTmp << _val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<(double _Val)
{
    tstringstream ssTmp;
    ssTmp << _Val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<(short _var )
{
    tstringstream ssTmp;
    ssTmp << _var;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<( const float _var )
{
    tstringstream ssTmp;
    ssTmp << _var;

    return ( *this << ssTmp.str() );
}



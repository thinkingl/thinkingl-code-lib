#include "Log.h"
#include "ClassFactory.h"
#include "ScopeLock.h"
#include "Common.h"
#include "LogManager.h"
//
CLog& Log( ELogLevel eLogLev )
{
	CLog *pLog = CLogManager::Instance()->GetCurThreadLog();	

	BOOL bWriteLog = pLog->IsLogout( eLogLev );

	pLog->SetIsLogout( bWriteLog );
    return *pLog;
}

CLog::CLog(void)
{
    this->m_pThreadSafeLock = CClassFactory::CreateMutex();

	this->m_bConsoleLogout = TRUE;
	this->m_eLogLev = LogLevHigh;
}

CLog::~CLog(void)
{
	if ( this->m_pThreadSafeLock )
	{
		delete m_pThreadSafeLock;
		m_pThreadSafeLock = NULL;
	}
}

void CLog::SetIsLogout( BOOL bLog )
{
	SCOPE_LOCK( *m_pThreadSafeLock );
	this->m_bConsoleLogout = bLog;
}

BOOL CLog::IsLogout( ELogLevel eLev )
{
	return eLev >= m_eLogLev;
}

void CLog::SetConsleLogLev( ELogLevel eLev )
{
	this->m_eLogLev = eLev;
}


void CLog::SetLogFileDir( LPCTSTR strLogDir, LPCTSTR strPrefix )
{
    SCOPE_LOCK( *m_pThreadSafeLock );

    if ( m_strLogDir != strLogDir )
    {
        m_strLogDir = strLogDir;

        tstring strLogFilePath = strLogDir;

        CCommon::NormalizeDir( strLogFilePath );

		CCommon::CreateDirRecurse( strLogFilePath.c_str() );

        strLogFilePath += strPrefix;
        strLogFilePath += _T( "-" );

        strLogFilePath += CCommon::TimeToStr( CCommon::GetCurTime() );
        strLogFilePath += _T( ".log" );

        // 某些版本的iostram的open不支持16bit unicode版本的open。
        //        this->m_fLog.open( strLogFilePath.c_str() );
//        string strUtf8 ;
//         CCommon::Utf16toUtf8( strLogFilePath.c_str(), strUtf8 );
//		 this->m_fLog.open( strUtf8.c_str(), ios::out | ios::binary );
		 this->m_fLog.Open( strLogFilePath.c_str(), CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyWrite );

		this->m_strLogFilePath = strLogFilePath;

    }
    else
    {
        // already。
    }
}

CLog& CLog::operator <<( const wchar_t * strMsg)
{
    SCOPE_LOCK( *m_pThreadSafeLock );


#if defined( _WIN32_WCE ) || defined ( _WIN32 ) 
    OutputDebugString( strMsg );
#endif

	if ( m_bConsoleLogout )
	{


#ifndef __CYGWIN__	// cygwin 不支持unicode!
    std::wcout << strMsg;
#else
    cout << ( "CLog << fail! cygwin don't support unicode!" ) << endl;
#endif

	}

	if ( m_fLog.m_hFile != CFile::hFileNull )
    {
		if ( m_fLog.GetLength() > 100000000 )
		{
			m_fLog.Close();
			m_fLog.Open( m_strLogFilePath.c_str(), CFile::modeWrite | CFile::modeCreate | CFile::shareDenyWrite );
		}

		string strUtf8;
		CCommon::Utf16toUtf8( strMsg, strUtf8 );
		m_fLog.Write( strUtf8.c_str(), strUtf8.length() );
//        m_fLog << strMsg ;
//        m_fLog.flush();
    }
	else if( !m_strLogFilePath.empty() )
	{
		ASSERT( FALSE );
	}

    return *this;
}

CLog& CLog::operator <<( const char * strMsg )
{
    SCOPE_LOCK( *m_pThreadSafeLock );

	if ( m_bConsoleLogout )
	{
		cout << strMsg;
	}
    

#if defined( _WIN32 )
	OutputDebugStringA( strMsg );
#endif

	if ( m_fLog.m_hFile != CFile::hFileNull )
	{
		m_fLog.Write( strMsg, strlen( strMsg ) * sizeof( strMsg[0] ) );
		//        m_fLog << strMsg ;
		//        m_fLog.flush();
	}
	else if( !m_strLogFilePath.empty() )
	{
		ASSERT( FALSE );
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

    *this << ssTmp.str();

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

CLog& CLog::operator<<( int64 _val )
{
    tstringstream ssTmp;
    ssTmp << _val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator<<( uint64 _val )
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

    *this << ssTmp.str();

    return *this;
}
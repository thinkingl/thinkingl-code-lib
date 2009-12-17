#pragma once

#include "portabledefine.h"
#include "IMutex.h"




class CLog 
{
public:
	CLog(void);
	virtual ~CLog(void);

    void SetLogFileDir( LPCTSTR strLogDir, LPCTSTR strPrefix );
public:

    /** �����־�� */
    //	CLog& operator << ( LPCTSTR strMsg );

    CLog& operator << ( const char * strMsg );

    CLog& operator << ( const wchar_t * strMsg );

    CLog& operator << ( const string& strMsg );

    CLog& operator << ( const wstring& strMsg );

    //	CLog& operator << ( int nNum );
    CLog& operator << ( short sNum );
    CLog& operator << ( unsigned short _Val );
    CLog& operator<<(int  _Val);
    CLog& operator<<(unsigned int  _Val);
    CLog& operator<<(long _Val);
    CLog& operator<<(unsigned long  _Val);

    CLog& operator<<( int64 _Val);
    CLog& operator<<( uint64  _Val);

    CLog& operator << ( char cChar );

    CLog& operator << ( const float fNum );	
    CLog& operator<<(double _Val);
    CLog& operator<<(long double _Val);

    CLog& operator<<(const void *_Val);

    /** Ϊ�����endl�� */
    typedef basic_ostream<TCHAR, char_traits<TCHAR> > _TMyt;
    typedef _TMyt& ( * FunTEndl )(_TMyt&) ;
    CLog& operator << ( FunTEndl _Pfn );

    //	typedef basic_ostream<wchar_t, char_traits<wchar_t> > _WMyt;
    //	typedef _WMyt& ( * FunWEndl )(_WMyt&) ;
    //	CLog& operator << ( FunWEndl _Pfn );

    //	typedef basic_ostream<char, char_traits<char> > _AMyt;
    //	typedef _AMyt& ( * FunAEndl )(_AMyt&) ;
    //	CLog& operator << ( FunAEndl _Pfn );

private:
    /** ���߳�ͬ������ */
    IMutex *m_pThreadSafeLock;

    /** �Ƿ�д�ļ��� */
    BOOL m_bWriteFile;

    /** �ļ����� */
    wofstream m_fLog;

    /** ��־�ļ��С� */
    tstring m_strLogDir;
};

CLog& Log();

//#define CLog() tcout


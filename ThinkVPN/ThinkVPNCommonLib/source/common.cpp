
#include "common.h"
#include <algorithm>
#include "log.h"

#ifdef __GNUC__
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#endif


#ifdef WIN32_MODE

string UTF16toUTF8( wstring strUTF16 )
{
	if ( strUTF16.empty() )
	{
		return "";
	}

	int nLen8 = WideCharToMultiByte( CP_UTF8, 0, strUTF16.c_str(), strUTF16.length(), 0, 0, 0, 0 );
	if ( 0 == nLen8 )
	{
		return "";
	}

	std::vector<char> vTemp( nLen8+1, 0 );
	WideCharToMultiByte( CP_UTF8, 0, strUTF16.c_str(), strUTF16.length(), &vTemp[0], nLen8, 0, 0 );

	return &vTemp[0];
}

wstring UTF8toUTF16( string strUTF8 )
{
	if ( strUTF8.empty() )
	{
		return L"";
	}

	std::vector< wchar_t > vTemp( strUTF8.length()+1, 0 );
	MultiByteToWideChar( CP_UTF8, 0, strUTF8.c_str(), strUTF8.length(), &vTemp[0], strUTF8.length() );

	return &vTemp[0];
}

tstring GetModulePath( HMODULE hm /*= NULL*/ )
{
	std::vector< wchar_t > vTemp( MAX_PATH+1, 0 );
	::GetModuleFileName( hm, &vTemp[0], MAX_PATH );

	return &vTemp[0];
}
#endif

bool TFileNameInfo::operator ==( const TFileNameInfo& tAnother ) const
{
	if ( this == &tAnother )
	{
		return true;
	}
	return( tAnother.m_strFilePath == this->m_strFilePath 
		&& tAnother.m_strFileName == this->m_strFileName
		&& tAnother.m_strExtName == this->m_strExtName
		&& tAnother.m_strDirectory == this->m_strDirectory
		&& tAnother.m_strBaseName == this->m_strBaseName );
}


TFileNameInfo ParsePath( LPCTSTR strPath )
{
	TFileNameInfo tInfo;
	ParsePath( strPath, tInfo );
	return tInfo;
}

BOOL ParsePath( LPCTSTR strPath, TFileNameInfo& tFileNameInfo )
{
	tFileNameInfo.m_strFilePath = strPath;

	tstring strAppPath = strPath;

#if defined( WIN32_MODE )
	const TCHAR chDirToken = '\\';
	const TCHAR chWrongDirToken = '/';
#else
	const TCHAR chDirToken = '/';
	const TCHAR chWrongDirToken = '\\';
#endif

	replace( strAppPath.begin(), strAppPath.end(), chWrongDirToken, chDirToken );


	int nIndex = strAppPath.rfind( chDirToken );
	if( nIndex >=0 )
	{	
		tFileNameInfo.m_strFileName = strAppPath.substr( nIndex + 1 );
		tFileNameInfo.m_strDirectory = strAppPath.substr( 0, nIndex + 1 );
	}	
	else
	{
		tFileNameInfo.m_strFileName = strAppPath;
		tFileNameInfo.m_strDirectory = _T( "" );
	}

	tstring strAppName = tFileNameInfo.m_strFileName;
	nIndex = strAppName.rfind( '.' );
	if( nIndex >= 0 )
	{
		tFileNameInfo.m_strBaseName = strAppName.substr( 0, nIndex );
		tFileNameInfo.m_strExtName = strAppName.substr( nIndex + 1);
	}
	else
	{
		tFileNameInfo.m_strBaseName = strAppName;
		tFileNameInfo.m_strExtName = _T( "" );
	}
	return TRUE;
}


tstring GetAppName()
{
#ifdef WIN32_MODE
	return ParsePath( GetModulePath().c_str() ).m_strBaseName;
#else
	return _T( "" );
#endif
}

tstring GetAppDir()
{
#ifdef WIN32_MODE
	return ParsePath( GetModulePath().c_str() ).m_strDirectory;
#else
	return _T( "" );
#endif
}

#ifdef WIN32_MODE

int GetScreenLong()
{
	int nScreenWidth = ::GetScreenWidth();
	int nScreenHeight = ::GetScreenHeight();
	int nScreenLong = max( nScreenWidth, nScreenHeight );
	return nScreenLong;
}

int GetScreenShort()
{
	int nScreenWidth = ::GetScreenWidth();
	int nScreenHeight = ::GetScreenHeight();
	return min( nScreenWidth, nScreenHeight );
}

int GetScreenWidth()
{
	return ::GetSystemMetrics( SM_CXSCREEN );
}

int GetScreenHeight()
{
	return ::GetSystemMetrics( SM_CYSCREEN );
}

/* char -> wchar_t */
BOOL MBtoWC(const char* srcStr, wchar_t * dstStr, int len )
{
	int length;

	length = MultiByteToWideChar(CP_ACP, 0, 
		srcStr, -1, NULL, 0);

	if ( length < len )
	{
		MultiByteToWideChar(CP_ACP, 0,
			srcStr, -1, dstStr, length);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* wchar_t -> char */
BOOL WCtoMB(const wchar_t* srcStr, char *dstStr, int len )
{
	int charlength;

	charlength = WideCharToMultiByte(CP_ACP, 0, srcStr,
		-1, NULL, 0, NULL, NULL);

	if( charlength < len )
	{
		WideCharToMultiByte(CP_ACP, 0, srcStr,
			-1, dstStr, charlength, NULL, NULL);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//__time64_t StrToTime( LPCTSTR lpTime )
//{
//	CString strTime = lpTime;
//	SYSTEMTIME tTime;
//	memset( &tTime, 0, sizeof( tTime ) );
//
//	if ( strTime.GetLength() >= 4 )
//	{
//		CString strYear = strTime.Left( 4 );
//		tTime.wYear = (WORD)_ttoi( strYear );
//
//		strTime = strTime.Right( strTime.GetLength() - 4 );
//	}
//
//	if ( strTime.GetLength() >= 2 )
//	{
//		CString strTemp = strTime.Left( 2 );
//		tTime.wMonth = (WORD)_ttoi( strTemp );
//
//		strTime = strTime.Right( strTime.GetLength() - 2 );
//	}
//
//	if ( strTime.GetLength() >= 2 )
//	{
//		CString strTemp = strTime.Left( 2 );
//		tTime.wDay = (WORD)_ttoi( strTemp );
//
//		strTime = strTime.Right( strTime.GetLength() - 2 );
//	}
//
//	if ( strTime.GetLength() >= 2 )
//	{
//		CString strTemp = strTime.Left( 2 );
//		tTime.wHour = (WORD)_ttoi( strTemp );
//
//		strTime = strTime.Right( strTime.GetLength() - 2 );
//	}
//
//	if ( strTime.GetLength() >= 2 )
//	{
//		CString strTemp = strTime.Left( 2 );
//		tTime.wMinute = (WORD)_ttoi( strTemp );
//
//		strTime = strTime.Right( strTime.GetLength() - 2 );
//	}
//
//	if ( strTime.GetLength() >= 2 )
//	{
//		CString strTemp = strTime.Left( 2 );
//		tTime.wSecond = (WORD)_ttoi( strTemp );
//
//		strTime = strTime.Right( strTime.GetLength() - 2 );
//	}
//
//	if( ( tTime.wYear >= 1900 ) && ( tTime.wMonth >= 1 && tTime.wMonth <= 12 ) && ( tTime.wDay >= 1 && tTime.wDay <= 31 ) 
//		&& ( tTime.wHour >= 0 && tTime.wHour <= 23 ) && ( tTime.wMinute >= 0 && tTime.wMinute <= 59 ) 
//		&& ( tTime.wSecond >= 0 && tTime.wSecond <= 59 ) )
//	{
//		try
//		{
//			return CTime( tTime ).GetTime();
//		}
//		catch (CException* e)
//		{
//			e;
//			return 0;
//		}
//		catch( ... )
//		{
//			return 0;
//		}
//	}
//	else
//	{
//		return 0;
//	}
//
//	
//	
//}
#endif



BOOL HasStorageCard()
{
#ifdef WIN32_MODE

	LPCTSTR strCardPath = _T( "\\Storage Card\\" );

#else
	LPCTSTR strCardPath = _T( "/media/mmc1" );
#endif
	return IsFileExist( strCardPath );
}

BOOL IsFileExist( LPCTSTR strPath )
{
#ifdef _WIN32
	DWORD dwFA = ::GetFileAttributes( strPath );
	return dwFA != -1;
#else
	BOOL bExist = ( _taccess( strPath, 0 ) == 0 ); 
	return bExist;
#endif
}

BOOL MakeDir( LPCTSTR strDir )
{
#ifdef WIN32_MODE
	return CreateDirectory( strDir, NULL );
#else
	return ( 0 == _tmkdir( strDir, S_IRWXU ) );
#endif
}



BOOL DelFile( LPCTSTR strFile )
{
#ifdef WIN32_MODE
	return ::DeleteFile( strFile );
#else
	int nResult = unlink( strFile );
	return ( 0 == nResult );
#endif
}

int CompareNoCase(const string& s, const string& s2)
{
	string::const_iterator   p   =   s.begin();   
	string::const_iterator   p2   =   s2.begin();   
	while(p!=s.end()   &&   p2!=s2.end())   
	{   
		if(toupper(*p)!=toupper(*p2))   
		{
			return   ( toupper(*p) < toupper(*p2) )  ?   -1:1;   
		}
		++p;   
		++p2;   
	}   
	return   (s2.size()   ==   s.size())?0:(s.size()<s2.size())?-1:1;   
}

int CompareNoCase(const wstring&   s,   const   wstring&   s2)
{
	wstring::const_iterator   p   =   s.begin();   
	wstring::const_iterator   p2   =   s2.begin();   
	while(p!=s.end()   &&   p2!=s2.end())   
	{   
		if(toupper(*p)!=toupper(*p2))   
		{
			return   ( toupper(*p) < toupper(*p2) )  ?   -1:1;   
		}
		++p;   
		++p2;   
	}   
	return   (s2.size()   ==   s.size())?0:(s.size()<s2.size())?-1:1;   
}

mu_uint64 GetTick()
{
#ifdef _WIN32
	return GetTickCount();
#else
	struct timeval tv;
	gettimeofday( &tv, NULL );
	return (mu_uint64)tv.tv_sec * 1000000 + tv.tv_usec;
#endif
}

__time64_t GetCurTime()
{	
#ifdef _WIN32
	return _time64( NULL );
#else
	return time( NULL );
#endif	
}

tstring TimeToStr( __time64_t nTime )
{
    tstring strTimeResult;

    tm localTime;
    if( GetLocalTime( nTime, &localTime ) )
    {
        tstringstream ssTime;
        ssTime << setfill( _T( '0' ) ) << setw(4) << ( localTime.tm_year + 1900 );
        ssTime << setw(2) << localTime.tm_mon + 1 << setw(2) << localTime.tm_mday;
        ssTime << setw(2) << localTime.tm_hour << setw(2) << localTime.tm_min << setw(2) << localTime.tm_sec;
        strTimeResult = ssTime.str();
    }
    return strTimeResult;
}

BOOL GetLocalTime( const time_t time, tm * plocalTime )
{
	BOOL bResult;
#ifdef _WIN32
	bResult = ( 0 == _localtime64_s( plocalTime, &time ) );
#else
	tm *ptm = localtime( &time );
	if( ptm )
	{
		*plocalTime = *ptm;
	}
	bResult = ( NULL != ptm );
#endif
	return bResult;
}

TFileList EnumAllFile( LPCTSTR strFolder )
{
	TFileList tFiles;
#ifdef _WIN32
#else
	DIR * pDir = opendir( strFolder );
	if( NULL != pDir )
	{
		dirent tDirEnt;
		dirent *pFileResult;
//		vpn::log << _T( "Open dir and enum all file: " ) << strFolder  << endl;
		while( TRUE )
		{			
			int nResult = readdir_r( pDir, &tDirEnt, &pFileResult );
			if( pFileResult == NULL )
			{
				break;
			}
			tstring strFilePath = strFolder;
			if( strFilePath.length() > 1 && strFilePath[ strFilePath.length() - 1 ] != '/' ) 
			{
				strFilePath += "/";
			}
			strFilePath += pFileResult->d_name;
			tFiles.push_back( strFilePath );
//			vpn::log << _T( "find file: " ) << pFileResult->d_name << _T( " i-num: " ) << pFileResult->d_ino << endl;
		}

		closedir( pDir );
	}
	else
	{
		vpn::log << _T( "EnumAllFile open dir fail! dir: " ) << strFolder << endl;
	}

#endif
	return tFiles;
}


void NormalizeDir( tstring& strFolder )
{
    if ( strFolder.empty() )
    {
        return;
    }

#if defined( WIN32_MODE )
    const TCHAR chDirToken = '\\';
    const TCHAR chWrongDirToken = '/';
#else
    const TCHAR chDirToken = '/';
    const TCHAR chWrongDirToken = '\\';
#endif

    replace( strFolder.begin(), strFolder.end(), chWrongDirToken, chDirToken );

    if ( strFolder.at( strFolder.size() - 1 ) != chDirToken )
    {
        strFolder += chDirToken;
    }

}



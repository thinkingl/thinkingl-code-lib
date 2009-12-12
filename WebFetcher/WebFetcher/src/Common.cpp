#include "..\include\Common.h"

CCommon::CCommon(void)
{
}

CCommon::~CCommon(void)
{
}

void CCommon::Utf16toUtf8( LPCWSTR strUtf16, string& strUtf8 )
{
	if ( strUtf16 == NULL || *strUtf16 == NULL )
	{
		return ;
	}

	int nLen16 = wcslen( strUtf16 );
	int nLen8 = WideCharToMultiByte( CP_UTF8, 0, strUtf16, nLen16, 0, 0, 0, 0 );
	if ( 0 == nLen8 )
	{
		return;
	}


	std::vector<char> vTemp( nLen8+1, 0 );
	WideCharToMultiByte( CP_UTF8, 0, strUtf16, nLen16, &vTemp[0], nLen8, 0, 0 );

	strUtf8 = &vTemp[0];

	return ;
}

void CCommon::Utf8toUtf16( LPCSTR strUtf8, wstring& strUtf16 )
{
	if ( strUtf8 == NULL || *strUtf8 == NULL )
	{
		return ;
	}

	int nLen8 = strlen( strUtf8 );
	std::vector< wchar_t > vTemp( nLen8+1, 0 );
	MultiByteToWideChar( CP_UTF8, 0, strUtf8, nLen8, &vTemp[0], nLen8 );

	strUtf16 = &vTemp[0];

	return;
}

int CCommon::StrNCmpNocase( LPCTSTR str1, LPCTSTR str2, int nLen )
{
	while( *str1 != NULL && *str2 != NULL && nLen > 0 )
	{
		if ( *str1 != *str2 )
		{
			// try Case..
			TCHAR cNew;
			if ( *str1 >= _T( 'A') && *str1 <= _T( 'Z' ) )
			{
				cNew = *str1 + ( 'a' - 'A' );
			}
			else if( *str1 >= _T( 'a' ) && *str1 <= _T( 'z' ) )
			{
				cNew = *str1 + ( 'A' - 'a' );
			}
			else
			{
				return str1 - str2;
			}

			if ( cNew != *str2 )
			{
				return cNew - *str2;
			}
		}
		str1 ++;
		str2 ++;
		nLen --;
	}

	return ( nLen == 0 ) ? 0 : ( *str1 == NULL ? -1 : 1 );
}

tstring CCommon::GetModulePath( void * hm /*= NULL*/ )
{
	std::vector< wchar_t > vTemp( MAX_PATH+1, 0 );
	::GetModuleFileName( (HMODULE)hm, &vTemp[0], MAX_PATH );

	return &vTemp[0];
}

bool CCommon::TFileNameInfo::operator ==( const CCommon::TFileNameInfo& tAnother ) const
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


CCommon::TFileNameInfo CCommon::ParsePath( LPCTSTR strPath )
{
	TFileNameInfo tInfo;
	ParsePath( strPath, tInfo );
	return tInfo;
}

BOOL CCommon::ParsePath( LPCTSTR strPath, CCommon::TFileNameInfo& tFileNameInfo )
{
	tFileNameInfo.m_strFilePath = strPath;

	tstring strAppPath = strPath;

#if defined( _WIN32_WCE )
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


tstring CCommon::GetAppName()
{

	return ParsePath( GetModulePath().c_str() ).m_strBaseName;

}

tstring CCommon::GetAppDir()
{
	return ParsePath( GetModulePath().c_str() ).m_strDirectory;
}

BOOL CCommon::IsWebpage( LPCTSTR strPath )
{
	tstring strExt = ParsePath( strPath ).m_strExtName;

	LPCTSTR arHtmlFileExt[] = 
	{
		_T( "htm" ),
		_T( "html" ),
		NULL
	};
	
	int i=0;
	while ( arHtmlFileExt[i] )
	{
		if ( StrNCmpNocase( arHtmlFileExt[i], strExt.c_str(), INT_MAX ) == 0 )
		{
			return TRUE;
		}
		++i;
	}

	return FALSE;
}
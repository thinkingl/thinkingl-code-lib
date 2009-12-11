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
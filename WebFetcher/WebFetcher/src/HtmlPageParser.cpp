#include "HtmlPageParser.h"
#include "Log.h"
#include "Common.h"

CHtmlPageParser::CHtmlPageParser( )
{
	
}

CHtmlPageParser::~CHtmlPageParser(void)
{
}

static LPCTSTR s_arToken[] = 
{
	_T( "href=" ),
	_T( "src=" ),
	NULL

};

BOOL CHtmlPageParser::Parse( LPCTSTR strHtmlFilePath, LPCTSTR strHtmlServerUrl )
{
	// 保留路径。
	m_strHtmlPageLocalPath = strHtmlFilePath;

	// 解析url。
	this->m_strServerUrl = strHtmlServerUrl;

	tstring strUrl = strHtmlServerUrl;
	int nPos = strUrl.rfind( _T( '/' ) );
	this->m_strServerUrlFolder = strUrl.substr( 0, nPos );

	if ( this->m_strServerUrlFolder.empty() )
	{
		Log() << _T( "Server url is empty!!!!" ) << endl;
		_ASSERT( FALSE );
		return FALSE;
	}

	fstream fPage;
	fPage.open( strHtmlFilePath );
	if ( !fPage )
	{
		Log() << _T( "Can't open file: " ) << strHtmlFilePath << endl;
		return FALSE;
	}
	else
	{
		string strData;
		int nUrlPos = 0;
		while( fPage >> strData )
		{
			wstring strUtf16;
			CCommon::Utf8toUtf16( strData.c_str(), strUtf16 );
			

			if ( this->IsContainUrl( strUtf16.c_str() ) )
			{
				Log() << strUtf16 << endl;
				//
				tstring strOriginalUrl, strFullUrl;
				int nPos = 0;
				if( GetUrl( strUtf16.c_str(), strFullUrl, strOriginalUrl, nPos ) )
				{
					int nCurUrlPos = nUrlPos + nPos;
					Log() << strFullUrl << _T( " pos: " ) << nCurUrlPos << endl;

					// save.
					TFullUrlOrignalUrl tPairUrl( strFullUrl, strOriginalUrl );
					this->m_tUrlSet.insert( strFullUrl );
					this->m_tUrlPosTable[ nCurUrlPos ] = tPairUrl;
				}
				else
				{
					Log() << _T( "Can't parse the url! : " ) << strUtf16 << endl;
//					_ASSERT( FALSE );
				}				
				
			}

			nUrlPos += strUtf16.length();
			
		}
	}

	fPage.close();
	
	return TRUE;
}

BOOL CHtmlPageParser::GetAllUrl( TUrlList& tUrlList )
{
	tUrlList.clear();
	for ( TUrlSet::iterator iter = this->m_tUrlSet.begin(); 
		iter != this->m_tUrlSet.end();
		++iter )
	{
		tUrlList.push_back( *iter );
	}
	return TRUE;
}

BOOL CHtmlPageParser::ReplaceAllUrl( LPCTSTR strSrcUrl, LPCTSTR strDstUrl )
{
	m_tReplaceTable[ strSrcUrl ] = strDstUrl;
	return TRUE;
}

BOOL CHtmlPageParser::SaveFile( LPCTSTR strPath )
{
	ifstream fPageSrc;
	fPageSrc.open( this->m_strHtmlPageLocalPath.c_str() );

	ofstream fPageDst;
	fPageDst.open( strPath );

	if ( !fPageSrc || !fPageDst )
	{
		Log() << _T( "SaveFile Open file Fail!!!!!" ) << endl;
		_ASSERT( FALSE );
		return FALSE;
	}

	int nPosCount = 0;
	string strData;
	TUrlPosTable::iterator iter = this->m_tUrlPosTable.begin();
	while ( fPageSrc >> strData )
	{
		wstring strUtf16;
		CCommon::Utf8toUtf16( strData.c_str(), strUtf16 );

		int nLen = strUtf16.length();

		if ( iter != this->m_tUrlPosTable.end() 
			&& nPosCount < iter->first && nPosCount + nLen > iter->first )
		{
			// 替换表中存储的是完整的url。
			tstring strRepUrl = iter->second.first;
			// 在原始数据中被替换的是原始的url。
			tstring strOriUrl = iter->second.second;
			TUrlReplaceTable::iterator iterRep = this->m_tReplaceTable.find( strRepUrl );
			if ( iterRep != this->m_tReplaceTable.end() )
			{
				tstring strDstUrl = iterRep->second;
				int nUrlPos = iter->first - nPosCount;

				tstring strHead = strUtf16.substr( 0, nUrlPos );
				tstring strEnd = strUtf16.substr( nUrlPos + strOriUrl.length() );

				tstring strFinal = strHead + strDstUrl + strEnd;

				CCommon::Utf16toUtf8( strFinal.c_str(), strData );
			}
			

			++iter;
		}

		fPageDst << strData << " ";

		nPosCount += nLen;
		
	}
	return FALSE;
}

BOOL CHtmlPageParser::IsContainUrl( LPCTSTR strLine )
{
	int i=0;
	while ( s_arToken[i] )
	{
		LPCTSTR strToken = s_arToken[i];
		int nTokenLen = _tcslen( strToken ) ;
		if ( 0 == CCommon::StrNCmpNocase( strToken, strLine, nTokenLen ) )
		{
			return TRUE;
		}

		++i;
	}

	return FALSE;
}

BOOL CHtmlPageParser::GetUrl( LPCTSTR strLine, tstring& strFullUrl, tstring& strOriginalUrl, int &nPos )
{
	// 2个引号中间的是url。
	LPCTSTR strUrlContainer = strLine;
	const TCHAR cToken1 = _T( '"' );
	const TCHAR cToken2 = _T( '\'' );
	int nCount = 0;
	nPos = 0;
	tstringstream ssUrl;
	while ( *strUrlContainer != NULL && nCount < 2 )
	{
		if ( nCount < 1 )
		{
			nPos ++;	// 没遇到引号之前寻找位置。
		}

		if ( cToken1 == *strUrlContainer || cToken2 == *strUrlContainer )
		{
			nCount ++;
		}
		else if ( nCount == 1 )
		{
			ssUrl << *strUrlContainer;
		}




		strUrlContainer ++;
	}
	if ( nCount < 2 )
	{
		ssUrl.clear();
	}
	
	strOriginalUrl = ssUrl.str();
	strFullUrl = strOriginalUrl;
	if ( strOriginalUrl.empty() )
	{
		return FALSE;
	}
	else
	{
		if( strOriginalUrl[0] == '#' )
		{
			strOriginalUrl.clear();
			strFullUrl.clear();
			return FALSE;
		}
		LPCTSTR lpstrJS = _T( "javascript" );
		if( 0 == CCommon::StrNCmpNocase( lpstrJS, strOriginalUrl.c_str(), _tcslen( lpstrJS ) ) )
		{
			strOriginalUrl.clear();
			strFullUrl.clear();
			return FALSE;
		}

		LPCTSTR strHttp = _T( "http://" );
		if ( 0 != CCommon::StrNCmpNocase( strHttp, strOriginalUrl.c_str(), _tcslen( strHttp ) ) )
		{
			if ( strOriginalUrl[0] != '/' )
			{
				strFullUrl = tstring( _T( "/" ) ) + strOriginalUrl;
			}
			
			strFullUrl = this->m_strServerUrlFolder + strFullUrl;
		}
	}

	

	return !strFullUrl.empty();
}

tstring CHtmlPageParser::GetCurServerUrl()
{
	return this->m_strServerUrl;
}



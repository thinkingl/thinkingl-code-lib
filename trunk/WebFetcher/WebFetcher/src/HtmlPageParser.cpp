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
	// ����·����
	m_strHtmlPageLocalPath = strHtmlFilePath;

	// ����url��
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

	ifstream fPage;
	fPage.open( strHtmlFilePath );
	if ( !fPage )
	{
		Log() << _T( "Can't open file: " ) << strHtmlFilePath << endl;
		return FALSE;
	}
	else
	{
		string strData;
		
		// ȡ���ļ����ȡ�
		int nBeginPos = fPage.tellg();
		fPage.seekg( 0,ios::end );
		int nEndPos = fPage.tellg();
		int nFileLen = nEndPos - nBeginPos;

		if ( nFileLen == 0 )
		{
			CLog() << _T( "webpage file len is 0 !!!!" ) << endl;
			return FALSE;
		}

		vector<char> tCharTmp( nFileLen, 0 );
		int nUrlPos = 0;

		fPage.seekg( 0, ios::beg );
		fPage.read( &tCharTmp[0], nFileLen );
		strData = &tCharTmp[0];

		wstring strUtf16;
		CCommon::Utf8toUtf16( strData.c_str(), strUtf16 );

		int nStrLen = strUtf16.length();

		for ( int i=0; s_arToken[i]; ++i )
		{
			LPCTSTR strUrlToken = s_arToken[i];
			// ���ұ�־��
			int nFindCursor = 0;
			int nPos = strUtf16.find( strUrlToken, nFindCursor );
			// ������һ�Ρ�
			// URL�ڽ��ӵ������
			// Ҫ���ǲ���ҳ�����Ŀ��ܡ�
			int nUrlPos =  nPos + _tcslen( strUrlToken );
			TCHAR quotes = strUtf16.at( nUrlPos );
			BOOL bRightQuotes = FALSE;
			switch ( quotes )
			{
			case _T( '\'' ):
			case _T( '"' ):
				bRightQuotes = TRUE;
				break;
			default:
				CLog() << _T( "Parse url error! token: " ) << quotes << endl;
				bRightQuotes = FALSE;
				break;
			}

			if ( !bRightQuotes )
			{
				continue;
			}
			else
			{
				// ������������������url��
				nUrlPos ++;

				int nUrlBegin = nUrlPos;
				// 
				tstringstream ssUrl;
				while( nUrlPos < nStrLen && strUtf16.at( nUrlPos ) != quotes )
				{
					ssUrl << strUtf16.at( nUrlPos );
					nUrlPos ++;
				}

				tstring strUrl = ssUrl.str();
				// save.
				TFullUrlOrignalUrl tPairUrl( strFullUrl, strOriginalUrl );
				this->m_tUrlSet.insert( strFullUrl );
				this->m_tUrlPosTable[ nUrlBegin ] = tPairUrl;
			}
		}

//		while( fPage >> strData )
//		{
//			wstring strUtf16;
//			CCommon::Utf8toUtf16( strData.c_str(), strUtf16 );
//			
//
//			if ( this->IsContainUrl( strUtf16.c_str() ) )
//			{
//				Log() << strUtf16 << endl;
//				//
//				tstring strOriginalUrl, strFullUrl;
//				int nPos = 0;
//				if( GetUrl( strUtf16.c_str(), strFullUrl, strOriginalUrl, nPos ) )
//				{
//					int nCurUrlPos = nUrlPos + nPos;
//					Log() << strFullUrl << _T( " pos: " ) << nCurUrlPos << endl;
//
//					// save.
//					TFullUrlOrignalUrl tPairUrl( strFullUrl, strOriginalUrl );
//					this->m_tUrlSet.insert( strFullUrl );
//					this->m_tUrlPosTable[ nCurUrlPos ] = tPairUrl;
//				}
//				else
//				{
//					Log() << _T( "Can't parse the url! : " ) << strUtf16 << endl;
////					_ASSERT( FALSE );
//				}				
//				
//			}
//
//			nUrlPos += strUtf16.length();
//			
//		}
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
			// �滻���д洢����������url��
			tstring strRepUrl = iter->second.first;
			// ��ԭʼ�����б��滻����ԭʼ��url��
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
	// 2�������м����url��
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
			nPos ++;	// û��������֮ǰѰ��λ�á�
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



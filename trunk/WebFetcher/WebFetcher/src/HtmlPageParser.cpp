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

	ifstream fPage;
	fPage.open( strHtmlFilePath, ios::in | ios::binary );
	if ( !fPage )
	{
		Log() << _T( "Can't open file: " ) << strHtmlFilePath << endl;
		return FALSE;
	}
	else
	{
		string strData;
		
		// 取得文件长度。
		int nBeginPos = fPage.tellg();
		fPage.seekg( 0,ios::end );
		int nEndPos = fPage.tellg();
		int nFileLen = nEndPos - nBeginPos;

		if ( nFileLen == 0 )
		{
			Log() << _T( "webpage file len is 0 !!!!" ) << endl;
			return FALSE;
		}

		// 一定要为字符串保留一个0结尾！文件不会读取到0结尾！！！
		vector<char> tCharTmp( nFileLen + 1, 0 );
//		int nUrlPos = 0;

		fPage.seekg( 0, ios::beg );
		fPage.read( &tCharTmp[0], nFileLen  );
		
		strData = &tCharTmp[0];

//		string strkkkkk = strData.substr( strData.length() - 50 );

		wstring strUtf16;
		CCommon::Utf8toUtf16( strData.c_str(), strUtf16 );

		// 保存网页内容。
		this->m_strHtmlPageContent = strUtf16;

		int nStrLen = strUtf16.length();

		for ( int i=0; s_arToken[i]; ++i )
		{
			LPCTSTR strUrlToken = s_arToken[i];
			// 查找标志。
			int nFindCursor = 0;
			while ( nFindCursor < nStrLen )
			{
				nFindCursor = strUtf16.find( strUrlToken, nFindCursor );
				if ( -1 == nFindCursor )
				{
					break;
				}
				// 解析这一段。
				// URL在紧接的引号里。
				// 要考虑部分页面错误的可能。
				nFindCursor += _tcslen( strUrlToken );
				// 当前的位置应该是引号。
				TCHAR quotes = strUtf16.at( nFindCursor );
				BOOL bRightQuotes = FALSE;
				switch ( quotes )
				{
				case _T( '\'' ):
				case _T( '"' ):
					bRightQuotes = TRUE;
					break;
				default:
					Log() << _T( "Parse url error! token: " ) << quotes << endl;
					bRightQuotes = FALSE;
					break;
				}

				if ( !bRightQuotes )
				{
					continue;
				}
				else
				{
					// 两个引号引起来的是url。
					nFindCursor ++;

					// 保存下url起始的位置。
					int nUrlBegin = nFindCursor;
					// 
					tstringstream ssUrl;
					while( nFindCursor < nStrLen && strUtf16.at( nFindCursor ) != quotes )
					{
						ssUrl << strUtf16.at( nFindCursor );
						nFindCursor ++;
					}
					// 跳过引号。
					nFindCursor ++;

					tstring strOriginalUrl = ssUrl.str();
					tstring strFullUrl;
					if ( this->GetFullUrl( strOriginalUrl.c_str(), strFullUrl ) )
					{
						//if ( tstring( strUrlToken ) == _T( "href=" ) )
						//{
						//	if ( this->IsInScript( nUrlBegin ) )
						//	{
						//		Log() << _T( "Href url is in Script !! Ignore it!!!!" ) << endl;
						//		continue;
						//	}
						//}
						if ( this->IsUrl( strFullUrl.c_str() ) )
						{
							// save.
							TFullUrlOrignalUrl tPairUrl( strFullUrl, strOriginalUrl );
							this->m_tUrlSet.insert( strFullUrl );
							this->m_tUrlPosTable[ nUrlBegin ] = tPairUrl;

							Log() << _T( "Find url: " ) << strFullUrl << endl;
						}
						else
						{
							Log() << _T( "url is not invalid!!! " ) << strFullUrl << endl;
						}
						
					}
					else
					{
						Log() << _T( "Get full url fail!! url: " ) << strOriginalUrl << endl;
					}
					
				}
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
	tstring strDir = CCommon::ParsePath( strPath ).m_strDirectory;
	CCommon::CreateDirRecurse( strDir.c_str() );

	ofstream fPageDst;
	fPageDst.open( strPath );
	if ( !fPageDst )
	{
		Log() << _T( "CHtmlPageParser::SaveFile Open file fail! " ) << strPath << endl;
		return FALSE;
	}

	tstringstream ssReplacedHtmlPage;
	for( size_t nCurPos = 0; nCurPos < this->m_strHtmlPageContent.length(); ++nCurPos )
	{
		TUrlPosTable::iterator iterPos = this->m_tUrlPosTable.find( nCurPos );

		BOOL bReplace = FALSE;
		if ( iterPos != this->m_tUrlPosTable.end() )
		{
			TCHAR cCurPosWord = this->m_strHtmlPageContent.at( nCurPos );
			// 替换掉这个url。
			// 替换表中存储的是完整的url。
			tstring strRepUrl = iterPos->second.first;
			// 在原始数据中被替换的是原始的url。
			tstring strOriUrl = iterPos->second.second;
			TUrlReplaceTable::iterator iterRep = this->m_tReplaceTable.find( strRepUrl );
			if ( iterRep != this->m_tReplaceTable.end() )
			{
				tstring strDstUrl = iterRep->second;
				ssReplacedHtmlPage << strDstUrl;

//				tstring tmp = ssReplacedHtmlPage.str();
//				tstring tmp222 = tmp.substr(  tmp.length() - 40 );

				// 直接跳到url后面。
				nCurPos += strOriUrl.length();
				// 循环中会+1，所以这里先减去。
				nCurPos --;

//				tstring strTmp0 = this->m_strHtmlPageContent.substr( nCurPos - 50 );
//				tstring strTmp = this->m_strHtmlPageContent.substr( nCurPos, 10 );

				bReplace = TRUE;
			}
		}
		
		if( !bReplace )
		{
			// 继续。
			ssReplacedHtmlPage << this->m_strHtmlPageContent.at( nCurPos );
			
		}
	}

//	tstring strXXX = ssReplacedHtmlPage.str().substr( ssReplacedHtmlPage.str().length() - 50 );

	string strUtf8Page;
	CCommon::Utf16toUtf8( ssReplacedHtmlPage.str().c_str() , strUtf8Page );

//	string strtmppp = strUtf8Page.substr( strUtf8Page.length() - 50 );

	
	fPageDst.write( strUtf8Page.c_str(), strUtf8Page.length() );
	fPageDst.close();
	return TRUE;

	//int nPosCount = 0;
	//string strData;
	//TUrlPosTable::iterator iter = this->m_tUrlPosTable.begin();
	//while ( fPageSrc >> strData )
	//{
	//	wstring strUtf16;
	//	CCommon::Utf8toUtf16( strData.c_str(), strUtf16 );

	//	int nLen = strUtf16.length();

	//	if ( iter != this->m_tUrlPosTable.end() 
	//		&& nPosCount < iter->first && nPosCount + nLen > iter->first )
	//	{
	//		// 替换表中存储的是完整的url。
	//		tstring strRepUrl = iter->second.first;
	//		// 在原始数据中被替换的是原始的url。
	//		tstring strOriUrl = iter->second.second;
	//		TUrlReplaceTable::iterator iterRep = this->m_tReplaceTable.find( strRepUrl );
	//		if ( iterRep != this->m_tReplaceTable.end() )
	//		{
	//			tstring strDstUrl = iterRep->second;
	//			int nUrlPos = iter->first - nPosCount;

	//			tstring strHead = strUtf16.substr( 0, nUrlPos );
	//			tstring strEnd = strUtf16.substr( nUrlPos + strOriUrl.length() );

	//			tstring strFinal = strHead + strDstUrl + strEnd;

	//			CCommon::Utf16toUtf8( strFinal.c_str(), strData );
	//		}
	//		

	//		++iter;
	//	}

	//	fPageDst << strData << " ";

	//	nPosCount += nLen;
	//	
	//}
	//return FALSE;
}

//BOOL CHtmlPageParser::IsContainUrl( LPCTSTR strLine )
//{
//	int i=0;
//	while ( s_arToken[i] )
//	{
//		LPCTSTR strToken = s_arToken[i];
//		int nTokenLen = _tcslen( strToken ) ;
//		if ( 0 == CCommon::StrNCmpNocase( strToken, strLine, nTokenLen ) )
//		{
//			return TRUE;
//		}
//
//		++i;
//	}
//
//	return FALSE;
//}

BOOL CHtmlPageParser::GetFullUrl( LPCTSTR lpstrOriginalUrl, tstring& strFullUrl )
{
	tstring strOriginalUrl = lpstrOriginalUrl;
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

		LPCTSTR arProtocolHead[] = 
		{
			_T( "http://" ),
			_T( "ftp://" ),
			_T( "file://" ),
			_T( "ed2k://" ),
			NULL
		};
		LPCTSTR strProtocol = NULL;
		for ( int i=0; arProtocolHead[i] != NULL; ++i )
		{
			if ( 0 == CCommon::StrNCmpNocase( arProtocolHead[i], strOriginalUrl.c_str(), _tcslen( arProtocolHead[i] ) ) )
			{
				strProtocol = arProtocolHead[i];
				break;
			}
		}
		if ( NULL == strProtocol )
		{
			// 原始url不是绝对url，需要拼接。

			// 将Url中的"\"都替换为"/".
			CCommon::NormalizeUrl( strOriginalUrl );

			tstring strBaseFolder = this->m_strServerUrlFolder;
			CCommon::NormalizeUrl( strBaseFolder );

			// 解析服务器域名。
			tstring strServerUrl;
			int nPos = strBaseFolder.find( '/', _tcslen( _T( "http://" ) ) );
			if ( nPos != -1 )
			{
				strServerUrl = strBaseFolder.substr( 0, nPos );
			}
			else
			{
				strServerUrl = strBaseFolder;
			}


			// / 表示路径是相对于网站根目录的。
			if ( strOriginalUrl[0] == '/' )
			{
				strOriginalUrl = strOriginalUrl.substr( 1 );

				strBaseFolder = strServerUrl;
			}

			if ( strBaseFolder.at( strBaseFolder.length() - 1 ) == '/' )
			{
				strBaseFolder = strBaseFolder.substr( 0, strBaseFolder.length() - 1 );
			}

			LPCTSTR PathCurrent = _T( "./" );
			LPCTSTR PathParent = _T( "../" );
		
			

			while( 1 )
			{
				if ( 0 == CCommon::StrNCmpNocase( PathCurrent, strOriginalUrl.c_str(), _tcslen( PathCurrent ) ) )
				{
					strOriginalUrl = strOriginalUrl.substr( _tcslen( PathCurrent ) );
				}
				else if ( 0 == CCommon::StrNCmpNocase( PathParent, strOriginalUrl.c_str(), _tcslen( PathParent ) ) )
				{
					strOriginalUrl = strOriginalUrl.substr( _tcslen( PathParent ) );

					// 上一级。
					int nPos = strBaseFolder.rfind( '/' );
					if ( -1 != nPos )
					{
						strBaseFolder = strBaseFolder.substr( 0, nPos );
					}
					else
					{
						ASSERT( FALSE );
						return FALSE;
					}
				}
				else
				{
					break;
				}
			}
			

			strFullUrl = strBaseFolder + _T( "/" ) + strOriginalUrl;
		}
	}
	return TRUE;
}

//BOOL CHtmlPageParser::GetUrl( LPCTSTR strLine, tstring& strFullUrl, tstring& strOriginalUrl, int &nPos )
//{
//	// 2个引号中间的是url。
//	LPCTSTR strUrlContainer = strLine;
//	const TCHAR cToken1 = _T( '"' );
//	const TCHAR cToken2 = _T( '\'' );
//	int nCount = 0;
//	nPos = 0;
//	tstringstream ssUrl;
//	while ( *strUrlContainer != NULL && nCount < 2 )
//	{
//		if ( nCount < 1 )
//		{
//			nPos ++;	// 没遇到引号之前寻找位置。
//		}
//
//		if ( cToken1 == *strUrlContainer || cToken2 == *strUrlContainer )
//		{
//			nCount ++;
//		}
//		else if ( nCount == 1 )
//		{
//			ssUrl << *strUrlContainer;
//		}
//
//
//
//
//		strUrlContainer ++;
//	}
//	if ( nCount < 2 )
//	{
//		ssUrl.clear();
//	}
//	
//	strOriginalUrl = ssUrl.str();
//	strFullUrl = strOriginalUrl;
//	if ( strOriginalUrl.empty() )
//	{
//		return FALSE;
//	}
//	else
//	{
//		if( strOriginalUrl[0] == '#' )
//		{
//			strOriginalUrl.clear();
//			strFullUrl.clear();
//			return FALSE;
//		}
//		LPCTSTR lpstrJS = _T( "javascript" );
//		if( 0 == CCommon::StrNCmpNocase( lpstrJS, strOriginalUrl.c_str(), _tcslen( lpstrJS ) ) )
//		{
//			strOriginalUrl.clear();
//			strFullUrl.clear();
//			return FALSE;
//		}
//
//		LPCTSTR strHttp = _T( "http://" );
//		if ( 0 != CCommon::StrNCmpNocase( strHttp, strOriginalUrl.c_str(), _tcslen( strHttp ) ) )
//		{
//			if ( strOriginalUrl[0] != '/' )
//			{
//				strFullUrl = tstring( _T( "/" ) ) + strOriginalUrl;
//			}
//			
//			strFullUrl = this->m_strServerUrlFolder + strFullUrl;
//		}
//	}
//
//	
//
//	return !strFullUrl.empty();
//}

tstring CHtmlPageParser::GetCurServerUrl()
{
	return this->m_strServerUrl;
}

BOOL CHtmlPageParser::IsUrl( LPCTSTR lpstrUrl )
{
	tstring strUrl = lpstrUrl;

	// 临时。。。
	if ( strUrl.find( _T( " + " ) ) != -1 )
	{
		return FALSE;
	}

	if ( strUrl.find( _T( "][" ) ) != -1 )
	{
		return FALSE;
	}

	if ( strUrl.find( _T( "'" ) ) != -1 )
	{
		return FALSE;
	}

	if ( strUrl.find( _T( "\"" ) ) != -1 )
	{
		return FALSE;
	}

	if ( strUrl.find( _T( "/comments/" ) ) != -1  )
	{
		return FALSE;
	}

	LPCTSTR strE2kProtocol = _T( "ed2k://" );
	if ( CCommon::StrNCmpNocase( strE2kProtocol, strUrl.c_str(), _tcslen( strE2kProtocol ) ) == 0 )
	{
		// 电骡链接，不是需要的url。
		return FALSE;
	}

	return TRUE;
}

BOOL CHtmlPageParser::IsInScript( int nPos )
{
	// 在网页内容中找标签。
	// 假定 <script 标签都是小写的！！！！

	int nScriptStartPos = this->m_strHtmlPageContent.rfind( _T( "<script" ), nPos );
	if ( -1 == nScriptStartPos )
	{
		return FALSE;
	}

	int nScriptEndPos = this->m_strHtmlPageContent.rfind( _T( "</script" ), nPos );
	if ( -1 == nScriptEndPos )
	{
		return TRUE;
	}

	return nScriptEndPos < nScriptStartPos;

}


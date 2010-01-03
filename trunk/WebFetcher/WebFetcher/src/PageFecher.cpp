#include "..\include\PageFecher.h"
#include "Log.h"
#include "IWebpageManager.h"
#include "ClassFactory.h"
#include "Common.h"
#include "IConfig.h"
#include <io.h>

CPageFecher::CPageFecher(void)
{
	this->m_pHtmlPageParser = NULL;
	this->m_pHtmlDownloader = NULL;

	this->m_bHasWork = TRUE;
}

CPageFecher::~CPageFecher(void)
{
	if( this->m_pHtmlDownloader )
	{
		delete m_pHtmlDownloader;
		m_pHtmlDownloader = NULL;
	}

	if( this->m_pHtmlPageParser )
	{
		delete m_pHtmlPageParser;
		m_pHtmlPageParser = NULL;
	}
}

BOOL CPageFecher::FetchOnePage()
{
	Log() << _T( "CPageFecher Fetch One Page!" ) << endl;

	if ( m_pHtmlDownloader )
	{
//		delete m_pHtmlDownloader;
//		m_pHtmlDownloader = NULL;
	}

	// �Ƿ�����Ҫ���ص�url��
	if ( !m_tUrlWaitingDownloadStack.empty() )
	{
		tstring strUrl = this->m_tUrlWaitingDownloadStack.top();

		// ������������ϵ�ҳ�����ء�
		m_tUrlWaitingDownloadStack.pop();
		Log() << _T( "waiting download queue pop." ) << endl;

		Log() << _T( "Find url wait for download! url: " ) << strUrl << endl;

		BOOL bHaveDownload, bShouldDownload, bNetworkOk;
		tstring strCachePath, strSavePath;
		BOOL bRet = this->GetUrlInfo( strUrl.c_str(), bNetworkOk, bHaveDownload, bShouldDownload,
			strCachePath, strSavePath, &m_pHtmlDownloader );


		if ( bRet )
		{
			Log() << _T( "GetUrlInfo url: " ) << strUrl
				<< _T( " network: " ) << bNetworkOk << _T( " bHaveDownload " ) << bHaveDownload 
				<< _T( " bShouldDownload " ) << bShouldDownload << _T( " strCachePath " ) << strCachePath
				<< _T( " strSavePath " ) << strSavePath << _T( " m_pHtmlDownloader " ) << m_pHtmlDownloader 
				<< endl;
			if( bShouldDownload )
			{
				if( !bHaveDownload )
				{
					// ���ء�
					ASSERT( m_pHtmlDownloader );
					if( m_pHtmlDownloader )
					{
						BOOL bDownload = m_pHtmlDownloader->DownloadFile( strCachePath.c_str() );
//						ASSERT( bDownload );
						if ( bDownload )
						{
							bHaveDownload = TRUE;

							IWebpageManager::Instance()->CachePageUrl( strUrl.c_str() );
						}
						else
						{
							Log() << _T( "DownloadFile fail!!!!! url: " ) << strUrl << endl;
						}
					}
				}
				
				// ���¡�
				if ( bHaveDownload && this->m_pHtmlPageParser )
				{
					BOOL bReplaceRet = this->ReplaceUrl( strUrl.c_str(), strSavePath.c_str() );
					ASSERT( bReplaceRet );
				}
				else if( m_pHtmlPageParser )
				{
					// ����Ҫ����Ҳ�滻һ�£�����ҳ���ϾͶ���������url�ˡ�
					BOOL bReplaceRet = this->m_pHtmlPageParser->ReplaceAllUrl( strUrl.c_str(), strUrl.c_str() );
					ASSERT( bReplaceRet );
				}
					
			}
			else if ( m_pHtmlPageParser )
			{
				// ����Ҫ����Ҳ�滻һ�£�����ҳ���ϾͶ���������url�ˡ�
				BOOL bReplaceRet = this->m_pHtmlPageParser->ReplaceAllUrl( strUrl.c_str(), strUrl.c_str() );
				ASSERT( bReplaceRet );
			}
		}
		else
		{
			Log() << _T( "GetUrlInfo fail!!!" ) << endl;

			this->m_bHasWork = FALSE;
			return FALSE;
		}
	

	}
	else if( m_pHtmlPageParser )
	{
		// �Ѿ�û����Ҫ���ص�url�ˡ�
		// �����ҳ�Ѿ�������ϣ����Ա����ˡ�
//		ASSERT( FALSE );

		tstring strUrl = this->m_pHtmlPageParser->GetCurServerUrl();
		// ���浽�����ļ��С�
		tstring strCache, strFinalPath;
		BOOL bSp = IWebpageManager::Instance()->SearchPagePath( strUrl.c_str(), strCache, strFinalPath );
		ASSERT( bSp );
		ASSERT( m_pHtmlPageParser );
		if ( bSp && m_pHtmlPageParser )
		{
			bSp &= this->m_pHtmlPageParser->SaveFile( strFinalPath.c_str() );
			ASSERT( bSp );

			delete m_pHtmlPageParser;
			m_pHtmlPageParser = NULL;

			bSp &= IWebpageManager::Instance()->CachedPageToSavedPage( strUrl.c_str() );
		}

		// ɾ�������ļ���
		BOOL bDel = ::DeleteFile( strCache.c_str() );
		ASSERT( bDel );

		Log() << _T( "Url is completed!!! Save to final path!!! url: " ) <<
			strUrl << _T( " save to " ) <<  strFinalPath << endl;
	}
	else
	{
		// ��ȡһ����Ҫ�����ҳ�档 
		tstring strUrl, strLocalPath;

		// �����ڻ����л�ȡҳ���Դ���
		if ( IWebpageManager::Instance()->GetCachedPage( strUrl, strLocalPath ) )
		{
			// ��������ļ���
			if ( 0 != _taccess( strLocalPath.c_str(), 0 ) )
			{
				// ����ļ������ڣ�������
				IWebpageManager::Instance()->UnCachePageUrl( strUrl.c_str() );
				Log() << _T( "Cache file don't exist!!! file: " ) << strLocalPath
					<< _T( " url: " ) << strUrl << endl;
			}

			// �ж��Ƿ�����ҳ��
			if ( CCommon::IsWebpage( strLocalPath.c_str() ) )
			{
				this->m_pHtmlPageParser = CClassFactory::CreateHtmlPageParser();
				BOOL bRet = this->m_pHtmlPageParser->Parse( strLocalPath.c_str(), strUrl.c_str() );
	//			ASSERT( bRet );

				if ( bRet )
				{
					IHtmlPageParser::TUrlList tUrlList;
					bRet = this->m_pHtmlPageParser->GetAllUrl( tUrlList );
					ASSERT( bRet );
					for ( size_t i=0; i<tUrlList.size(); ++i )
					{
						this->m_tUrlWaitingDownloadStack.push( tUrlList[i] );
					}
				}
				else
				{
					Log() << _T( "Parse webpage fail!!!" ) << strLocalPath << endl;

					// ���������ֱ�����ٽ����������ٴ������ҳ���ˡ�
					delete m_pHtmlPageParser;
					m_pHtmlPageParser = NULL;
				}
				
			}
			else
			{
				// ����ļ�������ҳ��ֱ�ӱ����ȥ�Ϳ����ˡ�
				// ��������ļ������ձ���Ŀ¼������¼��
//				ASSERT( FALSE );

				// ���浽�����ļ��С�
				tstring strCache, strFinalPath;
				BOOL bSp = IWebpageManager::Instance()->SearchPagePath( strUrl.c_str(), strCache, strFinalPath );
				ASSERT( bSp );
				
				tstring strFinalDir = CCommon::ParsePath( strFinalPath.c_str() ).m_strDirectory;
				CCommon::CreateDirRecurse( strFinalDir.c_str() );

				BOOL bCp = ::MoveFile( strCache.c_str(), strFinalPath.c_str() );

				if( !bCp )
				{
					Log() << _T( "Move file fail! copy it! file: " ) << strCache 
						<< _T( " to " ) << strFinalPath << endl;
					bCp = ::CopyFile( strCache.c_str(), strFinalPath.c_str(), FALSE );
				}
//				ASSERT( bCp );

				IWebpageManager::Instance()->CachedPageToSavedPage( strUrl.c_str() );

				Log() << _T( "Url is a file not page, copy direct! url: " ) << strUrl 
					<< _T( " from " )<< strCache << _T( " to " ) << strFinalPath 
					<< _T( " copy ret: " ) << bCp << endl;

			}
			

			
		}
		else
		{
			// ��ȡ��ҳ��
			tstring strRootUrl = IConfig::Instance()->GetRootUrl();
			Log() << _T( "No cached web page, deal with the root url: " ) << strRootUrl << endl;

			tstring strRootUrlLocalPath;
			if ( IWebpageManager::Instance()->GetPageLocalFilePath( strRootUrl.c_str(), strRootUrlLocalPath ) )
			{
				// ��ҳ��ȡ���ˡ�������
				Log() << _T( "No cached web page and the root page had been fetched! Fetch work done!!" ) << endl;
				this->m_bHasWork = FALSE;
			}
			else
			{
				// ��ҳû�л�ȡ������ȡ��ҳ��
				// ����ҳ�����ȡ�б���һ���н��ᱻ����
				this->m_tUrlWaitingDownloadStack.push( strRootUrl );
				Log() << _T( "The Root Url : " ) << strRootUrl << _T( " has been push to download queue!" ) << endl;
			}
		}

		
	}

	

	return TRUE;
}

BOOL CPageFecher::HasPageWaiting()
{
	return this->m_bHasWork;

}

BOOL CPageFecher::IsUrlShouldDownload( LPCTSTR strUrl, const CMimeType& mimetype )
{
	BOOL bShould = FALSE;

	// �ж����ǲ���html��ҳ��
	// 
//	Log() << _T( "url should not tested!" ) << endl;
//	ASSERT( FALSE );
	if ( CMimeType::HttpMimeHtmlHtm == mimetype.GetMimeType() )
	{
		// ��ȡ����url�б� 
		tstringarray tar = IConfig::Instance()->GetAllFetchFilter();
		for ( size_t i=0; i<tar.size(); ++i  )
		{
			tstring strFilter = tar[i];

			if ( CCommon::StrNCmpNocase( strFilter.c_str(), strUrl, strFilter.length() ) == 0 )
			{
				BOOL bShouldFetch = IConfig::Instance()->IsUrlFilterFetch( strFilter.c_str() );

				Log() << _T( "url ") << strUrl << _T( " is filterd to fetch: " ) << bShouldFetch << endl;
				bShould = bShouldFetch;
			}
		}
	}
	else
	{
		bShould = TRUE;
	}

	
	return bShould;
}

BOOL CPageFecher::IsPageNeedParse( LPCTSTR strUrl )
{
	ASSERT( FALSE );
	return TRUE;
}

BOOL CPageFecher::ReplaceUrl( LPCTSTR strSrcUrl, LPCTSTR strDstUrl )
{
	ASSERT( m_pHtmlPageParser );
	if ( this->m_pHtmlPageParser )
	{
		tstring strParentUrl = this->m_pHtmlPageParser->GetCurServerUrl();
		tstring strParentCachePath, strParentSavePath;
		// ��ѯ��ҳ���λ��.
//		ASSERT( FALSE );
		BOOL bSerchRet = IWebpageManager::Instance()->SearchPagePath( strParentUrl.c_str(), strParentCachePath,
			strParentSavePath );
		Log() << _T( "Parent page : " ) << strParentUrl 
			<< _T( " should cached to: " ) << strParentCachePath 
			<< _T( " should saved to: " ) << strParentSavePath << endl;
		ASSERT( bSerchRet );
		
		tstring strRelativePath;
		strRelativePath = CCommon::GetRelativePath( strParentSavePath.c_str(), strDstUrl );
		Log() << _T( "Relative url on parent page is : " ) << strRelativePath << endl;

		this->m_pHtmlPageParser->ReplaceAllUrl( strSrcUrl, strRelativePath.c_str() );

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CPageFecher::GetUrlInfo( LPCTSTR strUrl, BOOL& bNetworkOk, BOOL& bHaveDownload, BOOL& bShouldDownload, tstring& strCache, tstring& strSave, IHttpDownloader **ppDowloader )
{
//	ASSERT( FALSE );

	BOOL bResult = TRUE;

	bNetworkOk = TRUE;
	bHaveDownload = FALSE;
	bShouldDownload = FALSE;
	strCache.clear();
	strSave.clear();
//	_ASSERT( NULL == *ppDowloader );

	// �Ƿ��������أ�ֱ�ӷ��ء�
	if ( this->IsBanUrl( strUrl ) )
	{
		return TRUE;
	}

	// �Ƿ��Ѿ����ء�
	tstring strLocalPath;
	bHaveDownload = IWebpageManager::Instance()->GetPageLocalFilePath( strUrl, strLocalPath );

	// �Ѿ����ص�ҳ��϶���Ҫ���ء�
	bShouldDownload |= bHaveDownload;
	if ( bHaveDownload )
	{
		// ��ȡ������ҳ��Ӧ���ڵ�λ�á�
		IWebpageManager::Instance()->SearchPagePath( strUrl, strCache, strSave );
		Log() << _T( "Url had been download ! url: " ) << strUrl << endl;
		Log() << _T( "It should be cached to: " ) << strCache << endl;
		Log() << _T( "It should be saved to: " ) << strSave << endl;

	}
	else
	{
		// �Ƿ���Ҫ���ء�
		// ��Ҫ��URL�ж�URL�����͡�
		IHttpDownloader *pDownloader = NULL;

		if ( *ppDowloader )
		{
			pDownloader = *ppDowloader;
		}
		else
		{
			pDownloader = CClassFactory::CreateHttpDownloader();	
		}

		BOOL bOpenUrl = pDownloader->OpenUrl( strUrl );
		//if ( !bOpenUrl )
		//{
		//	// ��������Ϊinet�����ڲ������������´���һ�β������硣����
		//	delete pDownloader;
		//	pDownloader = NULL;
		//	pDownloader = CClassFactory::CreateHttpDownloader();
		//	*ppDowloader = pDownloader;

		//	bOpenUrl = pDownloader->OpenUrl( strUrl );
		//}

//		_ASSERT( bOpenUrl );
		if ( !bOpenUrl )
		{
			Log() << _T( "Open Url Fail!!!! url: " ) << strUrl << endl;
//			ASSERT( FALSE );

			// Ӧ����������
			

			bNetworkOk = pDownloader->TestNetwork();
			if ( bNetworkOk )
			{
				// ���������������ַ�����⡣
//				ASSERT( FALSE );
				Log() << _T( "Network is ok! but url can't get! " ) << strUrl << endl;
				tstring strParentUrl = IConfig::Instance()->GetRootUrl();
				if ( m_pHtmlPageParser )
				{
					strParentUrl = m_pHtmlPageParser->GetCurServerUrl();
				}
				IWebpageManager::Instance()->AddFailUrl( strParentUrl.c_str(), strUrl );

				// ��Ҫ�����ˡ�
				bShouldDownload = FALSE;

				bNetworkOk = TRUE;
			}
			else
			{
				// ���粻������
				ASSERT( FALSE );
				bNetworkOk = FALSE;
				bResult =  FALSE;
			}

			//delete pDownloader;
			//pDownloader = NULL;
			//*ppDowloader = pDownloader;
		}
		else
		{
			// ����ָ�롣
			*ppDowloader = pDownloader;

			CMimeType cMimeType;
			BOOL bGetType = this->m_pHtmlDownloader->GetMimeType( cMimeType );
			ASSERT( bGetType );
			if ( bGetType )
			{
				// �Ƿ���Ҫ���ء� 
				bShouldDownload = this->IsUrlShouldDownload( strUrl, cMimeType ); 
				if ( bShouldDownload )
				{
					// ���ء�
					Log() << _T( "Url Should be downloaded! url: " ) << strUrl << endl;
					// ��ȡ������ҳ��Ӧ���ڵ�λ�á�
					IWebpageManager::Instance()->PreAllocateFilePath( strUrl, cMimeType, strCache, strSave );

					Log() << _T( "It should be cached to: " ) << strCache << endl;
					Log() << _T( "It should be saved to: " ) << strSave << endl;

				}
				else
				{
					Log() << _T( "Url Should not download! " ) << strUrl << endl;
				}
			}
			else
			{
				bResult = FALSE;
			}
		}
	}
	return bResult;
}

BOOL CPageFecher::IsBanUrl( LPCTSTR strUrl )
{
	BOOL bShould = FALSE;
	tstringarray tar = IConfig::Instance()->GetAllBanUrl();
	for ( size_t i=0; i<tar.size(); ++i )
	{
		tstring strBan = tar[i];

		if ( CCommon::StrNCmpNocase( strBan.c_str(), strUrl, strBan.length() ) == 0 )
		{
			BOOL bShouldBan = IConfig::Instance()->IsUrlBan( strBan.c_str() );

			Log() << _T( "url ") << strUrl << _T( " is ban to open: " ) << bShouldBan << endl;
			bShould = bShouldBan;
		}
	}

	return bShould;
}


#include "..\include\PageFecher.h"
#include "Log.h"
#include "IWebpageManager.h"
#include "ClassFactory.h"
#include "Common.h"
#include "IConfig.h"

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

	// 是否有需要下载的url？
	if ( !m_tUrlWaitingDownloadStack.empty() )
	{
		tstring strUrl = this->m_tUrlWaitingDownloadStack.top();

		// 清除这个处理完毕的页面下载。
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
					// 下载。
					ASSERT( m_pHtmlDownloader );
					if( m_pHtmlDownloader )
					{
						BOOL bDownload = m_pHtmlDownloader->DownloadFile( strCachePath.c_str() );
						ASSERT( bDownload );
						if ( bDownload )
						{
							bHaveDownload = TRUE;

							IWebpageManager::Instance()->CachePageUrl( strUrl.c_str() );
						}
					}
				}
				
				// 更新。
				if ( bHaveDownload )
				{
					BOOL bReplaceRet = this->ReplaceUrl( strUrl.c_str(), strSavePath.c_str() );
					ASSERT( bReplaceRet );
				}
					
			}
		}
		else
		{
			Log() << _T( "GetUrlInfo fail!!!" ) << endl;

			this->m_bHasWork = FALSE;
			return FALSE;
		}
		//// 是否已经下载。
		//tstring strLocalPath;
		//BOOL bHaveDownload = IWebpageManager::Instance()->GetPageLocalFilePath( strUrl.c_str(), strLocalPath );
		//if( !bHaveDownload )
		//{
		//	// 是否需要下载。
		//	// 需要打开URL判断URL的类型。
		//	if( m_pHtmlDownloader == NULL )
		//	{
		//		m_pHtmlDownloader = CClassFactory::CreateHttpDownloader();
		//	}
		//	BOOL bOpenUrl = m_pHtmlDownloader->OpenUrl( strUrl.c_str() );
		//	_ASSERT( bOpenUrl );
		//	if ( !bOpenUrl )
		//	{
		//		Log() << _T( "Open Url Fail!!!! url: " ) << strUrl << endl;
		//		ASSERT( FALSE );

		//		// 应该做错误处理。
		//	}
		//	else
		//	{
		//		IHttpDownloader::EMimeType eHttpFileType;
		//		BOOL bGetType = this->m_pHtmlDownloader->GetFileType( eHttpFileType );
		//		ASSERT( bGetType );
		//		if ( bGetType )
		//		{
		//			// 是否需要下载。 
		//			BOOL bShouldDownload = this->IsUrlShouldDownload( strUrl.c_str() ); 
		//			if ( bShouldDownload )
		//			{
		//				// 下载。
		//				Log() << _T( "Url Should be downloaded! url: " ) << strUrl << endl;
		//				// 获取理论上页面应该在的位置。
		//				tstring strCachePath, strSavePath;
		//				IWebpageManager::Instance()->PreAllocateFilePath( strUrl.c_str(), strCachePath, strSavePath );

		//				Log() << _T( "It should be cached to: " ) << strCachePath << endl;
		//				Log() << _T( "It should be saved to: " ) << strSavePath << endl;

		//				// 下载到缓存目录。
		//				if( m_pHtmlDownloader == NULL )
		//				{
		//					m_pHtmlDownloader = CClassFactory::CreateHttpDownloader();
		//				}
		//				BOOL bDownloadRet = m_pHtmlDownloader->DownloadFile( strUrl.c_str(), strCachePath.c_str() );
		//				Log() << _T( "Download " ) << strUrl
		//					<< _T( " to " ) << strCachePath 
		//					<< _T( " ret " ) << bDownloadRet << endl;
		//				if ( !bDownloadRet )
		//				{
		//					tstring strParentUrl;
		//					if ( this->m_pHtmlPageParser )
		//					{
		//						strParentUrl = this->m_pHtmlPageParser->GetCurServerUrl();
		//					}
		//					else
		//					{
		//						strParentUrl = IConfig::Instance()->GetRootUrl();
		//					}

		//					// 这里应该要判断一下是不是能够上网。
		//					// 如果能上网，则只是这个网页获取失败。
		//					ASSERT( FALSE );
		//					// 否则，退出这个线程，因为它已经无法工作了。

		//					IWebpageManager::Instance()->AddFailUrl( strParentUrl.c_str(), strUrl.c_str() );
		//					Log() << _T( "Add Download fail url: " ) << strUrl
		//						<< _T( " it's parent: " ) << strParentUrl << endl;
		//				}
		//				else
		//				{
		//					BOOL bCacheRet = IWebpageManager::Instance()->CachePageUrl( strUrl.c_str() );
		//					Log() << _T( "Record cached page : " ) << strUrl << _T( " ret: " ) << bCacheRet << endl;
		//				}

		//			}
		//			else
		//			{
		//				delete m_pHtmlDownloader;
		//				m_pHtmlDownloader = NULL;
		//			}
		//		}
		//	}
		//}
		//else
		//{
		//	Log() << _T( "No need to download !The url have been downloaded to " ) << strLocalPath << endl;
		//}

		//	
		//// 更新网页。
		//BOOL bReplaceRet = this->ReplaceUrl( strUrl.c_str(), );
	

		//}

		

	}
	else if( m_pHtmlPageParser )
	{
		// 已经没有需要下载的url了。
		// 这个网页已经处理完毕，可以保存了。
		ASSERT( FALSE );

		// 保存到最终文件夹。

		// 删除缓存文件。
	}
	else
	{
		// 获取一个需要处理的页面。 
		tstring strUrl, strLocalPath;

		// 尝试在缓存中获取页面以处理。
		if ( IWebpageManager::Instance()->GetCachedPage( strUrl, strLocalPath ) )
		{
			// 处理这个文件。

			// 判断是否是网页。
			if ( CCommon::IsWebpage( strLocalPath.c_str() ) )
			{
				this->m_pHtmlPageParser = CClassFactory::CreateHtmlPageParser();
				BOOL bRet = this->m_pHtmlPageParser->Parse( strLocalPath.c_str(), strUrl.c_str() );
				ASSERT( bRet );

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
				// 保存这个文件到最终保存目录，并记录。
				ASSERT( FALSE );

			}
			

			
		}
		else
		{
			// 获取主页。
			tstring strRootUrl = IConfig::Instance()->GetRootUrl();
			Log() << _T( "No cached web page, deal with the root url: " ) << strRootUrl << endl;

			tstring strRootUrlLocalPath;
			if ( IWebpageManager::Instance()->GetPageLocalFilePath( strRootUrl.c_str(), strRootUrlLocalPath ) )
			{
				// 主页获取过了。结束。
				Log() << _T( "No cached web page and the root page had been fetched! Fetch work done!!" ) << endl;
				this->m_bHasWork = FALSE;
			}
			else
			{
				// 主页没有获取到，获取主页。
				// 将主页加入获取列表。下一轮中将会被处理。
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

	// 判断它是不是html网页。
	// 
//	Log() << _T( "url should not tested!" ) << endl;
//	ASSERT( FALSE );
	if ( CMimeType::HttpMimeHtmlHtm == mimetype.GetMimeType() )
	{
		// 获取过滤url列表。 
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
		// 查询父页面的位置.
		ASSERT( FALSE );
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
	ASSERT( FALSE );

	BOOL bResult = TRUE;

	bNetworkOk = FALSE;
	bHaveDownload = FALSE;
	bShouldDownload = FALSE;
	strCache.clear();
	strSave.clear();
	_ASSERT( NULL == *ppDowloader );

	// 是否已经下载。
	tstring strLocalPath;
	bHaveDownload = IWebpageManager::Instance()->GetPageLocalFilePath( strUrl, strLocalPath );

	// 已经下载的页面肯定需要下载。
	bShouldDownload |= bHaveDownload;
	if( !bHaveDownload )
	{
		// 是否需要下载。
		// 需要打开URL判断URL的类型。
		IHttpDownloader *pDownloader = CClassFactory::CreateHttpDownloader();		

		BOOL bOpenUrl = pDownloader->OpenUrl( strUrl );
		_ASSERT( bOpenUrl );
		if ( !bOpenUrl )
		{
			Log() << _T( "Open Url Fail!!!! url: " ) << strUrl << endl;
			ASSERT( FALSE );

			// 应该做错误处理。
			BOOL bNetWorkOk = pDownloader->TestNetwork();
			if ( bNetWorkOk )
			{
				// 网络正常。这个网址有问题。
				ASSERT( FALSE );
				Log() << _T( "Network is ok! but url can't get! " ) << strUrl << endl;
				tstring strParentUrl = IConfig::Instance()->GetRootUrl();
				if ( m_pHtmlPageParser )
				{
					strParentUrl = m_pHtmlPageParser->GetCurServerUrl();
				}
				IWebpageManager::Instance()->AddFailUrl( strParentUrl.c_str(), strUrl );

				// 不要下载了。
				bShouldDownload = FALSE;

				bNetWorkOk = TRUE;
			}
			else
			{
				// 网络不正常。
				ASSERT( FALSE );
				bNetWorkOk = FALSE;
				bResult =  FALSE;
			}

			delete pDownloader;
			pDownloader = NULL;
		}
		else
		{
			// 保存指针。
			*ppDowloader = pDownloader;

			CMimeType cMimeType;
			BOOL bGetType = this->m_pHtmlDownloader->GetMimeType( cMimeType );
			ASSERT( bGetType );
			if ( bGetType )
			{
				// 是否需要下载。 
				bShouldDownload = this->IsUrlShouldDownload( strUrl, cMimeType ); 
				if ( bShouldDownload )
				{
					// 下载。
					Log() << _T( "Url Should be downloaded! url: " ) << strUrl << endl;
					// 获取理论上页面应该在的位置。
					IWebpageManager::Instance()->PreAllocateFilePath( strUrl, cMimeType, strCache, strSave );

					Log() << _T( "It should be cached to: " ) << strCache << endl;
					Log() << _T( "It should be saved to: " ) << strSave << endl;

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


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
	CLog() << _T( "CPageFecher Fetch One Page!" ) << endl;
//	Sleep( 1000 );


	// 是否有需要下载的url？
	if ( !m_tUrlWaitingDownloadStack.empty() )
	{
		tstring strUrl = this->m_tUrlWaitingDownloadStack.top();

		CLog() << _T( "Find url wait for download! url: " ) << strUrl << endl;

		// 是否需要下载。 
		BOOL bShouldDownload = this->IsUrlShouldDownload( strUrl.c_str() );
		if ( bShouldDownload )
		{
			CLog() << _T( "Url Should be downloaded! url: " ) << strUrl << endl;
			// 获取理论上页面应该在的位置。
			tstring strCachePath, strSavePath;
			IWebpageManager::Instance()->GetUrlPagePrePath( strUrl.c_str(), strCachePath, strSavePath );

			CLog() << _T( "It should be cached to: " ) << strCachePath << endl;
			CLog() << _T( "It should be saved to: " ) << strSavePath << endl;

			// 是否已经下载。 
			tstring strLocalPath;
			BOOL bHaveDownload = IWebpageManager::Instance()->GetPageLocalFilePath( strUrl.c_str(), strLocalPath );
			if ( !bHaveDownload )
			{
				// 下载到缓存目录。
				if( m_pHtmlDownloader == NULL )
				{
					m_pHtmlDownloader = CClassFactory::CreateHttpDownloader();
				}
				BOOL bDownloadRet = m_pHtmlDownloader->DownloadFile( strUrl.c_str(), strCachePath.c_str() );
				CLog() << _T( "Download " ) << strUrl
					<< _T( " to " ) << strCachePath 
					<< _T( " ret " ) << bDownloadRet << endl;
				if ( !bDownloadRet )
				{
					tstring strParentUrl;
					if ( this->m_pHtmlPageParser )
					{
						strParentUrl = this->m_pHtmlPageParser->GetCurServerUrl();
					}
					else
					{
						strParentUrl = IConfig::Instance()->GetRootUrl();
					}
					IWebpageManager::Instance()->AddFailUrl( strParentUrl.c_str(), strUrl.c_str() );
					CLog() << _T( "Add Download fail url: " ) << strUrl
						<< _T( " it's parent: " ) << strParentUrl << endl;
				}
				else
				{
					BOOL bCacheRet = IWebpageManager::Instance()->CachePageUrl( strUrl.c_str() );
					CLog() << _T( "Record cached page : " ) << strUrl << _T( " ret: " ) << bCacheRet << endl;
				}
			}	
			else
			{
				CLog() << _T( "No need to download !The url have been downloaded to " ) << strLocalPath << endl;
			}
			
			// 更新网页。
			if ( this->m_pHtmlPageParser )
			{
				tstring strParentUrl = this->m_pHtmlPageParser->GetCurServerUrl();
				tstring strParentCachePath, strParentSavePath;
				IWebpageManager::Instance()->GetUrlPagePrePath( strParentUrl.c_str(), strParentCachePath,
					strParentSavePath );
				CLog() << _T( "Parent page : " ) << strParentUrl 
					<< _T( " should cached to: " ) << strParentCachePath 
					<< _T( " should saved to: " ) << strParentSavePath << endl;

				tstring strRelativePath;
				strRelativePath = CCommon::GetRelativePath( strParentSavePath.c_str(), strSavePath.c_str() );
				CLog() << _T( "Relative url on parent page is : " ) << strRelativePath << endl;

				this->m_pHtmlPageParser->ReplaceAllUrl( strUrl.c_str(), strRelativePath.c_str() );
			}

		}

		// 清除这个处理完毕的页面下载。
		m_tUrlWaitingDownloadStack.pop();
		CLog() << _T( "waiting download queue pop." ) << endl;

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
			CLog() << _T( "No cached web page, deal with the root url: " ) << strRootUrl << endl;

			tstring strRootUrlLocalPath;
			if ( IWebpageManager::Instance()->GetPageLocalFilePath( strRootUrl.c_str(), strRootUrlLocalPath ) )
			{
				// 主页获取过了。结束。
				CLog() << _T( "No cached web page and the root page had been fetched! Fetch work done!!" ) << endl;
				this->m_bHasWork = FALSE;
			}
			else
			{
				// 主页没有获取到，获取主页。
				// 将主页加入获取列表。下一轮中将会被处理。
				this->m_tUrlWaitingDownloadStack.push( strRootUrl );
				CLog() << _T( "The Root Url : " ) << strRootUrl << _T( " has been push to download queue!" ) << endl;
			}
		}

		
	}

	

	return TRUE;
}

BOOL CPageFecher::HasPageWaiting()
{
	return this->m_bHasWork;

}

BOOL CPageFecher::IsUrlShouldDownload( LPCTSTR strUrl )
{
	BOOL bShould = FALSE;

	// 判断它是不是html网页。
	CLog() << _T( "url should " ) << endl;
	ASSERT( FALSE );

	// 获取过滤url列表。 
	tstringarray tar = IConfig::Instance()->GetAllFetchFilter();
	for ( size_t i=0; i<tar.size(); ++i  )
	{
		tstring strFilter = tar[i];

		if ( CCommon::StrNCmpNocase( strFilter.c_str(), strUrl, strFilter.length() ) == 0 )
		{
			BOOL bShouldFetch = IConfig::Instance()->IsUrlFilterFetch( strFilter.c_str() );

			CLog() << _T( "url ") << strUrl << _T( " is filterd to fetch: " ) << bShouldFetch << endl;
			bShould = bShouldFetch;
		}
	}
	return TRUE;
}

BOOL CPageFecher::IsPageNeedParse( LPCTSTR strUrl )
{
	ASSERT( FALSE );
	return TRUE;
}



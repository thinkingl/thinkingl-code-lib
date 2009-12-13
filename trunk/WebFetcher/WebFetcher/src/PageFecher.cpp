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


	// �Ƿ�����Ҫ���ص�url��
	if ( !m_tUrlWaitingDownloadStack.empty() )
	{
		tstring strUrl = this->m_tUrlWaitingDownloadStack.top();

		CLog() << _T( "Find url wait for download! url: " ) << strUrl << endl;

		// �Ƿ���Ҫ���ء� 
		BOOL bShouldDownload = this->IsUrlShouldDownload( strUrl.c_str() );
		if ( bShouldDownload )
		{
			CLog() << _T( "Url Should be downloaded! url: " ) << strUrl << endl;
			// ��ȡ������ҳ��Ӧ���ڵ�λ�á�
			tstring strCachePath, strSavePath;
			IWebpageManager::Instance()->GetUrlPagePrePath( strUrl.c_str(), strCachePath, strSavePath );

			CLog() << _T( "It should be cached to: " ) << strCachePath << endl;
			CLog() << _T( "It should be saved to: " ) << strSavePath << endl;

			// �Ƿ��Ѿ����ء� 
			tstring strLocalPath;
			BOOL bHaveDownload = IWebpageManager::Instance()->GetPageLocalFilePath( strUrl.c_str(), strLocalPath );
			if ( !bHaveDownload )
			{
				// ���ص�����Ŀ¼��
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
			
			// ������ҳ��
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

		// ������������ϵ�ҳ�����ء�
		m_tUrlWaitingDownloadStack.pop();
		CLog() << _T( "waiting download queue pop." ) << endl;

	}
	else if( m_pHtmlPageParser )
	{
		// �Ѿ�û����Ҫ���ص�url�ˡ�
		// �����ҳ�Ѿ�������ϣ����Ա����ˡ�
		ASSERT( FALSE );

		// ���浽�����ļ��С�

		// ɾ�������ļ���
	}
	else
	{
		// ��ȡһ����Ҫ�����ҳ�档 
		tstring strUrl, strLocalPath;

		// �����ڻ����л�ȡҳ���Դ���
		if ( IWebpageManager::Instance()->GetCachedPage( strUrl, strLocalPath ) )
		{
			// ��������ļ���

			// �ж��Ƿ�����ҳ��
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
				// ��������ļ������ձ���Ŀ¼������¼��
				ASSERT( FALSE );

			}
			

			
		}
		else
		{
			// ��ȡ��ҳ��
			tstring strRootUrl = IConfig::Instance()->GetRootUrl();
			CLog() << _T( "No cached web page, deal with the root url: " ) << strRootUrl << endl;

			tstring strRootUrlLocalPath;
			if ( IWebpageManager::Instance()->GetPageLocalFilePath( strRootUrl.c_str(), strRootUrlLocalPath ) )
			{
				// ��ҳ��ȡ���ˡ�������
				CLog() << _T( "No cached web page and the root page had been fetched! Fetch work done!!" ) << endl;
				this->m_bHasWork = FALSE;
			}
			else
			{
				// ��ҳû�л�ȡ������ȡ��ҳ��
				// ����ҳ�����ȡ�б���һ���н��ᱻ����
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

	// �ж����ǲ���html��ҳ��
	CLog() << _T( "url should " ) << endl;
	ASSERT( FALSE );

	// ��ȡ����url�б� 
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



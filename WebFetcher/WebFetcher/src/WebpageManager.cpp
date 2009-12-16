#include "WebpageManager.h"
#include "Log.h"
#include "ClassFactory.h"

CWebpageManager::CWebpageManager(void)
{
	this->m_pDatabase = NULL;
}

CWebpageManager::~CWebpageManager(void)
{
	if ( m_pDatabase )
	{
		BOOL bClose = m_pDatabase->Close();
		CLog() << _T( "Close database ret: " ) << bClose << endl;
		delete m_pDatabase;
		m_pDatabase = NULL;
	}
}

BOOL CWebpageManager::Init()
{
	if ( NULL == m_pDatabase )
	{
		m_pDatabase = CClassFactory::CreateDatabase();
	}
	BOOL bOpen = m_pDatabase->Open();
	CLog() << _T( "Open Database ret: " ) << bOpen << endl;
	return bOpen;
}

BOOL CWebpageManager::GetCachedPage( tstring& strUrl, tstring& strLocalPath )
{
	ASSERT( m_pDatabase );
	if ( m_pDatabase )
	{
		// 首先看自己缓存的页面中有没有。如果有的话就暂时不用去数据库获取了。
		if ( !m_tCachedPageStack.empty() )
		{
			// 直接返回缓存的。
			IDatabase::TUrlRecordItem item = m_tCachedPageStack.top();
			strUrl = item.m_strUrl;
			strLocalPath = item.m_strCachePath;

			// 将这条记录为正在获取。
			this->m_tFetchingUrl.insert( strUrl );

			CLog() << _T( "GetCachedPage return a self cached page: " ) << strUrl << endl;
			return TRUE;
		}
		else
		{
			const int MAX_NUM = 1000;
			IDatabase::TUrlRecordItemList tItemList;
			BOOL bResult = m_pDatabase->GetUrlByStateByNum( IDatabase::PageStateCached,
				tItemList, MAX_NUM );
			ASSERT( bResult );
			if ( bResult )
			{
				if ( tItemList.size() > 0 )
				{
					for ( size_t i=0; i<tItemList.size(); ++i )
					{
						tstring strtmpUrl = tItemList[i].m_strUrl;
						if ( this->m_tFetchingUrl.find( strtmpUrl ) == this->m_tFetchingUrl.end() )
						{
							this->m_tCachedPageStack.push( tItemList[i] );

							CLog() << _T( "GetCachedPage cache one page: " ) << strtmpUrl << endl;
						}
						else
						{
							// 这个页面正在获取。
							CLog() << _T( "GetCachedPage page is fetching, skip it!" ) << strtmpUrl << endl;
						}
					}
					CLog() << _T( "GetCachedPage recersive call GetCachedPage!!!!!" ) << endl;
					return this->GetCachedPage( strUrl, strLocalPath );
				}
				else
				{
					CLog() <<_T( "There is no cached page in database record!!" ) << endl;
					return FALSE; // 查找数据库，结果数据库中没有。
				}
				
			}
			else
			{
				CLog() << _T( "GetCachedPage GetUrlByStateByNum fail!" ) << endl;
				ASSERT( FALSE );
				return FALSE;
			}
		}		

	}
	ASSERT( FALSE );
	return FALSE;
}

BOOL CWebpageManager::GetPageLocalFilePath( LPCTSTR strUrl, tstring& strLocalPath )
{
	ASSERT( m_pDatabase );
	if ( m_pDatabase )
	{
		IDatabase::TUrlRecordItem item;
		BOOL bRet = this->m_pDatabase->SearchUrl( strUrl, item );
		if ( bRet )
		{
			CLog() << _T( "GetPageLocalFilePath url: " ) << strUrl
				<< _T( " cache " ) << item.m_strCachePath
				<< _T( " save " ) << item.m_strSavePath
				<< _T( " state " ) << item.m_eState << endl;
			switch ( item.m_eState )
			{
			case IDatabase::PageStateCached:
				strLocalPath = item.m_strCachePath;
				break;
			case IDatabase::PageStateSaved:
				strLocalPath = item.m_strSavePath;
				break;

			default:
				ASSERT( FALSE );
				return FALSE;
			}
			CLog() << _T( "GetPageLocalFilePath url: " ) << strUrl 
				<< _T( " path: " ) << strLocalPath << endl;
			return TRUE;
		}
		else
		{
			CLog() << _T( "GetPageLocalFilePath Can't find url in database: " ) << strUrl << endl;
			return FALSE;
		}
	}
	

	ASSERT( FALSE );
	return FALSE;
}

BOOL CWebpageManager::CachePageUrl( LPCTSTR strUrl )
{
	ASSERT( FALSE );
	return FALSE;
}

BOOL CWebpageManager::CachedPageToSavedPage( LPCTSTR strUrl )
{
	ASSERT( FALSE );
	return FALSE;
}

BOOL CWebpageManager::AddFailUrl( LPCTSTR strBaseUrl, LPCTSTR strFailUrl )
{
	ASSERT( FALSE );
	return FALSE;
}

BOOL CWebpageManager::PreAllocateFilePath( LPCTSTR strUrl, IHttpDownloader::EMimeType eFileType, tstring& strCachePath, tstring& strSavePath )
{
	BOOL bResult = FALSE;
	ASSERT( m_pDatabase );
	if ( m_pDatabase )
	{
		IDatabase::TUrlRecordItem existItem;
		BOOL bHad = m_pDatabase->SearchUrl( strUrl, existItem );
		if ( bHad )
		{
			bResult = TRUE;
			strCachePath = existItem.m_strCachePath;
			strSavePath = existItem.m_strSavePath;			
		}
		else
		{
			// 创建一个。
			tstring strExt;
			switch ( eFileType )
			{
			case IHttpDownloader::HttpMimeHtmlHtm:
				strExt = _T( "html" );
				break;
			case IHttpDownloader::HttpMimeOther:
			default:
				strExt = _T( "" );
				ASSERT( FALSE );
				break;
			}

			// 保存进数据库。
			
		}
	}
	ASSERT( bResult );
	return bResult;
}
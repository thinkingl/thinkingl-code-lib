#include "WebpageManager.h"
#include "Log.h"
#include "ClassFactory.h"
#include "Common.h"
#include "IConfig.h"

LPCTSTR CACHE_FOLDER = _T( "cache" );
LPCTSTR FINAL_FOLDER = _T( "final" );

CWebpageManager::CWebpageManager(void)
{
	this->m_pDatabase = NULL;
}

CWebpageManager::~CWebpageManager(void)
{
	if ( m_pDatabase )
	{
		BOOL bClose = m_pDatabase->Close();
		Log() << _T( "Close database ret: " ) << bClose << endl;
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
	Log() << _T( "Open Database ret: " ) << bOpen << endl;
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

			Log() << _T( "GetCachedPage return a self cached page: " ) << strUrl << endl;
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

							Log() << _T( "GetCachedPage cache one page: " ) << strtmpUrl << endl;
						}
						else
						{
							// 这个页面正在获取。
							Log() << _T( "GetCachedPage page is fetching, skip it!" ) << strtmpUrl << endl;
						}
					}
					Log() << _T( "GetCachedPage recersive call GetCachedPage!!!!!" ) << endl;
					return this->GetCachedPage( strUrl, strLocalPath );
				}
				else
				{
					Log() <<_T( "There is no cached page in database record!!" ) << endl;
					return FALSE; // 查找数据库，结果数据库中没有。
				}
				
			}
			else
			{
				Log() << _T( "GetCachedPage GetUrlByStateByNum fail!" ) << endl;
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
			item.m_strCachePath = this->GetFileFullPath( item.m_strCachePath.c_str() );
			item.m_strSavePath = this->GetFileFullPath( item.m_strSavePath.c_str() );

			Log() << _T( "GetPageLocalFilePath url: " ) << strUrl
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
//				ASSERT( FALSE );
				return FALSE;
			}
			Log() << _T( "GetPageLocalFilePath url: " ) << strUrl 
				<< _T( " path: " ) << strLocalPath << endl;
			return TRUE;
		}
		else
		{
			Log() << _T( "GetPageLocalFilePath Can't find url in database: " ) << strUrl << endl;
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

BOOL CWebpageManager::PreAllocateFilePath( LPCTSTR strUrl, const CMimeType& cMimeType, tstring& strCachePath, tstring& strSavePath )
{
	BOOL bResult = TRUE;
	
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
			tstring strExt = cMimeType.GetRecommendFileExt();
			tstring strBaseName = CCommon::Url2FileName( strUrl );

			tstringstream ssFileName;
			ssFileName << _T( "\\" ) << strExt << _T( "\\" ) << strBaseName << _T( "." ) << strExt;	

			strCachePath.clear();
			strCachePath += CACHE_FOLDER;
			strCachePath += ssFileName.str();

			strSavePath.clear();
			strSavePath += FINAL_FOLDER;
			strSavePath += ssFileName.str();
			
			ASSERT( FALSE );
			// 保存进数据库。
			IDatabase::TUrlRecordItem newItem;
			newItem.m_strUrl = strUrl;
			newItem.m_strCachePath = strCachePath;
			newItem.m_strSavePath = strSavePath;
			newItem.m_eState = IDatabase::PageStateWaiting;
			BOOL bret = this->m_pDatabase->AddRecord( newItem );
			ASSERT( bret );				
		
		}
		
		// 数据库中存放的是相对路径，还需要添加根目录。
		strCachePath = this->GetFileFullPath( strCachePath.c_str() );
		strSavePath = this->GetFileFullPath( strSavePath.c_str() );

		CLog() << _T( "PreAllocate file path: cache: " ) << strCachePath << _T( " final: " ) << strSavePath << endl;
	}
	ASSERT( bResult );
	return bResult;
	
}

BOOL CWebpageManager::SearchPagePath( LPCTSTR strUrl, tstring& strCachePath, tstring& strSavePath )
{
	BOOL bRet = FALSE;
	ASSERT( m_pDatabase );
	if ( m_pDatabase )
	{
		IDatabase::TUrlRecordItem existItem;
		BOOL bHad = m_pDatabase->SearchUrl( strUrl, existItem );
		if ( bHad )
		{
			bRet = TRUE;
			strCachePath = this->GetFileFullPath( existItem.m_strCachePath.c_str() );
			strSavePath = this->GetFileFullPath( existItem.m_strSavePath.c_str() );			
		}
		else
		{
			bRet = FALSE;
		}
	}
	return bRet;
}

tstring CWebpageManager::GetFileFullPath( LPCTSTR strRelativePath )
{
	tstring strRootFolder = IConfig::Instance()->GetRootFolder();
	tstring strFullPath = strRootFolder + strRelativePath;

	return strFullPath;
}

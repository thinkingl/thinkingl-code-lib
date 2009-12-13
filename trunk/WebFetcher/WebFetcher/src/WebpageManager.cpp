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
	ASSERT( FALSE );
	return FALSE;
}

BOOL CWebpageManager::GetPageLocalFilePath( LPCTSTR strUrl, tstring& strLocalPath )
{
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

BOOL CWebpageManager::GetUrlPagePrePath( LPCTSTR strUrl, tstring& strCachePath, tstring& strSavePath )
{
	ASSERT( FALSE );
	return FALSE;
}
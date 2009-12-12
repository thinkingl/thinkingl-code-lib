#include "WebpageManager.h"

CWebpageManager::CWebpageManager(void)
{
}

CWebpageManager::~CWebpageManager(void)
{
}

BOOL CWebpageManager::GetCachedPage( tstring& strUrl, tstring& strLocalPath )
{
	return FALSE;
}

BOOL CWebpageManager::GetLocalPath( LPCTSTR strUrl, tstring& strLocalPath )
{
	return FALSE;
}

BOOL CWebpageManager::CachePageUrl( LPCTSTR strUrl, LPCTSTR strLocalPath )
{
	return FALSE;
}

BOOL CWebpageManager::CachedPageToSavedPage( LPCTSTR strUrl )
{
	return FALSE;
}

BOOL CWebpageManager::AddFailUrl( LPCTSTR strBaseUrl, LPCTSTR strFailUrl )
{
	return FALSE;
}
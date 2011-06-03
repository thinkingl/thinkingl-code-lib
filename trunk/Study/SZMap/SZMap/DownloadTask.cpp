#include "DownloadTask.h"

#include "WinInetHttpFecher.h"

CDownloadTask::CDownloadTask( ctstring& url, ctstring& localFilePath )
{
	m_url = url;
	m_localFilePath = localFilePath;
}


CDownloadTask::~CDownloadTask(void)
{
}

BOOL CDownloadTask::Do()
{
	// 完成这个任务.
	CWinInetHttpFecher httpDownloader;

	BOOL bRet = httpDownloader.OpenUrl( this->m_url.c_str() );
	bRet &= httpDownloader.DownloadFile( m_localFilePath.c_str() );

	return bRet;
}

ctstring CDownloadTask::GetLocalPath() const
{
	return m_localFilePath;
}

ctstring CDownloadTask::GetUrl() const
{
	return m_url;
}
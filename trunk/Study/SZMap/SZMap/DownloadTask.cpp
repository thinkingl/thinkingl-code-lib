#include "DownloadTask.h"


CDownloadTask::CDownloadTask( ctstring& url, ctstring& localFilePath )
{
	m_url = url;
	m_localFilePath = localFilePath;
}


CDownloadTask::~CDownloadTask(void)
{
}

bool CDownloadTask::Do()
{
	// 完成这个任务.


	return true;
}

ctstring CDownloadTask::GetLocalPath() const
{
	return m_localFilePath;
}

ctstring CDownloadTask::GetUrl() const
{
	return m_url;
}
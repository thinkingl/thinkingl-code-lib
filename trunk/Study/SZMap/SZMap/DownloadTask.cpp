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
	// ����������.


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
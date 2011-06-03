#pragma once
#include "itask.h"
#include "portabledefine.h"

/** 下载任务. */

class CDownloadTask :
	public ITask
{
public:
	CDownloadTask( ctstring& url, ctstring& localFilePath );
	virtual ~CDownloadTask(void);

public:	// 继承自 ITask.
	/** 完成这个任务. */
	virtual BOOL Do();

public: // 下载任务特有的.
	ctstring GetUrl()const;
	ctstring GetLocalPath() const;

private:
	tstring m_url;
	tstring m_localFilePath;
};


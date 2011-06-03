#pragma once
#include "itask.h"
#include "portabledefine.h"

/** ��������. */

class CDownloadTask :
	public ITask
{
public:
	CDownloadTask( ctstring& url, ctstring& localFilePath );
	virtual ~CDownloadTask(void);

public:	// �̳��� ITask.
	/** ����������. */
	virtual BOOL Do();

public: // �����������е�.
	ctstring GetUrl()const;
	ctstring GetLocalPath() const;

private:
	tstring m_url;
	tstring m_localFilePath;
};


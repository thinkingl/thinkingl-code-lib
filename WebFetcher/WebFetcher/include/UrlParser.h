#pragma once

#include "portabledefine.h"

class CUrlParser
{
public:
	CUrlParser(void);
	CUrlParser( LPCTSTR strUrl );
	virtual ~CUrlParser(void);


	/** 获取子域名。 */
	BOOL GetSubDomain( tstring& strSubDomain );

	/** 获取相对网站根目录路径。 */
	BOOL GetWebDir( tstring& strWebDir );

	/** 获取文件扩展名。 */
	BOOL GetFileExt( tstring strExt );

private:
	tstring m_strUrl;
	
};

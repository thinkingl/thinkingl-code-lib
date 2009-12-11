#pragma once

#include "portabledefine.h"

class CHtmlPageParser
{
public:
	CHtmlPageParser( LPCTSTR strHtmlFilePath );
	virtual ~CHtmlPageParser(void);

	typedef std::vector< tstring > TUrlList;

	/** 获取所有URL。 */
	BOOL GetAllUrl( TUrlList& tUrlList );

	/** 替换Url。 */
	BOOL ReplaceAllUrl( LPCTSTR strSrcUrl, LPCTSTR strDstUrl );

	/** 保存。 */
	BOOL SaveFile( LPCTSTR strPath );
};

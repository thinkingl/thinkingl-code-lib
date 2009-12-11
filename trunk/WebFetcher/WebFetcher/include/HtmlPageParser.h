#pragma once

#include "portabledefine.h"

class CHtmlPageParser
{
public:
	CHtmlPageParser( LPCTSTR strHtmlFilePath );
	virtual ~CHtmlPageParser(void);

	typedef std::vector< tstring > TUrlList;

	/** ��ȡ����URL�� */
	BOOL GetAllUrl( TUrlList& tUrlList );

	/** �滻Url�� */
	BOOL ReplaceAllUrl( LPCTSTR strSrcUrl, LPCTSTR strDstUrl );

	/** ���档 */
	BOOL SaveFile( LPCTSTR strPath );
};

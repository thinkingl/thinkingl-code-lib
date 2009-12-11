#pragma once

#include "portabledefine.h"
#include "IHtmlPageParser.h"

class CHtmlPageParser : public IHtmlPageParser
{
public:
	CHtmlPageParser(  );
	virtual ~CHtmlPageParser(void);


	/** 解析。 
	*	strHtmlFilePath 存在本地的文件。
	*	strHtmlServerUrl 这个文件之前的URL。用于拼接页面中的相对URL.
	*/
	virtual BOOL Parse( LPCTSTR strHtmlFilePath, LPCTSTR strHtmlServerUrl );

	/** 获取所有URL。 */
	virtual BOOL GetAllUrl( TUrlList& tUrlList );

	/** 替换Url。 */
	virtual BOOL ReplaceAllUrl( LPCTSTR strSrcUrl, LPCTSTR strDstUrl );

	/** 保存。 */
	virtual BOOL SaveFile( LPCTSTR strPath );

private:
	BOOL IsContainUrl( LPCTSTR strLine );

	BOOL GetUrl( LPCTSTR strLine, tstring& strFullUrl, tstring& strOriginalUrl, int &nPos );

private:
	/** 网页路径。 */
	tstring m_strHtmlPageLocalPath;;

	/** url. */
	tstring m_strServerUrlFolder;

	/** url */
	typedef std::set< tstring > TUrlSet;
	TUrlSet m_tUrlSet;

	/** 前面是完整的URL，后面是原URL。 */
	typedef std::pair< tstring, tstring > TFullUrlOrignalUrl; 

	/** 记录所有url出现的位置。 
	*	这里的位置指读取的字符串的长度累加，一直到目标url出现的值。
	*/	
	typedef std::map< int, TFullUrlOrignalUrl > TUrlPosTable;
	TUrlPosTable m_tUrlPosTable;


	/** 记录url替换映射关系。 */
	typedef std::map< tstring , tstring > TUrlReplaceTable;
	TUrlReplaceTable m_tReplaceTable;
};

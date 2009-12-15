#pragma once

class IHtmlPageParser
{
public:
    virtual ~IHtmlPageParser(){};

	typedef std::vector< tstring > TUrlList;

	/** 解析。 
	*	strHtmlFilePath 存在本地的文件。
	*	strHtmlServerUrl 这个文件之前的URL。用于拼接页面中的相对URL.
	*/
	virtual BOOL Parse( LPCTSTR strHtmlFilePath, LPCTSTR strHtmlServerUrl ) = 0;

	/** 获取所有URL。 */
	virtual BOOL GetAllUrl( TUrlList& tUrlList ) = 0;

	/** 替换Url。 */
	virtual BOOL ReplaceAllUrl( LPCTSTR strSrcUrl, LPCTSTR strDstUrl ) = 0;

	/** 保存。 */
	virtual BOOL SaveFile( LPCTSTR strPath ) = 0;

	/** 获取当前正在处理的服务器url。 */
	virtual tstring GetCurServerUrl() = 0;
};

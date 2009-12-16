#pragma once
#include "ipagefecher.h"
#include "IHtmlPageParser.h"
#include <stack>
#include "ihttpdownloader.h"

class CPageFecher :
	public IPageFecher
{
public:
	CPageFecher(void);
	virtual ~CPageFecher(void);

	/** 抓取一个网页。
	*/
	virtual BOOL FetchOnePage() ;

	/** 是否还有工作。 */
	virtual BOOL HasPageWaiting() ;

private:
	/** 分析一个URL，获取信息。 */
	BOOL GetUrlInfo( LPCTSTR strUrl, BOOL& bNetworkOk, BOOL& bHaveDownload, BOOL& bShouldDownload,
		tstring& strCache, tstring& strSave, IHttpDownloader **ppDowloader );

	/** 替换网页中的url。 */
	BOOL ReplaceUrl( LPCTSTR strSrcUrl, LPCTSTR strDstUrl );

	/** 根据过滤条件，决定网页是否需要下载。 
	*	只有需要解析的网页才需要下载。
	*	其它文件都需要下载。
	*	这里不检验是否已经下载过了！
	*/
	BOOL IsUrlShouldDownload( LPCTSTR strUrl, IHttpDownloader::EMimeType eType );

	/** 根据过滤条件，决定网页是否需要解析。
	*	只有过滤条件中被设置为需要抓取的网页才需要解析。
	*/
	BOOL IsPageNeedParse( LPCTSTR strUrl );
private:
	IHtmlPageParser *m_pHtmlPageParser;

	typedef std::stack< tstring > TStringStack;
	TStringStack m_tUrlWaitingDownloadStack;
//	IHtmlPageParser::TUrlList m_tUrlWaitingDownload;

	IHttpDownloader *m_pHtmlDownloader;

	/** 是否有工作做。 */
	BOOL m_bHasWork;
};

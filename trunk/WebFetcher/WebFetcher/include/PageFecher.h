#pragma once
#include "ipagefecher.h"
#include "IHtmlPageParser.h"

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
	IHtmlPageParser *m_pHtmlPageParser;

	IHtmlPageParser::TUrlList m_tUrlWaitingDownload;

};

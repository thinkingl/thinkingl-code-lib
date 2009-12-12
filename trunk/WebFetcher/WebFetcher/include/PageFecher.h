#pragma once
#include "ipagefecher.h"
#include "IHtmlPageParser.h"

class CPageFecher :
	public IPageFecher
{
public:
	CPageFecher(void);
	virtual ~CPageFecher(void);

	/** ץȡһ����ҳ��
	*/
	virtual BOOL FetchOnePage() ;

	/** �Ƿ��й����� */
	virtual BOOL HasPageWaiting() ;

private:
	IHtmlPageParser *m_pHtmlPageParser;

	IHtmlPageParser::TUrlList m_tUrlWaitingDownload;

};

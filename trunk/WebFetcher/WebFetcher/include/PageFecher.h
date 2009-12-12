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

	/** ץȡһ����ҳ��
	*/
	virtual BOOL FetchOnePage() ;

	/** �Ƿ��й����� */
	virtual BOOL HasPageWaiting() ;

private:
	/** ���ݹ���������������ҳ�Ƿ���Ҫ���ء� 
	*	ֻ����Ҫ��������ҳ����Ҫ���ء�
	*	�����ļ�����Ҫ���ء�
	*	���ﲻ�����Ƿ��Ѿ����ع��ˣ�
	*/
	BOOL IsUrlShouldDownload( LPCTSTR strUrl );

	/** ���ݹ���������������ҳ�Ƿ���Ҫ������
	*	ֻ�й��������б�����Ϊ��Ҫץȡ����ҳ����Ҫ������
	*/
	BOOL IsPageNeedParse( LPCTSTR strUrl );
private:
	IHtmlPageParser *m_pHtmlPageParser;

	typedef std::stack< tstring > TStringStack;
	TStringStack m_tUrlWaitingDownloadStack;
//	IHtmlPageParser::TUrlList m_tUrlWaitingDownload;

	IHttpDownloader *m_pHtmlDownloader;

	/** �Ƿ��й������� */
	BOOL m_bHasWork;
};

#pragma once

#include "portabledefine.h"
#include "IHtmlPageParser.h"

class CHtmlPageParser : public IHtmlPageParser
{
public:
	CHtmlPageParser(  );
	virtual ~CHtmlPageParser(void);


	/** ������ 
	*	strHtmlFilePath ���ڱ��ص��ļ���
	*	strHtmlServerUrl ����ļ�֮ǰ��URL������ƴ��ҳ���е����URL.
	*/
	virtual BOOL Parse( LPCTSTR strHtmlFilePath, LPCTSTR strHtmlServerUrl );

	/** ��ȡ����URL�� */
	virtual BOOL GetAllUrl( TUrlList& tUrlList );

	/** �滻Url�� */
	virtual BOOL ReplaceAllUrl( LPCTSTR strSrcUrl, LPCTSTR strDstUrl );

	/** ���档 */
	virtual BOOL SaveFile( LPCTSTR strPath );

	/** ��ȡ��ǰ���ڴ���ķ�����url�� */
	virtual tstring GetCurServerUrl() ;

private:

	BOOL IsUrl( LPCTSTR strUrl );

	/** ��ǰλ���Ƿ���Scrip��ǩ� */
	BOOL IsInScript( int nPos );

	/** ���������� script ��ǩ��λ�á� */
	typedef std::pair< int,int > TTokenBeginEnd;
	typedef std::vector< TTokenBeginEnd > THtmlTokenPosList;
	
	BOOL ParseAllScriptToken( LPCTSTR strHtmlContent, THtmlTokenPosList& tAllTokenList );

private:
//	BOOL IsContainUrl( LPCTSTR strLine );

//	BOOL GetUrl( LPCTSTR strLine, tstring& strFullUrl, tstring& strOriginalUrl, int &nPos );
	BOOL GetFullUrl( LPCTSTR strOriginalUrl, tstring& strFullUrl );
private:
	/** ��ҳ·���� */
	tstring m_strHtmlPageLocalPath;;

	/** ��ҳ���ݡ� */
	tstring m_strHtmlPageContent;

	/** url. */
	tstring m_strServerUrlFolder;
	tstring m_strServerUrl;

	/** url */
	typedef std::set< tstring > TUrlSet;
	TUrlSet m_tUrlSet;

	/** ǰ����������URL��������ԭURL�� */
	typedef std::pair< tstring, tstring > TFullUrlOrignalUrl; 

	/** ��¼����url���ֵ�λ�á� 
	*	�����λ��ָ��ȡ���ַ����ĳ����ۼӣ�һֱ��Ŀ��url���ֵ�ֵ��
	*/	
	typedef std::map< int, TFullUrlOrignalUrl > TUrlPosTable;
	TUrlPosTable m_tUrlPosTable;


	/** ��¼url�滻ӳ���ϵ�� */
	typedef std::map< tstring , tstring > TUrlReplaceTable;
	TUrlReplaceTable m_tReplaceTable;

	/** ��¼���нű���λ�ã����ˡ� */
};

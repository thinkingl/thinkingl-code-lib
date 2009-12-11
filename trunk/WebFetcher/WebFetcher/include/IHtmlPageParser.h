#pragma once

class IHtmlPageParser
{
public:
	typedef std::vector< tstring > TUrlList;

	/** ������ 
	*	strHtmlFilePath ���ڱ��ص��ļ���
	*	strHtmlServerUrl ����ļ�֮ǰ��URL������ƴ��ҳ���е����URL.
	*/
	virtual BOOL Parse( LPCTSTR strHtmlFilePath, LPCTSTR strHtmlServerUrl ) = 0;

	/** ��ȡ����URL�� */
	virtual BOOL GetAllUrl( TUrlList& tUrlList ) = 0;

	/** �滻Url�� */
	virtual BOOL ReplaceAllUrl( LPCTSTR strSrcUrl, LPCTSTR strDstUrl ) = 0;

	/** ���档 */
	virtual BOOL SaveFile( LPCTSTR strPath ) = 0;
};

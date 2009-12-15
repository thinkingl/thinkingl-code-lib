#pragma once
#include "iwebpagemanager.h"
#include "IDatabase.h"
#include <stack>

class CWebpageManager :
	public IWebpageManager
{

protected:
	friend class IWebpageManager;

	CWebpageManager(void);
	virtual ~CWebpageManager(void);

	virtual BOOL Init();

public:
	/** ��ȡһ���ȴ������Ļ�����ҳ�� */
	virtual BOOL GetCachedPage( tstring& strUrl, tstring& strLocalPath ) ;

	/** Ԥ�Ȼ�ȡһ��URL��Ӧ�Ļ����ļ�·���ͱ����ļ�·����
	*	������ڣ������Ѿ����ڵġ�
	*/
	virtual BOOL PreAllocateFilePath( LPCTSTR strUrl, 
		IHttpDownloader::EHttpFileType eFileType,
		tstring& strCachePath, 
		tstring& strSavePath );

	/** ��ȡURL�ڱ��صı���·���� */
	virtual BOOL GetPageLocalFilePath( LPCTSTR strUrl, tstring& strLocalPath ) ;

	/** ����һ�������url��¼�� */
	virtual BOOL CachePageUrl( LPCTSTR strUrl ) ;

	/** ����url��Ϊ����õ���ҳ�� */
	virtual BOOL CachedPageToSavedPage( LPCTSTR strUrl ) ;

	/** ��¼һ����ȡʧ�ܵ���ҳ�� 
	*	�ڻ�ȡ strBaseUrl ʱ���޷���ȡ�� strFailUrl��
	*/
	virtual BOOL AddFailUrl( LPCTSTR strBaseUrl, LPCTSTR strFailUrl ) ;

private:
	/** ���ݿ⡣ */
	IDatabase *m_pDatabase;

	/** ��¼���ڴ�����ҳ�档 */
	typedef std::set< tstring > TStringSet;
	TStringSet m_tFetchingUrl;

	/** �����¼�������ҳ�档 */
	typedef std::stack< IDatabase::TUrlRecordItem > TDatabaseRecordStack;
	TDatabaseRecordStack m_tCachedPageStack;

	/** */
};
#pragma once

#include "portabledefine.h"
#include "IHttpDownloader.h"

/** url table*/

class IWebpageManager
{
public:
	static IWebpageManager *Instance();
	static void Release();
private:
	static IWebpageManager *s_instance;
protected:
    virtual ~IWebpageManager(){};
public:

	virtual BOOL Init() = 0;

	/** ��ȡһ���ȴ�����Ļ�����ҳ�� */
	virtual BOOL GetCachedPage( tstring& strUrl, tstring& strLocalPath ) = 0;

	/** Ԥ�Ȼ�ȡһ��URL��Ӧ�Ļ����ļ�·���ͱ����ļ�·����
	*	������ڣ������Ѿ����ڵġ�
	*/
	virtual BOOL PreAllocateFilePath( LPCTSTR strUrl, 
		IHttpDownloader::EMimeType eFileType,
		tstring& strCachePath, 
		tstring& strSavePath ) = 0;

	/** ��ȡURL�ڱ��صı���·���� */
	virtual BOOL GetPageLocalFilePath( LPCTSTR strUrl, tstring& strLocalPath ) = 0;

	/** ����һ�������url��¼�� */
	virtual BOOL CachePageUrl( LPCTSTR strUrl ) = 0;

	/** ����url��Ϊ����õ���ҳ�� */
	virtual BOOL CachedPageToSavedPage( LPCTSTR strUrl ) = 0;

	/** ��¼һ����ȡʧ�ܵ���ҳ�� 
	*	�ڻ�ȡ strBaseUrl ʱ���޷���ȡ�� strFailUrl��
	*/
	virtual BOOL AddFailUrl( LPCTSTR strBaseUrl, LPCTSTR strFailUrl ) = 0;
};

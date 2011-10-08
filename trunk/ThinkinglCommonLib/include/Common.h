#pragma once

#include "portabledefine.h"

class CCommon
{
public:
	CCommon(void);
	virtual ~CCommon(void);

	static void Utf8toUtf16( LPCSTR strUtf8, wstring& strUtf16 );

	static void Utf16toUtf8( LPCWSTR strUtf16, string& strUtf8 );


	static int StrNCmpNocase( LPCTSTR str1, LPCTSTR str2, int nLen );

	static tstring GetModulePath( void * hm = NULL );

	/** �ļ�����Ϣ. */
	struct TFileNameInfo 
	{
		tstring m_strFilePath;	// ȫ·��.
		tstring m_strFileName;	// �ļ���.
		tstring m_strDirectory;	// �ļ������ļ���.
		tstring m_strBaseName;	// �����ļ���.
		tstring m_strExtName;	// �ļ���չ��.

		bool operator == ( const TFileNameInfo& tAnother ) const;
	};
	/** �����ļ���. */
	static TFileNameInfo ParsePath( LPCTSTR strPath );
	static BOOL ParsePath( LPCTSTR strPath, TFileNameInfo& tFileNameInfo );

	/** ��ȡ�����ļ�����. */
	static tstring GetAppName();
	/** ��ȡ�����ļ���·���� */
	static tstring GetAppDir();

	/** �ж�һ���ļ��Ƿ�����ҳ�ļ���
	*	ֻ�����ļ�����׺���жϡ�
	*/
	static BOOL IsWebpage( LPCTSTR strPath );

	/** ��ȡ���·����
	*	���ļ�strSrc������strDstPath�����·����
	*/
	static tstring GetRelativePath( LPCTSTR strSrcPath, LPCTSTR strDstPath );

    /** ��һ���ļ��б�ɱ�׼����ʽ�� */
    static void NormalizeDir( tstring& strFolder );

	/** ��һ��urlת�ɱ�׼��ʽ���� / �滻 \ .*/
	static void NormalizeUrl( tstring& strUrl );

    /** ʱ��->�ַ��� */
    static tstring TimeToStr( __time64_t nTime );
    static __time64_t StrToTime( LPCTSTR strTime );

    /** ʱ��ת������ȡ����ʱ�䡣 */
    static BOOL GetLocalTime( const time_t time, tm * plocalTime );

    /** ��ȡ��ǰʱ�䡣 UTC ���ʱ�׼ʱ�䣬��Ҫʱ��ת���� */
    static __time64_t GetCurTime();

	/** Urlת��Ϊ�ļ���. */
	static tstring Url2FileName( LPCTSTR strUrl );

	/** �ļ���ת�����ļ���. */
	static tstring FileName2Url( LPCTSTR strFileName );

	/** �ݹ�Ĵ����ļ��С� */
	static BOOL CreateDirRecurse( LPCTSTR strDir );

	/** ˯�� ��λ����.. */
	static void Sleep( int sleepMSec );

	/** ��ȡ�ļ�����. */
	static uint64 GetFileLength( ctstring filePath );

};

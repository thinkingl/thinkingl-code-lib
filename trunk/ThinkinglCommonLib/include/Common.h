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

	/** 文件名信息. */
	struct TFileNameInfo 
	{
		tstring m_strFilePath;	// 全路径.
		tstring m_strFileName;	// 文件名.
		tstring m_strDirectory;	// 文件所在文件夹.
		tstring m_strBaseName;	// 基本文件名.
		tstring m_strExtName;	// 文件扩展名.

		bool operator == ( const TFileNameInfo& tAnother ) const;
	};
	/** 解析文件名. */
	static TFileNameInfo ParsePath( LPCTSTR strPath );
	static BOOL ParsePath( LPCTSTR strPath, TFileNameInfo& tFileNameInfo );

	/** 获取程序文件名。. */
	static tstring GetAppName();
	/** 获取程序文件夹路径。 */
	static tstring GetAppDir();

	/** 判断一个文件是否是网页文件。
	*	只根据文件名后缀来判断。
	*/
	static BOOL IsWebpage( LPCTSTR strPath );

	/** 获取相对路径。
	*	在文件strSrc看来，strDstPath的相对路径。
	*/
	static tstring GetRelativePath( LPCTSTR strSrcPath, LPCTSTR strDstPath );

    /** 将一个文件夹变成标准的形式。 */
    static void NormalizeDir( tstring& strFolder );

	/** 将一个url转成标准形式。用 / 替换 \ .*/
	static void NormalizeUrl( tstring& strUrl );

    /** 时间->字符串 */
    static tstring TimeToStr( __time64_t nTime );
    static __time64_t StrToTime( LPCTSTR strTime );

    /** 时区转换，获取本地时间。 */
    static BOOL GetLocalTime( const time_t time, tm * plocalTime );

    /** 获取当前时间。 UTC 国际标准时间，需要时区转换。 */
    static __time64_t GetCurTime();

	/** Url转换为文件名. */
	static tstring Url2FileName( LPCTSTR strUrl );

	/** 文件名转换回文件名. */
	static tstring FileName2Url( LPCTSTR strFileName );

	/** 递归的创建文件夹。 */
	static BOOL CreateDirRecurse( LPCTSTR strDir );

	/** 睡眠 单位毫秒.. */
	static void Sleep( int sleepMSec );

	/** 获取文件长度. */
	static uint64 GetFileLength( ctstring filePath );

};

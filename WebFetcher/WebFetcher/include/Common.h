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
};

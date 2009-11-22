#pragma once

#include "portabledefine.h"

#ifndef __CYGWIN__	// cygwin 不支持unicode。
MU_DECLSPEC string UTF16toUTF8( wstring strUTF16 );
MU_DECLSPEC wstring UTF8toUTF16( string strUTF8 );
#endif

/** 获取模块路径. */
#ifdef WIN32_MODE
MU_DECLSPEC tstring GetModulePath( HMODULE hm = NULL );
#endif

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
MU_DECLSPEC TFileNameInfo ParsePath( LPCTSTR strPath );
MU_DECLSPEC BOOL ParsePath( LPCTSTR strPath, TFileNameInfo& tFileNameInfo );

/** 获取程序文件名。. */
MU_DECLSPEC tstring GetAppName();
/** 获取程序文件夹路径。 */
MU_DECLSPEC tstring GetAppDir();

/** 获取屏幕大小（）*/
MU_DECLSPEC int GetScreenLong();	// 高，宽中更长的那个
MU_DECLSPEC int GetScreenShort();	// 短的那个。

MU_DECLSPEC int GetScreenHeight();	// 高,y轴方向.
MU_DECLSPEC int GetScreenWidth();	// 宽,x轴方向.

/** 多字节转为Unicode。 */
MU_DECLSPEC BOOL MBtoWC(const char* srcStr, wchar_t * dstStr, int len );

/** Unicode转为多字节。 */
MU_DECLSPEC BOOL WCtoMB(const wchar_t* srcStr, char *dstStr, int len );

/** 时间->字符串 */
MU_DECLSPEC tstring TimeToStr( __time64_t nTime );
//MU_DECLSPEC __time64_t StrToTime( LPCTSTR strTime );

/** 判断手机是否有存储卡。 */
MU_DECLSPEC BOOL HasStorageCard();

/** check folder /file exist. */
MU_DECLSPEC BOOL IsFileExist( LPCTSTR strPath );

/** 创建文件夹,暂不考虑其它属性。 */
MU_DECLSPEC BOOL MakeDir( LPCTSTR strDir );

/** 删除文件。 */
MU_DECLSPEC BOOL DelFile( LPCTSTR strFile );

/** 忽略大小写的字符串比较。 */
MU_DECLSPEC int CompareNoCase(const string&   s,   const   string&   s2);

#ifndef __CYGWIN__	// cygwin 不支持unicode。
MU_DECLSPEC int CompareNoCase(const wstring&   s,   const   wstring&   s2);
#endif

/** 获取一个高精度相对时间。 */
MU_DECLSPEC mu_uint64 GetTick();

/** 获取当前时间。 UTC 国际标准时间，需要时区转换。 */
MU_DECLSPEC __time64_t GetCurTime();

/** 时区转换，获取本地时间。 */
MU_DECLSPEC BOOL GetLocalTime( const time_t time, tm * plocalTime );

/** 搜索一个文件夹，获取所有的文件。 
*	返回的文件是绝对路径。
*/
typedef std::vector< tstring > TFileList;
MU_DECLSPEC TFileList EnumAllFile( LPCTSTR strFolder );

/** 将一个文件夹变成标准的形式。 */
MU_DECLSPEC void NormalizeDir( tstring& strFolder );




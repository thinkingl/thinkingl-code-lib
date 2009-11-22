#pragma once

#include "portabledefine.h"

#ifndef __CYGWIN__	// cygwin ��֧��unicode��
MU_DECLSPEC string UTF16toUTF8( wstring strUTF16 );
MU_DECLSPEC wstring UTF8toUTF16( string strUTF8 );
#endif

/** ��ȡģ��·��. */
#ifdef WIN32_MODE
MU_DECLSPEC tstring GetModulePath( HMODULE hm = NULL );
#endif

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
MU_DECLSPEC TFileNameInfo ParsePath( LPCTSTR strPath );
MU_DECLSPEC BOOL ParsePath( LPCTSTR strPath, TFileNameInfo& tFileNameInfo );

/** ��ȡ�����ļ�����. */
MU_DECLSPEC tstring GetAppName();
/** ��ȡ�����ļ���·���� */
MU_DECLSPEC tstring GetAppDir();

/** ��ȡ��Ļ��С����*/
MU_DECLSPEC int GetScreenLong();	// �ߣ����и������Ǹ�
MU_DECLSPEC int GetScreenShort();	// �̵��Ǹ���

MU_DECLSPEC int GetScreenHeight();	// ��,y�᷽��.
MU_DECLSPEC int GetScreenWidth();	// ��,x�᷽��.

/** ���ֽ�תΪUnicode�� */
MU_DECLSPEC BOOL MBtoWC(const char* srcStr, wchar_t * dstStr, int len );

/** UnicodeתΪ���ֽڡ� */
MU_DECLSPEC BOOL WCtoMB(const wchar_t* srcStr, char *dstStr, int len );

/** ʱ��->�ַ��� */
MU_DECLSPEC tstring TimeToStr( __time64_t nTime );
//MU_DECLSPEC __time64_t StrToTime( LPCTSTR strTime );

/** �ж��ֻ��Ƿ��д洢���� */
MU_DECLSPEC BOOL HasStorageCard();

/** check folder /file exist. */
MU_DECLSPEC BOOL IsFileExist( LPCTSTR strPath );

/** �����ļ���,�ݲ������������ԡ� */
MU_DECLSPEC BOOL MakeDir( LPCTSTR strDir );

/** ɾ���ļ��� */
MU_DECLSPEC BOOL DelFile( LPCTSTR strFile );

/** ���Դ�Сд���ַ����Ƚϡ� */
MU_DECLSPEC int CompareNoCase(const string&   s,   const   string&   s2);

#ifndef __CYGWIN__	// cygwin ��֧��unicode��
MU_DECLSPEC int CompareNoCase(const wstring&   s,   const   wstring&   s2);
#endif

/** ��ȡһ���߾������ʱ�䡣 */
MU_DECLSPEC mu_uint64 GetTick();

/** ��ȡ��ǰʱ�䡣 UTC ���ʱ�׼ʱ�䣬��Ҫʱ��ת���� */
MU_DECLSPEC __time64_t GetCurTime();

/** ʱ��ת������ȡ����ʱ�䡣 */
MU_DECLSPEC BOOL GetLocalTime( const time_t time, tm * plocalTime );

/** ����һ���ļ��У���ȡ���е��ļ��� 
*	���ص��ļ��Ǿ���·����
*/
typedef std::vector< tstring > TFileList;
MU_DECLSPEC TFileList EnumAllFile( LPCTSTR strFolder );

/** ��һ���ļ��б�ɱ�׼����ʽ�� */
MU_DECLSPEC void NormalizeDir( tstring& strFolder );




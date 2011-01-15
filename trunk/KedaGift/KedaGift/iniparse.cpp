// iniparse.cpp: implementation of the CIniParse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "iniparse.h"
//#include "log.h"
#include <string.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniParse::CIniParse()
{

}

CIniParse::CIniParse(LPCTSTR strIniFilePath )
{
	SetIniFilePath( strIniFilePath );
}

CIniParse::CIniParse(LPCTSTR strIniFileName, LPCTSTR ModuleName )
{
	SetOwnerModule( ModuleName );
	SetIniFileName( strIniFileName );
}

CIniParse::CIniParse(LPCTSTR strIniFileName, HMODULE hModuleHandle )
{
	SetOwnerModule( hModuleHandle );
	SetIniFileName( strIniFileName );
}

CIniParse::~CIniParse()
{
//	OutputDebugString( _T( "~CIniParse" ) );
}

BOOL CIniParse::ParseBuffer(TCHAR aBuffer[], int nBuffLen, CStringArray& stringArray )
{
	stringArray.RemoveAll();

	// 分析tcKey中的Key列表.里面是用'\0'隔开的字符串.最终以"\0\0"结束.
	TCHAR *pchCursor = aBuffer;
	while ( _tcslen( pchCursor ) > 0 && pchCursor < aBuffer + nBuffLen)
	{
		CString strKey = pchCursor;
		pchCursor += strKey.GetLength() + 1;

		if ( !strKey.IsEmpty() )
		{
			stringArray.Add( strKey );
		}
	}

	return TRUE;
}

CString CIniParse::GetModuleDirectory(HMODULE hModuleHandle )
{
	TCHAR szDllPath[MAX_PATH] = {0};
    int nlen = ::GetModuleFileName( hModuleHandle, szDllPath, MAX_PATH-1);   
	CString strPath;
	
	if ( nlen > 0 )
	{
		TCHAR drive[4] = { 0 };//驱动器号 MAX_DRIVER
		TCHAR dir[MAX_PATH] = { 0 };//目录
		TCHAR title[MAX_PATH] = { 0 };//文件标题
		TCHAR ext[MAX_PATH] = { 0 }; //扩展名
		
		_tsplitpath(szDllPath,drive,dir,title,ext);
		
		strPath += drive;
		strPath += dir;
	}
    return strPath;
}

CString CIniParse::GetIniFilePath(LPCTSTR sFileName)
{

//	::GetModuleFileName(AfxGetInstanceHandle(), szPath, sizeof(szPath));
	CString strPath = GetModuleDirectory( m_hOwnerHandle );
	if ( strPath.IsEmpty() )
	{	// 如果无法获取到本模块的路径,就直接使用系统路径.
//		::GetSystemDirectory( strPath.GetBuffer( MAX_PATH ), MAX_PATH );
		::GetCurrentDirectory( MAX_PATH*2, strPath.GetBuffer( MAX_PATH*2 ) );
		strPath.ReleaseBuffer();
	}
	if ( strPath.Right( 1 ) != "\\" )
	{
		strPath += "\\";
	}
	strPath += sFileName;

//	CLog::WriteLog( "获取配置文件ini路径: %s", strPath );
	return strPath; 
}

// CString& CIniParse::Normalization(CString& strText )
// {
// 	strText.MakeLower();
// 	strText.TrimRight();
// 	strText.TrimLeft();
// 	return strText;
// }

void CIniParse::SetIniFileName(LPCTSTR strIniFileName )
{
	m_strIniFileName = strIniFileName;
	m_strIniFilePath = GetIniFilePath( strIniFileName );
}

void CIniParse::SetIniFilePath(LPCTSTR strIniFilePath )
{
	CString strPath = strIniFilePath;
	// 分析文件名.
	int index = strPath.ReverseFind( _T('\\') );
	if( index == -1 )
		return ;	// 错误! 没有"\\",不是合法的路径.
	else
	{
		int length = strPath.GetLength();
		m_strIniFileName = strPath.Right( length-index-1 );	// 解析出了正确的配置文件名.
	}

	m_strIniFilePath = strIniFilePath;
}

CString CIniParse::GetIniFilePath()
{
	return m_strIniFilePath;
}

int CIniParse::ReadInt(LPCTSTR strApp, LPCTSTR strKey, int nDefaultValue )
{
	return ::GetPrivateProfileInt(
		strApp,
		strKey,
		nDefaultValue,
		m_strIniFilePath );
}

BOOL CIniParse::WriteInt(LPCTSTR strApp, LPCTSTR strKey, int nValue )
{
	CString strValue;
	strValue.Format( _T( "%d" ), nValue );
	return WriteString( strApp, strKey, strValue );
}

CString CIniParse::ReadString(LPCTSTR strApp, LPCTSTR strKey, LPCTSTR strDefaultValue )
{
	CString strValue;
	int nLen = ::GetPrivateProfileString( strApp,
		strKey,
		strDefaultValue,
		strValue.GetBuffer( Max_Buffer_Len ),
		Max_Buffer_Len,
		m_strIniFilePath );
	strValue.ReleaseBuffer( );

	return strValue;
}

BOOL CIniParse::WriteString(LPCTSTR strApp, LPCTSTR strKey, LPCTSTR strValue )
{
	return ::WritePrivateProfileString( strApp,
		strKey,
		strValue,
		m_strIniFilePath );
}

BOOL CIniParse::ReadAppList(CStringArray& caAppList )
{
	TCHAR pTCBuffer[ Max_Buffer_Len ];
	int nWriteLen = ::GetPrivateProfileString( NULL,
		NULL,
		NULL,
		pTCBuffer,
		Max_Buffer_Len,
		m_strIniFilePath );

	BOOL bResult = FALSE;
	if ( nWriteLen > 0 )
	{
		bResult = ParseBuffer( pTCBuffer, nWriteLen + 1, caAppList ) ;
	}
	return bResult;
}

BOOL CIniParse::ReadKeyList(LPCTSTR strApp, CStringArray& caKeyList )
{
	caKeyList.RemoveAll();

	TCHAR pTCBuffer[ Max_Buffer_Len ];
	int nWriteLen = ::GetPrivateProfileString( strApp,
		NULL,
		NULL,
		pTCBuffer,
		Max_Buffer_Len,
		m_strIniFilePath );
	
	BOOL bResult = FALSE;
	if ( nWriteLen > 0 )
	{
		bResult = ParseBuffer( pTCBuffer, nWriteLen + 1, caKeyList ) ;
	}
	return bResult;
}

BOOL CIniParse::DelApp(LPCTSTR strApp )
{
	BOOL bResult = ( ::WritePrivateProfileSection( strApp,
			NULL,
			m_strIniFilePath ) );

	return bResult;
}

BOOL CIniParse::DelKey( LPCTSTR strApp, LPCTSTR strKey )
{
	BOOL bRet = ::WritePrivateProfileString( strApp, strKey, NULL, m_strIniFilePath );
	
	return bRet;
}

void CIniParse::SetOwnerModule(LPCTSTR strModuleName )
{
	m_hOwnerHandle = ::GetModuleHandle( strModuleName );
	
	SetOwnerModule( m_hOwnerHandle );
}

void CIniParse::SetOwnerModule(HMODULE hModuleHandle )
{
	m_hOwnerHandle = hModuleHandle;

	m_strIniFilePath = GetIniFilePath( m_strIniFileName );
}
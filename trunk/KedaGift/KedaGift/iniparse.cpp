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

	// ����tcKey�е�Key�б�.��������'\0'�������ַ���.������"\0\0"����.
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
		TCHAR drive[4] = { 0 };//�������� MAX_DRIVER
		TCHAR dir[MAX_PATH] = { 0 };//Ŀ¼
		TCHAR title[MAX_PATH] = { 0 };//�ļ�����
		TCHAR ext[MAX_PATH] = { 0 }; //��չ��
		
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
	{	// ����޷���ȡ����ģ���·��,��ֱ��ʹ��ϵͳ·��.
//		::GetSystemDirectory( strPath.GetBuffer( MAX_PATH ), MAX_PATH );
		::GetCurrentDirectory( MAX_PATH*2, strPath.GetBuffer( MAX_PATH*2 ) );
		strPath.ReleaseBuffer();
	}
	if ( strPath.Right( 1 ) != "\\" )
	{
		strPath += "\\";
	}
	strPath += sFileName;

//	CLog::WriteLog( "��ȡ�����ļ�ini·��: %s", strPath );
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
	// �����ļ���.
	int index = strPath.ReverseFind( _T('\\') );
	if( index == -1 )
		return ;	// ����! û��"\\",���ǺϷ���·��.
	else
	{
		int length = strPath.GetLength();
		m_strIniFileName = strPath.Right( length-index-1 );	// ����������ȷ�������ļ���.
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
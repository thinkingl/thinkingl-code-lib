#include "StdAfx.h"
#include "EmployerInput.h"

#include <iostream>
#include <fstream>
#include <set>

using namespace std;

CEmployerInput::CEmployerInput(void)
{
}


CEmployerInput::~CEmployerInput(void)
{
}

BOOL ReadString( CFile* pFile, CString& strInput )
{	
	strInput.Empty();

	while ( true )
	{
		TCHAR chInput;
		if( pFile->Read( &chInput, 1 ) == 0 )
		{
			break;
		}
		bool bBreakToken = ( ' ' == chInput 
			|| '\n' == chInput 
			|| '\r'== chInput
			|| '\t' == chInput);

		if ( bBreakToken  )
		{
			if ( strInput.IsEmpty() )
			{
				continue;
			}
			else
			{
				break;
			}
		}
		else
		{
			strInput.AppendChar( chInput );
		}
	}

	return !strInput.IsEmpty();
	
}

TEmployerList CEmployerInput::GetAllNoGiftEmployer() const
{
	TEmployerList allEmplyer;

	LPCTSTR ALL_EMPLOYER_LIST_FILE = _T( "EmployerList.txt" );
	LPCTSTR ALL_EMPLOYER_LIST_FILE_V2 = _T( "employerlistv2.txt" );

	CFile inputFile;
	BOOL bOpen = inputFile.Open( ALL_EMPLOYER_LIST_FILE_V2, CFile::modeRead );
	if ( !bOpen )
	{
		AfxMessageBox( _T( "打开员工列表文件失败!" ) );
		return allEmplyer;
	}

	try
	{
		std::set< CString > tAllKedaNo; 

		BOOL bOk = TRUE;
		while ( bOk )
		{
			CEmployer employerIn;
			
			bOk &= ReadString( &inputFile, employerIn.m_department );	// 读取部门。
			bOk &= ReadString( &inputFile, employerIn.m_strName );		// 读取姓名。
			bOk &=ReadString( &inputFile, employerIn.m_strKedaNo );		// 读取工号。
			if ( bOk )
			{
				// 头要去掉.
				if ( employerIn.m_strKedaNo.Find( _T( "KD" ) ) == -1 )
				{
					AfxMessageBox( _T( "有个工号不太对劲,没有KD!" ) );
					AfxMessageBox( employerIn.m_strKedaNo );
					continue;
				}

				allEmplyer.push_back( employerIn );

				_ASSERT( tAllKedaNo.find( employerIn.m_strKedaNo ) == tAllKedaNo.end() );
				_ASSERT( employerIn.m_strKedaNo.GetLength() == 8 );

				if ( employerIn.m_strKedaNo.GetLength() != 8 )
				{
					AfxMessageBox( _T("工号错误,有一个工号不是8位的！") ) ;
					AfxMessageBox( employerIn.m_strKedaNo );
					continue;
				}

				if ( tAllKedaNo.find( employerIn.m_strKedaNo ) != tAllKedaNo.end() )
				{
					AfxMessageBox( _T("工号重复出现！") );
					AfxMessageBox( employerIn.m_strKedaNo );
				}

				tAllKedaNo.insert( employerIn.m_strKedaNo );
			}			
		}
	}
	catch (CException* e)
	{
		e;
		int dfwef = 34;
	}
	

	return allEmplyer;
}
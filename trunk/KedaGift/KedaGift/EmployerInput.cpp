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
		AfxMessageBox( _T( "��Ա���б��ļ�ʧ��!" ) );
		return allEmplyer;
	}

	try
	{
		std::set< CString > tAllKedaNo; 

		BOOL bOk = TRUE;
		while ( bOk )
		{
			CEmployer employerIn;
			
			bOk &= ReadString( &inputFile, employerIn.m_department );	// ��ȡ���š�
			bOk &= ReadString( &inputFile, employerIn.m_strName );		// ��ȡ������
			bOk &=ReadString( &inputFile, employerIn.m_strKedaNo );		// ��ȡ���š�
			if ( bOk )
			{
				allEmplyer.push_back( employerIn );

				_ASSERT( tAllKedaNo.find( employerIn.m_strKedaNo ) == tAllKedaNo.end() );
				_ASSERT( employerIn.m_strKedaNo.GetLength() == 8 );

				if ( employerIn.m_strKedaNo.GetLength() != 8 )
				{
					AfxMessageBox( _T("���Ŵ���,��һ�����Ų���8λ�ģ�") ) ;
					AfxMessageBox( employerIn.m_strKedaNo );
					continue;
				}

				if ( tAllKedaNo.find( employerIn.m_strKedaNo ) != tAllKedaNo.end() )
				{
					AfxMessageBox( _T("�����ظ����֣�") );
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
#include "StdAfx.h"
#include "EmployerGiftConfig.h"

LPCTSTR EMPLOYER_GIFT_STATE = _T( "EmployerGiftState.ini" );

//LPCTSTR KEY_KEDA_NO = _T( "KedaNo" );
LPCTSTR KEY_NAME = _T( "name" );
LPCTSTR KEY_TIME = _T( "time" ); 

CEmployerGiftConfig::CEmployerGiftConfig(void)
{
	m_iniFileParser.SetIniFileName( EMPLOYER_GIFT_STATE );
}


CEmployerGiftConfig::~CEmployerGiftConfig(void)
{
}

TEmployerList CEmployerGiftConfig::GetLuckies()
{
	TEmployerList allLuckyMan;

	CStringArray arAllApp;
	this->m_iniFileParser.ReadAppList( arAllApp );
	for ( int i=0; i<arAllApp.GetSize(); ++i )
	{
		CString strKedaNo = arAllApp[i];
		CEmployer lucyOne;
		lucyOne.m_strKedaNo = strKedaNo;
		lucyOne.m_strName = this->m_iniFileParser.ReadString( strKedaNo, KEY_NAME );
		lucyOne.m_time = CTime( this->m_iniFileParser.ReadInt( strKedaNo, KEY_TIME, 0 ) );

		allLuckyMan.push_back( lucyOne );
	}
	return allLuckyMan;
}

void CEmployerGiftConfig::AddLuckMan( const CEmployer& lucyMan )
{
	this->m_iniFileParser.WriteString( lucyMan.m_strKedaNo, KEY_NAME, lucyMan.m_strName );
	this->m_iniFileParser.WriteInt( lucyMan.m_strKedaNo, KEY_TIME, lucyMan.m_time.GetTime() );
}

void CEmployerGiftConfig::Reset()
{
	TEmployerList allEmp = this->GetLuckies();
	for ( int i=0; i<allEmp.size(); ++i )
	{	
		this->m_iniFileParser.DelApp( allEmp[i].m_strKedaNo );
	}	
}
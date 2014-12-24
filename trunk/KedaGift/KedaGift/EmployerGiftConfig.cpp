#include "StdAfx.h"
#include "EmployerGiftConfig.h"

LPCTSTR EMPLOYER_GIFT_STATE = _T( "EmployerGiftState.ini" );

//LPCTSTR KEY_KEDA_NO = _T( "KedaNo" );
LPCTSTR KEY_NAME = _T( "name" );
LPCTSTR KEY_TIME = _T( "time" );
LPCTSTR KEY_GETGIFT_TIME = _T( "getgifttime" );
LPCTSTR KEY_ABSENT = _T( "isabsent" );
LPCTSTR KEY_RAUND = _T( "raund" );


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
		BOOL bOk = this->GetEmployerLucky( strKedaNo, lucyOne );
		_ASSERT( bOk );
		allLuckyMan.push_back( lucyOne );
	}
	return allLuckyMan;
}

void CEmployerGiftConfig::AddLuckMan( const CEmployer& lucyMan )
{
	this->m_iniFileParser.WriteString( lucyMan.m_strKedaNo, KEY_NAME, lucyMan.m_strName );
	this->m_iniFileParser.WriteInt( lucyMan.m_strKedaNo, KEY_TIME, lucyMan.m_timeLuck.GetTime() );
	this->m_iniFileParser.WriteInt( lucyMan.m_strKedaNo, KEY_GETGIFT_TIME, lucyMan.m_timeGetGift.GetTime() );
	this->m_iniFileParser.WriteInt( lucyMan.m_strKedaNo, KEY_ABSENT, lucyMan.m_bAbsent );
	this->m_iniFileParser.WriteInt( lucyMan.m_strKedaNo, KEY_RAUND, lucyMan.m_luckyRaund );
}

void CEmployerGiftConfig::Reset()
{
	TEmployerList allEmp = this->GetLuckies();
	for ( int i=0; i<allEmp.size(); ++i )
	{	
		this->m_iniFileParser.DelApp( allEmp[i].m_strKedaNo );
	}	
}

BOOL CEmployerGiftConfig::GetEmployerLucky( LPCTSTR strKedaNo, CEmployer& employer )
{
	employer.m_strKedaNo = strKedaNo;
	employer.m_strName = this->m_iniFileParser.ReadString( strKedaNo, KEY_NAME );
	employer.m_timeLuck = CTime( this->m_iniFileParser.ReadInt( strKedaNo, KEY_TIME, 0 ) );
	employer.m_timeGetGift = CTime( this->m_iniFileParser.ReadInt( strKedaNo, KEY_GETGIFT_TIME, 0 ) );
	employer.m_bAbsent = this->m_iniFileParser.ReadInt( strKedaNo, KEY_ABSENT, FALSE );
	employer.m_luckyRaund = this->m_iniFileParser.ReadInt( strKedaNo, KEY_RAUND, 0 );
	return !employer.m_strName.IsEmpty();
}
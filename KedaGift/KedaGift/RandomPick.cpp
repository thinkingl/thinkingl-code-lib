#include "StdAfx.h"
#include "RandomPick.h"


CRandomPick::CRandomPick(void)
{
	m_nFirstNoGift = 0;

	CTime timeNow = GetCurrentTime();
	CString strLogFileName = timeNow.Format( _T( "KedagiftLog_%y%m%d%H%M%S.txt" ) );
	m_fileLog.Open( strLogFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone );
	m_pArchiveLog = new CArchive( &m_fileLog, CArchive::store );
}


CRandomPick::~CRandomPick(void)
{
	if ( m_pArchiveLog )
	{
		m_pArchiveLog->Close();
		delete m_pArchiveLog;
		m_pArchiveLog = NULL;
	}
	m_fileLog.Close();	
}

int CRandomPick::GetRadomNum( int nBegin, int nEnd ) const
{
	// 是否初始化过随机种子.
	static bool s_bSetRadom = false;
	if( !s_bSetRadom )
	{
		DWORD dwSeed = GetTickCount();
		srand( dwSeed );
		s_bSetRadom = true;
	}

	int nRand = rand();
	int nCount = nEnd - nBegin +1;
	int nRet = nBegin + ( nRand % nCount );
	return nRet;
}

void CRandomPick::ResetGift()
{
	this->m_nFirstNoGift = 0;

	this->m_giftConfig.Reset();
}

void CRandomPick::SetEmployerList( const TEmployerList& employerList )
{
	this->m_tAllEmployer = employerList;
	
	this->InitAllReadyGifted();
}

CEmployer CRandomPick::GetLukyOne()
{
	int luckyIndex = this->GetRadomNum( m_nFirstNoGift, m_tAllEmployer.size() -1 );

	this->m_tAllEmployer[ luckyIndex ].m_time = CTime::GetCurrentTime();
	CEmployer luckyOne = this->m_tAllEmployer[ luckyIndex ];

	this->HandleGiftedLucyMan( luckyIndex );
	
	return luckyOne;
}

void CRandomPick::HandleGiftedLucyMan( int luckyIndex )
{
	CEmployer luckyOne = this->m_tAllEmployer[ luckyIndex ];
	// 把已经中奖的同志放到前面.
	CEmployer firstNoGift = this->m_tAllEmployer[ m_nFirstNoGift ];
	this->m_tAllEmployer[ luckyIndex ] = firstNoGift;
	this->m_tAllEmployer[ m_nFirstNoGift ] = luckyOne;

	m_nFirstNoGift ++;

	// 保存..
	this->m_giftConfig.AddLuckMan( luckyOne );
}

CEmployer CRandomPick::RandomPickOneNoGiftToShow() const
{
	int luckyIndex = this->GetRadomNum( m_nFirstNoGift, m_tAllEmployer.size() -1 );

	CEmployer luckyOne = this->m_tAllEmployer[ luckyIndex ];

	return luckyOne;
}

CArchive& CRandomPick::Log()
{
	_ASSERT( m_pArchiveLog );
	return *m_pArchiveLog;
}

void CRandomPick::InitAllReadyGifted()
{
	this->m_nFirstNoGift = 0;
	TEmployerList allGifted = m_giftConfig.GetLuckies();
	for ( int i=0; i<allGifted.size(); ++i )
	{
		for ( int k=0; k<m_tAllEmployer.size(); ++k )
		{
			if ( allGifted[i].m_strKedaNo == m_tAllEmployer[k].m_strKedaNo )
			{
				m_tAllEmployer[k].m_time = allGifted[i].m_time;
				this->HandleGiftedLucyMan( k );
				break;
			}
		}
	}
}
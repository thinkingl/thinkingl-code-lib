#include "StdAfx.h"
#include "UnitTest.h"
#include "RandomPick.h"
#include "EmployerInput.h"
#include <vector>
#include <map>

CUnitTest::CUnitTest(void)
{
	this->UTRadomPick();
}


CUnitTest::~CUnitTest(void)
{
}

void CUnitTest::UTRadomPick()
{
	CRandomPick rp;



	CEmployerInput employInput;
	TEmployerList allEmp = employInput.GetAllNoGiftEmployer();

	int nEmployerNum = allEmp.size();

	typedef std::map< CString, int > TCounterList;
	TCounterList tCounter;

	rp.SetEmployerList( allEmp );

	// 奖品数目.
	const int GIFT_NUM = 50;

	// 抽奖轮数.
	const int TEST_TIMES = 10000;

	for ( int i=0; i<TEST_TIMES; ++i )
	{
		for ( int k=0; k<GIFT_NUM; ++k )
		{
			CEmployer luckone = rp.GetLukyOne( i );
			tCounter[ luckone.m_strKedaNo ] ++;
		}
		rp.ResetGift();	// 重新一轮抽奖...
	}

	// 每个人能得到的奖品的平均数.
	int nAvrage = TEST_TIMES * GIFT_NUM / nEmployerNum;

	int nMax  = 0;
	CEmployer unluckOne;
	for ( size_t i=0; i<allEmp.size(); ++i )
	{
		CString strKedaNo = allEmp[i].m_strKedaNo;
		int nOffset = abs( nAvrage - tCounter[ strKedaNo ] );
		if ( nOffset > nMax )
		{
			unluckOne = allEmp[i];
			nMax = nOffset;
		}
	}

	CString strMsg;
	strMsg.Format( _T( "随机最大偏差比例为 %02f%% 平均值应为 %d 最大偏差项偏差值为 %d 最倒霉的家伙是 : %s" ),
		float( nMax * 100 )/nAvrage, nAvrage, nMax, unluckOne.m_strName  );
	AfxMessageBox( strMsg );
//	_ASSERT( FALSE );
}

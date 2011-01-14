#include "StdAfx.h"
#include "UnitTest.h"
#include "RadomPick.h"
#include <vector>


CUnitTest::CUnitTest(void)
{
	this->UTRadomPick();
}


CUnitTest::~CUnitTest(void)
{
}

void CUnitTest::UTRadomPick()
{
	CRadomPick rp;

	const int TEST_NUM = 356;

	typedef std::vector< int > TCounterList;
	TCounterList tCounter( TEST_NUM, 0 );

	const int TEST_TIMES = 10000000;
	for ( int i=0; i<TEST_TIMES; ++i )
	{
		int nRadom = rp.GetRadomNum( 0, tCounter.size()-1 );
		tCounter[ nRadom ] ++;
	}

	int nAvrage = TEST_TIMES / TEST_NUM;

	int nMax  = 0;
	for ( size_t i=0; i<tCounter.size(); ++i )
	{
		int nOffset = abs( nAvrage - tCounter[i] );
		if ( nOffset > nMax )
		{
			nMax = nOffset;
		}
	}

	CString strMsg;
	strMsg.Format( _T( "随机最大偏差比例为 %02f 平均值应为 %d 最大偏差项偏差值为 %d " ),float( nMax )/nAvrage, nAvrage, nMax  );
	AfxMessageBox( strMsg );
//	_ASSERT( FALSE );
}

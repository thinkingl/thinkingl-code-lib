/*
ID: thinkin6
PROG: spin
LANG: C++
*/

/** USA CO 3.2.3 :
Spinning Wheels
1998 ACM NE Regionals
Each of five opaque spinning wheels has one or more wedges cut out of its edges. These wedges must be aligned quickly and correctly. Each wheel also has an alignment mark (at 0 degrees) so that the wheels can all be started in a known position. Wheels rotate in the `plus degrees' direction, so that shortly after they start, they pass through 1 degree, 2 degrees, etc. (though probably not at the same time).

This is an integer problem. Wheels are never actually at 1.5 degrees or 23.51234123 degrees. For example, the wheels are considered to move instantaneously from 20 to 25 degrees during a single second or even from 30 to 40 degrees if the wheel is spinning quickly.

All angles in this problem are presumed to be integers in the range 0 <= angle <= 359. The angle of 0 degrees follows the angle of 359 degrees. Each wheel rotates at a certain integer number of degrees per second, 1 <= speed <= 180.

Wedges for each wheel are specified by an integer start angle and integer angle size (or `extent'), both specified in degrees. Wedges in the test data will be separated by at least one degree. The 'extent' also includes the original "degree" of the wedge, so '0 180' means degrees 0..180 inclusive -- one more than most would imagine.

At the start, which is time 0, all the wheels' alignment marks line up. Your program must determine the earliest time (integer seconds) at or after the start that some wedge on each wheel will align with the wedges on the other wheel so that a light beam can pass through openings on all five wedges. The wedges can align at any part of the rotation.

PROGRAM NAME: spin

INPUT FORMAT

Each of five input lines describes a wheel.

The first integer on an input line is the wheel's rotation speed. The next integer is the number of wedges, 1 <= W <= 5. The next W pairs of integers tell each wedge's start angle and extent.

SAMPLE INPUT (file spin.in)

30 1 0 120
50 1 150 90
60 1 60 90
70 1 180 180
90 1 180 60
OUTPUT FORMAT

A single line with a single integer that is the first time the wedges align so a light beam can pass through them. Print `none' (lower case, no quotes) if the wedges will never align properly.

SAMPLE OUTPUT (file spin.out)

9
*/

/** 
思路：
*	模拟.
*	所有的纺车在转360秒后都会回到起点,所以如果360秒内没有符合题意,就再也没有了.
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <math.h>
#include <set>
#include <sstream>
#include <algorithm>
#include <memory.h>
#include <complex>
#include <queue>

#ifdef _WIN32
#include <time.h>
#endif

using namespace std;

#ifdef _WIN32
typedef __int64 n64;
typedef unsigned __int64 u64;
#else
typedef long long n64;
typedef unsigned long long u64;
#endif
typedef unsigned long u32;

#ifdef _WIN32
#define THINKINGL 1
#endif

const char* NERVER_PASS = "none";
const int MAX_WEDGE_NUM = 5;
const int SPANNING_WHEEL_NUM = 5;

class CAngleRange
{
public:
	CAngleRange( int nStart, int nExtent );

	/** 是否在区域里. */
	bool IsIn( int nAngle )const;
	/** 旋转. */
	void Turn( int nAngle );
private:
	int m_nStart;
	int m_nExtent;
};

CAngleRange::CAngleRange( int nStart, int nExtent )
{
	m_nStart = nStart;
	m_nExtent = nExtent;
}

void CAngleRange::Turn( int nAngle )
{
	m_nStart += nAngle;
	if ( m_nStart > 359 )
	{
		m_nStart -= 360;
	}
}

bool CAngleRange::IsIn( int nAngle )const
{
	if ( nAngle >= m_nStart && nAngle <= m_nStart+m_nExtent )
	{
		return true;
	}
	if ( nAngle <= m_nStart + m_nExtent - 360 )
	{
		return true;
	}
	return false;
}

class CSpanningWheel
{
public:
	CSpanningWheel();
	void SetSpeed( int nSpeed );
	void AddWedge( int nStart, int nExtent );
	void TurnOneSecond();

	bool IsLightCanPass( int nAngle ) const;
private:
	int m_nSpeed;

	typedef std::vector< CAngleRange > TWedgesList;
	TWedgesList m_tWedges;

};

CSpanningWheel::CSpanningWheel()
{
	m_nSpeed = 0;
}

void CSpanningWheel::SetSpeed( int nSpeed )
{
	m_nSpeed = nSpeed;
}

void CSpanningWheel::AddWedge( int nStart, int nExtent )
{
	m_tWedges.push_back( CAngleRange( nStart, nExtent ) );
}

void CSpanningWheel::TurnOneSecond()
{
	for ( int i=0; i<m_tWedges.size(); ++i )
	{
		m_tWedges[i].Turn( this->m_nSpeed );
	}
}

bool CSpanningWheel::IsLightCanPass( int nAngle )const
{
	for ( int i=0; i<m_tWedges.size(); ++i )
	{
		if ( m_tWedges[i].IsIn( nAngle ) )
		{
			return true;
		}
	}
	return false;
}



int main()
{
	string strProblemName = "spin";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	CSpanningWheel arSpanningWheels[ SPANNING_WHEEL_NUM ];
	for ( int i=0; i<SPANNING_WHEEL_NUM; ++i )
	{
		int nSpeed, nWedgeNum;
		fin >> nSpeed >> nWedgeNum;
		arSpanningWheels[i].SetSpeed( nSpeed );
		for ( int k=0; k<nWedgeNum; ++k )
		{
			int nStart, nExtent;
			fin >> nStart >> nExtent;
			arSpanningWheels[i].AddWedge( nStart, nExtent );
		}
	}

	// 最大只考察360秒,因为不管速度是多少,360秒后都将回到起点.
	int nPassTime = -1;
	for ( int nTime=0; nTime<360; ++nTime )
	{
		// 判断是否能穿过光线.
		bool bPass = false;
		for ( int nAngle=0; nAngle<360; ++nAngle )
		{
			if ( nAngle == 270 )
			{
				int wdfw = 2;
			}
			bool bCanPass = true;
			for ( int nWheel=0; nWheel<SPANNING_WHEEL_NUM; ++nWheel )
			{
				if ( !arSpanningWheels[nWheel].IsLightCanPass( nAngle ) )
				{
					bCanPass = false;
					break;
				}
			}
			if ( bCanPass )
			{
				// 穿过了!
				bPass = true;
				break;
			}
		}

		if ( bPass )
		{
			nPassTime = nTime;
			break;
		}
		
		// 旋转.
		for ( int i=0; i< SPANNING_WHEEL_NUM; ++i )
		{
			arSpanningWheels[i].TurnOneSecond();
		}
	}

	if ( nPassTime != -1 )
	{
		fout << nPassTime << endl;
	}
	else
	{
		fout << NERVER_PASS << endl;
	}

	fin.close();
	fout.close();

#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}
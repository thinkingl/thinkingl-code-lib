/*
ID: thinkin6
PROG: ratios
LANG: C++
*/

/** USA CO 3.2.4 :
Feed Ratios
1998 ACM Finals, Dan Adkins
Farmer John feeds his cows only the finest mixture of cow food, which has three components: Barley, Oats, and Wheat. While he knows the precise mixture of these easily mixable grains, he can not buy that mixture! He buys three other mixtures of the three grains and then combines them to form the perfect mixture.

Given a set of integer ratios barley:oats:wheat, find a way to combine them IN INTEGER MULTIPLES to form a mix with some goal ratio x:y:z.

For example, given the goal 3:4:5 and the ratios of three mixtures:

1:2:3
3:7:1
2:1:2
your program should find some minimum number of integer units (the `mixture') of the first, second, and third mixture that should be mixed together to achieve the goal ratio or print `NONE'. `Minimum number' means the sum of the three non-negative mixture integers is minimized.
For this example, you can combine eight units of mixture 1, one unit of mixture 2, and five units of mixture 3 to get seven units of the goal ratio:

8*(1:2:3) + 1*(3:7:1) + 5*(2:1:2) = (21:28:35) = 7*(3:4:5)
Integers in the goal ratio and mixture ratios are all non-negative and smaller than 100 in magnitude. The number of units of each type of feed in the mixture must be less than 100. The mixture ratios are not linear combinations of each other.

PROGRAM NAME: ratios

INPUT FORMAT

Line 1:	 Three space separated integers that represent the goal ratios
Line 2..4:	Each contain three space separated integers that represent the ratios of the three mixtures purchased.
SAMPLE INPUT (file ratios.in)

3 4 5
1 2 3
3 7 1
2 1 2
OUTPUT FORMAT

The output file should contain one line containing four integers or the word `NONE'. The first three integers should represent the number of units of each mixture to use to obtain the goal ratio. The fourth number should be the multiple of the goal ratio obtained by mixing the initial feed using the first three integers as mixing ratios.

SAMPLE OUTPUT (file ratios.out)

8 1 5 7
*/

/** 
˼·��
*	.
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

/** ���ϳɷ���Ŀ. */
const int COMPONENT_NUM = 3;
/** ��������. */
const int FEED_NUM = 3;

/** ���ĳɷֱ���. */
const int MAX_FEED_RATIO = 100;

const char* NO_RATIO = "NONE";

int main()
{
	string strProblemName = "ratios";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	// ��ȡĿ��ɷ�.
	int arGoalComponent[ COMPONENT_NUM ] = {0};
	for ( int i=0; i<COMPONENT_NUM; ++i )
	{
		fin >> arGoalComponent[i];
	}

	// ��ȡ���гɷ�.
	int arFeedComponent[ FEED_NUM ][ COMPONENT_NUM ];
	memset( arFeedComponent, 0, sizeof( arFeedComponent ) );

	for ( int i=0; i<FEED_NUM; ++i )
	{
		for ( int k=0; k<COMPONENT_NUM;++k )
		{
			fin >> arFeedComponent[ i ][k];
		}
	}

	// ��ٴ���.
	int arFeedRatio[FEED_NUM] = { 0 };
	// ��ĿҪ�����������ĳɷ���Ŀ��ɷ�֮��ı���.
	int nRatioMyToGoal = 0;
	while ( arFeedRatio[ FEED_NUM-1 ] < 100 )
	{
		// ��ȡ��ǰ��������и��ֳɷֵı���.
		int arComponentRatio[ COMPONENT_NUM ] = {0};
		for ( int i=0; i<FEED_NUM; ++i )
		{
			for ( int k=0; k<COMPONENT_NUM; ++k )
			{
				// ��i�����ϵĵ�k�ֳɷ�.���ϱ��� * �����еĳɷֱ���.
				arComponentRatio[k] += arFeedRatio[i] * arFeedComponent[i][k];
			}
		}
		// ��ǰ�����Ƿ����Ҫ��.
		// �ȼ���Ŀ��ɷ����û�й�Լ��.
		bool bOk = true;
		int nRatio = -1;
		for ( int i=0; i<COMPONENT_NUM; ++i )
		{
			// �����0.
			if ( 0 == arGoalComponent[i]  )
			{
				if ( arComponentRatio[i] != 0 )
				{
					bOk = false;
					break;
				}
				else
				{
					continue; // ��Ϊ0,����Ҫ�󵫲���ȷ������.
				}		
			}

			if ( arComponentRatio[i] % arGoalComponent[i] != 0 ) // Ŀ��ɷֱ�����������ǰ�ɷ�.
			{
				bOk = false;
				break;
			}
			if ( nRatio == -1 )
			{
				nRatio = arComponentRatio[i] / arGoalComponent[i];
//				bOk = false
			}
			else if( nRatio == 0 || ( nRatio != arComponentRatio[i] / arGoalComponent[i] ) ) // ���ֳɷֵĵ�ǰ����Ҫ��Ŀ��ɷֵı�����ֵһ��.
			{
				bOk = false;
				break;
			}

		}
		if( bOk )
		{
			// �������ok��.
			nRatioMyToGoal = nRatio;
			break;
		}

		// ��һ�ִ���.
		++arFeedRatio[0];
		for ( int i=0; i<FEED_NUM-1; ++i )
		{
			if ( arFeedRatio[i] >= 100 )
			{
				arFeedRatio[i] = 0;
				arFeedRatio[i+1] ++;
			}
		}
	}

	// �Ƿ��ҵ��˷������������.
	if ( arFeedRatio[ FEED_NUM-1 ] < 100 )
	{
		// ok
		for( int i=0; i<FEED_NUM; ++i )
		{
			fout << arFeedRatio[i] << " ";
		}

		// �ҵı���ͬĿ������ı���.
		fout << nRatioMyToGoal << endl;

	}
	else
	{
		fout << NO_RATIO << endl;
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
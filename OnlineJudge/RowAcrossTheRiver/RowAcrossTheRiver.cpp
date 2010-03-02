// RowAcrossTheRiver.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"


#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <set>
#include <sstream>
#include <algorithm>

#include <memory.h>

#include <assert.h>

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

#ifdef _WIN32
#define THINKINGL 1
#endif

const int MAX_NUM = 4;

/** 当前的最小时间。 */
unsigned int g_nMinTime = -1;

/** 四个人的过河时间。 */
int g_arTime[MAX_NUM] = { 10, 5, 2, 1 };

/** 检查人是否在这边，可以过河。 */
#define IS_HERE( nState,nIndex ) 0 ==( ( nState >> nIndex ) & 0x01 )
/** 检查人是否在那边，可以过来。 */
#define IS_THERE( nState,nIndex ) 0 !=( ( nState >> nIndex ) & 0x01 ) 

/** 存放过程。 */
typedef std::vector<int> TStateList;
typedef std::vector< TStateList > TRowSequnce;

TRowSequnce g_tBestRowSequence;

void Come( int nState, unsigned int nTotalTime, TStateList tsl );

/** 过河。 
*	一定是2人过河。
*/
void Go( int nState, unsigned int nTotalTime, TStateList tsl )
{
	// 找出两人来。
	for ( int j=0; j<MAX_NUM; ++j )
	{
		if ( IS_HERE( nState, j ) )
		{			

			for ( int k=j+1; k<MAX_NUM; ++k )
			{
				if ( IS_HERE( nState, k ) )
				{

					int nTime = max( g_arTime[j] , g_arTime[k] );
					int nNextState = nState | ( 1 << j ) | ( 1 << k );
					TStateList tNextSL = tsl;
					tNextSL.push_back( nNextState );
					Come( nNextState, nTotalTime + nTime, tNextSL );					

				}
			}
		}		
	}


}

/** 回来接人。 
*	一定是一个人回来。
*/
void Come( int nState, unsigned int nTotalTime, TStateList tsl )
{
	const int ALL_THERE = 0x0F;
	if ( nState == ALL_THERE )
	{
		if ( g_nMinTime > nTotalTime )
		{
			g_tBestRowSequence.clear();
		}
		if ( g_nMinTime >= nTotalTime )
		{
			g_nMinTime = nTotalTime;

			g_tBestRowSequence.push_back( tsl );
		}
		return;
	}

	for ( int i=0; i<MAX_NUM; ++i )
	{
		if ( IS_THERE( nState, i ) )
		{
			// 这个人回去。
			int nTime = g_arTime[i];
			int nNextState = nState & ( ~( 1 << i ) );
			TStateList tNextSl = tsl;
			tNextSl.push_back( nNextState );
			
			Go( nNextState, nTotalTime + nTime, tNextSl );
		}
	}
	return ;
}

void PrintState( int nState )
{
	for( int i=0; i<MAX_NUM; ++i )
	{
		char cOut = 'A' + i;
		if ( IS_HERE( nState, i ) )
		{
			cout << cOut << "|";
		}
		else
		{
			cout << " |" << cOut;
		}
		cout << endl;
	}
}

int main( )
{
	// 使用1位表示一个人的状态，使用32位int的低4位表示所有4个人的状态。
	// 每个人的状态有两种，0表示还没过河。1表示已经过河。
	// 初始状态，大家都没过河。
	int nState = 0;
	TStateList tsl;
	tsl.push_back( nState );
	Go( nState, 0 , tsl );

	cout << "Min time: " << g_nMinTime << endl;
	cout << "共有 " << g_tBestRowSequence.size() << " 种方法。" << endl;

	for ( size_t i=0; i<g_tBestRowSequence.size(); ++i )
	{
		TStateList bt = g_tBestRowSequence[i];

		cout << "方法 " << i+1 << endl;
		for ( size_t m = 0; m<bt.size(); ++m )
		{
			PrintState( bt[m] );
			cout << "-->" << endl;
		}
		cout << "完成!!!" << endl;
	}

	return 0;
}


/*
ID: thinkin6
PROG: stall4
LANG: C++
*/

/** USA CO 4.2.2 :
The Perfect Stall
Hal Burch
Farmer John completed his new barn just last week, complete with all the latest milking technology. Unfortunately, due to engineering problems, all the stalls in the new barn are different. For the first week, Farmer John randomly assigned cows to stalls, but it quickly became clear that any given cow was only willing to produce milk in certain stalls. For the last week, Farmer John has been collecting data on which cows are willing to produce milk in which stalls. A stall may be only assigned to one cow, and, of course, a cow may be only assigned to one stall.

Given the preferences of the cows, compute the maximum number of milk-producing assignments of cows to stalls that is possible.

PROGRAM NAME: stall4

INPUT FORMAT

Line 1:	 One line with two integers, N (0 <= N <= 200) and M (0 <= M <= 200). N is the number of cows that Farmer John has and M is the number of stalls in the new barn.
Line 2..N+1:	 N lines, each corresponding to a single cow. The first integer (Si) on the line is the number of stalls that the cow is willing to produce milk in (0 <= Si <= M). The subsequent Si integers on that line are the stalls in which that cow is willing to produce milk. The stall numbers will be integers in the range (1..M), and no stall will be listed twice for a given cow.
SAMPLE INPUT (file stall4.in)

5 5
2 2 5
3 2 3 4
2 1 5
3 1 2 5
1 2 
OUTPUT FORMAT

A single line with a single integer, the maximum number of milk-producing stall assignments that can be made.

SAMPLE OUTPUT (file stall4.out)

4
*/

/** 
*	
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
#include <stack>




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

const u32 INFINITE = 0xFFFFFF;	// 无限大.
typedef std::vector< u32 > TWeightList;
typedef std::vector< TWeightList > TWeightTable;

u32 GetMaxFlow( const TWeightTable& weightTable, int source, int dst )
{
	int nPointNum = weightTable.size();
	int nMaxFlow = 0;

	TWeightTable flowTable = weightTable;

	// 一直寻找最大路径
	while ( true )
	{
		typedef std::vector< bool > TVisited;
		TVisited visited( nPointNum, false );
		TWeightList tFlow( nPointNum, 0 );

		tFlow[ source ] = INFINITE;
		u32 maxflow = 0;
		int maxflowIndex = -1;
		for ( int i=0; i<nPointNum; ++i )
		{
			if ( !visited[ i ] && tFlow[i] > maxflow )
			{
				maxflow = tFlow[i];
				maxflowIndex = i;
			}
		}

		if ( maxflowIndex == -1 )
		{
			break;
		}
		
	}
	return 0;
}

int main()
{
	string strProblemName = "stall4";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	// Maxmum Matching problem.

	int nCowNum, nStallNum;
	fin >> nCowNum >> nStallNum;

	// 节点矩阵的大小,是牛的数目 + Stalls 的数目 + source + sink.
	int nTotalNum = nCowNum + nStallNum + 2;

	// 矩阵
	TWeightTable tNetworkFlowTable( nTotalNum, TWeightList( nTotalNum, 0 ) );

	for ( int i=0; i<nCowNum; ++i )
	{
		int nCowIndex = i + 2;	// 奶牛在表中的序号.
		tNetworkFlowTable[0][ nCowIndex ] = 1;
		int nStallNum;
		for ( int k=0; k<nStallNum; ++k )
		{
			int nStallIndex;
			fin >> nStallIndex;

			int nStallIndexInTable = 2 + nCowNum + nStallIndex - 1;

			tNetworkFlowTable[ nCowIndex ][ nStallIndexInTable ] = 1;
		}
	}

	for ( int i=0; i<nStallNum; ++i )
	{
		int nStallIndexInTable = 2 + nCowNum + i;
		tNetworkFlowTable[1][nStallIndexInTable] = 1;
	}

	int nMaxFlow = GetMaxFlow( tNetworkFlowTable, 0, 1 );

	int maxCowNum = nMaxFlow / 3;
	fout << maxCowNum << endl;


#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}
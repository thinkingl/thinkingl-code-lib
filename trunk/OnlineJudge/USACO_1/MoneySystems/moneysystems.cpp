/*
ID: thinkin6
PROG: money
LANG: C++
*/

/** USA CO 2.3.4 :

Money Systems
The cows have not only created their own government but they have chosen to create their own money system. In their own rebellious way, they are curious about values of coinage. Traditionally, coins come in values like 1, 5, 10, 20 or 25, 50, and 100 units, sometimes with a 2 unit coin thrown in for good measure.

The cows want to know how many different ways it is possible to dispense a certain amount of money using various coin systems. For instance, using a system of {1, 2, 5, 10, ...} it is possible to create 18 units several different ways, including: 18x1, 9x2, 8x2+2x1, 3x5+2+1, and many others.

Write a program to compute how many ways to construct a given amount of money using supplied coinage. It is guaranteed that the total will fit into both a signed long long (C/C++) and Int64 (Free Pascal).

PROGRAM NAME: money

INPUT FORMAT

The number of coins in the system is V (1 <= V <= 25).

The amount money to construct is N (1 <= N <= 10,000).
Line 1:	Two integers, V and N
Lines 2..:	V integers that represent the available coins (no particular number of integers per line)
SAMPLE INPUT (file money.in)

3 10
1 2 5
OUTPUT FORMAT

A single line containing the total number of ways to construct N money units using V coins.
SAMPLE OUTPUT (file money.out)

10


/** 
思路：


*/
#include <stdio.h>
#include <stdlib.h>

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

#if !defined( _WIN32 )
#define _itoa itoa

char * itoa( int _Val, char * _DstBuf, int _Radix)
{
	sprintf( _DstBuf, "%d", _Val );
	return _DstBuf;
}

#endif

#ifdef _WIN32
#define THINKINGL 1
#endif

 const int MAX_MONEY_UNIT_NUM = 100+1;
 const int MAX_MONEY_AMOUNT = 10000+1;
// 
 int g_arMoneyUnit[ MAX_MONEY_UNIT_NUM ];
// 
// u64 (*g_arDPUnitAmount)[MAX_MONEY_AMOUNT] ; // [MAX_MONEY_UNIT_NUM][MAX_MONEY_AMOUNT];
// 
// /** 用tMoneyUnit这些零钱凑齐nAmount，有多少种组合。*/
// u64 CountNum( int nMoneyUnitIndex, int nAmount )
// {
// 	if ( g_arDPUnitAmount[ nMoneyUnitIndex][nAmount] != -1 )
// 	{
// 		return g_arDPUnitAmount[ nMoneyUnitIndex][nAmount];
// 	}
// 
// 	// 只剩下一种零钱了。
// 	if( nMoneyUnitIndex ==  0 )
// 	{
// 		int nUnit = g_arMoneyUnit[ nMoneyUnitIndex ];
// 		bool bOk = ( nAmount % nUnit ) == 0;
// 		return bOk ? 1 : 0;
// 	}
// 
// 
// 	u64 uCountNum = 0;
// 	int nLastUnit = g_arMoneyUnit[ nMoneyUnitIndex ];
// 
// 	for ( int nLeft = nAmount; nLeft >= 0; nLeft -= nLastUnit )
// 	{
// 		u64 uOtherCount = CountNum( nMoneyUnitIndex - 1, nLeft );
// 		uCountNum += uOtherCount;
// 	}
// 
// 	g_arDPUnitAmount[ nMoneyUnitIndex][nAmount] = uCountNum;
// 
// 	return uCountNum;
// 
// }

u64 g_arDPAmount[MAX_MONEY_AMOUNT+1];

int main()
{
	string strProblemName = "money";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nNum, nMoneyAmount;

	fin >> nNum >> nMoneyAmount;

	int nUnit;
	for ( int i=0; i<nNum; ++i )
	{
		fin >> nUnit;
		g_arMoneyUnit[i] = nUnit;
	}

// 	g_arDPUnitAmount = new  u64[MAX_MONEY_UNIT_NUM][MAX_MONEY_AMOUNT];
// 
// 	int nLen = sizeof( u64 ) * MAX_MONEY_UNIT_NUM * MAX_MONEY_AMOUNT;
// 	memset( g_arDPUnitAmount, -1, nLen );
// 
// 
// 	u64 uCountNum = CountNum( nNum - 1, nMoneyAmount );

	memset( g_arDPAmount, 0, sizeof( g_arDPAmount ) );

	g_arDPAmount[0] = 1;


	for ( int nUnitNum = 0; nUnitNum < nNum; ++nUnitNum )
	{		
		int nCurUnit = g_arMoneyUnit[ nUnitNum ];
		for ( int nAmount = nCurUnit; nAmount <= nMoneyAmount; ++ nAmount )
		{
			g_arDPAmount[ nAmount ] += g_arDPAmount[ nAmount - nCurUnit ];
		}

	}
	u64 uCountNum = g_arDPAmount[ nMoneyAmount ];
	fout << uCountNum << endl;

	fin.close();
	fout.close();
//	delete[] g_arDPUnitAmount;

#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );

	cout << endl;

	system( "pause" );
#endif

	return 0;
}
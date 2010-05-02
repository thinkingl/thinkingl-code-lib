/*
ID: thinkin6
PROG: kimbits
LANG: C++
*/

/** USA CO 3.2.2 :
Stringsobits
Kim Schrijvers
Consider an ordered set S of strings of N (1 <= N <= 31) bits. Bits, of course, are either 0 or 1.

This set of strings is interesting because it is ordered and contains all possible strings of length N that have L (1 <= L <= N) or fewer bits that are `1'.

Your task is to read a number I (1 <= I <= sizeof(S)) from the input and print the Ith element of the ordered set for N bits with no more than L bits that are `1'.

PROGRAM NAME: kimbits

INPUT FORMAT

A single line with three space separated integers: N, L, and I.
SAMPLE INPUT (file kimbits.in)

5 3 19
OUTPUT FORMAT

A single line containing the integer that represents the Ith element from the order set, as described.
SAMPLE OUTPUT (file kimbits.out)

10011


*/

/** 
思路：
*	实现大数乘法.
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

/** 获取nTotalBits长度下,不多于nMax1Num个1时可以组成的组合数目. 
*/
u32 GetCount( int nMax1Num, int nTotalBits )
{
	// 判断结束条件.
	if ( nMax1Num == 0 )
	{
		return 1;
	}
	if ( nTotalBits == 0 )
	{
		return 1;
	}

	u32 nCount = 0;
	// 最高位0
	nCount += GetCount( nMax1Num, nTotalBits - 1 );
	// 最高位1
	nCount += GetCount( nMax1Num-1, nTotalBits-1 );
	return nCount;
}

/** 循环的方式得到结果. */
u32 GetCountFast( int nMax1Num, int nTotalBits )
{
	const int MAX_1_NUM = 32;
	const int MAX_TOTAL_BITS = 32;
	static u32 s_arCount[MAX_1_NUM][MAX_TOTAL_BITS];	
	static bool s_bInited = false;

	if ( !s_bInited )
	{
		memset( s_arCount, 0, sizeof( s_arCount ) );
		s_bInited = true;

		// 初始化数组.
		for ( int i=0; i<MAX_1_NUM; ++i )
		{
			s_arCount[i][0] = 1;
		}
		for ( int i=0; i<MAX_TOTAL_BITS; ++i )
		{
			s_arCount[0][i] = 1;
		}

		// 循环获取.
		// f( nMax1Num, nTotalBits ) = f( nMax1Num, nTotalBits-1 ) + f( nMax1Num-1, nTotalBits-1 )
		for ( int i=1; i<MAX_TOTAL_BITS; ++i )
		{
			for ( int k=1; k<=MAX_1_NUM; ++k )
			{
				if ( k>i )
				{
					s_arCount[k][i] = s_arCount[i][i];
				}
				else
				{
					s_arCount[k][i] = s_arCount[k][i-1] + s_arCount[k-1][i-1];
				}
				
			}
		}
	}

	return s_arCount[nMax1Num][nTotalBits];
}

int main()
{
	string strProblemName = "kimbits";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nBitsNum, nMax1Num, nIndex;
	fin >> nBitsNum >> nMax1Num >> nIndex;

	stringstream ssTargetNum;
	for ( int i=nBitsNum; i>0; --i )
	{		
		if ( GetCountFast( nMax1Num, nBitsNum-1 ) >= nIndex )
		{
			// 最高位0.可以满足数目要求.
			ssTargetNum << '0';
		}
		else
		{
			// 最高位1,去掉0时的组合.
			ssTargetNum << '1';
			nIndex -= GetCountFast( nMax1Num, nBitsNum-1 );
			nMax1Num --; // 最高位占用一个1.			
		}

		// 去掉最高位.
		nBitsNum --;
	}

	cout << ssTargetNum.str() << endl;
	fout << ssTargetNum.str() << endl;

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
/*
ID: thinkin6
PROG: zerosum
LANG: C++
*/

/** USA CO 2.3.3 :

Zero Sum
Consider the sequence of digits from 1 through N (where N=9) in increasing order: 1 2 3 ... N.

Now insert either a `+' for addition or a `-' for subtraction or a ` ' [blank] to run the digits together between each pair of digits (not in front of the first digit). Calculate the result that of the expression and see if you get zero.

Write a program that will find all sequences of length N that produce a zero sum.

PROGRAM NAME: zerosum

INPUT FORMAT

A single line with the integer N (3 <= N <= 9).
SAMPLE INPUT (file zerosum.in)

7
OUTPUT FORMAT

In ASCII order, show each sequence that can create 0 sum with a `+', `-', or ` ' between each pair of numbers.
SAMPLE OUTPUT (file zerosum.out)

1+2-3+4-5-6+7
1+2-3-4+5+6-7
1-2 3+4+5+6+7
1-2 3-4 5+6 7
1-2+3+4-5+6-7
1-2-3-4-5+6+7

/** 
思路：
数据规模小。
模拟，穷举。

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

int g_nMaxNum;

typedef set< string > TStringSet;
TStringSet g_tResultSet;

void ZeroSum( int nNumPos, int nCurSum, int nCurNum, string curSeq, ofstream *pfout )
{
	nNumPos++;

	if ( nNumPos > g_nMaxNum )
	{
		if ( nCurSum + nCurNum == 0 )
		{
			// ok,this is it.
			g_tResultSet.insert( curSeq );
			
			if ( pfout )
			{
				*pfout << curSeq << endl;
			}

		}
		return;
	}

	int nNum = nNumPos;

	char arNum[5]={0};

	// blank
	if ( nCurNum >= 0 )
	{
		ZeroSum( nNumPos, nCurSum, nCurNum * 10 + nNum, curSeq + " " + _itoa( nNum, arNum, 10 ), pfout );
	}
	else 
	{
		ZeroSum( nNumPos, nCurSum, nCurNum * 10 - nNum, curSeq + " " + _itoa( nNum, arNum, 10 ), pfout );
	}	

	// +	
	ZeroSum( nNumPos, nCurSum + nCurNum, nNum, curSeq  + "+" + _itoa( nNum, arNum, 10 ), pfout );

	// -
	ZeroSum( nNumPos, nCurSum + nCurNum, -nNum, curSeq  + "-" + _itoa( nNum, arNum, 10 ), pfout );


	
}

int main()
{
	string strProblemName = "zerosum";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nNum;
	fin >> nNum;

	g_nMaxNum = nNum;

	// 1
	// 当前位置 1之后，是1.
	// 当前和为0.
	// 当前未确定的数 1
	// 当前字串 “1” 
	ZeroSum( 1, 0, 1, "1", &fout );


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
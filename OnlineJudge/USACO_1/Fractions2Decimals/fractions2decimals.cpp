/*
ID: thinkin6
PROG: fracdec
LANG: C++
*/

/** USA CO 2.4.6 :
Fractions to Decimals
Write a program that will accept a fraction of the form N/D, where N is the numerator and D is the denominator and print the decimal representation. If the decimal representation has a repeating sequence of digits, indicate the sequence by enclosing it in brackets. For example, 1/3 = .33333333...is denoted as 0.(3), and 41/333 = 0.123123123...is denoted as 0.(123). Use xxx.0 to denote an integer. Typical conversions are:

1/3     =  0.(3)
22/5    =  4.4
1/7     =  0.(142857)
2/2     =  1.0
3/8     =  0.375
45/56   =  0.803(571428)
PROGRAM NAME: fracdec

INPUT FORMAT

A single line with two space separated integers, N and D, 1 <= N,D <= 100000.
SAMPLE INPUT (file fracdec.in)

45 56
OUTPUT FORMAT

The decimal expansion, as detailed above. If the expansion exceeds 76 characters in length, print it on multiple lines with 76 characters per line.
SAMPLE OUTPUT (file fracdec.out)

0.803(571428)
*/


/** 
思路：
模拟除法竖式计算过程。
当余数重复出现的时候，就是循环出现的时候。
很有趣的题目，当两个运算数字很大的时候，循环体会非常的长。

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
#include <complex>

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


int main()
{
	string strProblemName = "fracdec";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int n,d;
	fin >> n >> d;

	typedef int TRemainder;
	typedef int TResult;

	typedef set<TRemainder> TRemainderSet;
	typedef pair<TRemainder, TResult> TDevisionExp;
	typedef vector< TDevisionExp > TDevisionExpList;

	TRemainderSet tRemainderSet;
	TDevisionExpList tDevisionExpList;

	int nLoopN = 0;
	while( 1 )
	{
		if ( tRemainderSet.find( n ) != tRemainderSet.end() )
		{
			// 有循环。
			nLoopN = n;
			break;
		}

		if ( n == 0 )
		{
			// 结束。
			break;
		}

		tRemainderSet.insert( n );

		int nRet = n / d;

		tDevisionExpList.push_back( TDevisionExp( n, nRet ) );

		n -= nRet * d;
		n *= 10;

	}

	fout << tDevisionExpList[0].second;
	if ( tDevisionExpList.size() > 1 )
	{
		fout << ".";
		int i=1;
		int nRemainder = tDevisionExpList[i].first;
		while( nRemainder != nLoopN  )
		{
			fout << tDevisionExpList[i].second;
			++i;
			if ( i>= tDevisionExpList.size() )
			{
				break;
			}
			nRemainder = tDevisionExpList[i].first;
		}

		if (  i < tDevisionExpList.size() )
		{
			fout << "(";

			for ( ; i<tDevisionExpList.size(); ++ i )
			{
				fout << tDevisionExpList[i].second;
			}

			fout << ")";
		}
	}
	else if( nLoopN != 0 )
	{
		fout << ".(" << tDevisionExpList[0].second << ")";
	}
	
	fout << endl;

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
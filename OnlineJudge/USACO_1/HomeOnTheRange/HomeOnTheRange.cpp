/*
ID: thinkin6
PROG: range
LANG: C++
*/

/** USA CO 3.3.5 :
Home on the Range
Farmer John grazes his cows on a large, square field N (2 <= N <= 250) miles on a side (because, for some reason, his cows will only graze on precisely square land segments). Regrettably, the cows have ravaged some of the land (always in 1 mile square increments). FJ needs to map the remaining squares (at least 2x2 on a side) on which his cows can graze (in these larger squares, no 1x1 mile segments are ravaged).

Your task is to count up all the various square grazing areas within the supplied dataset and report the number of square grazing areas (of sizes >= 2x2) remaining. Of course, grazing areas may overlap for purposes of this report.

PROGRAM NAME: range

INPUT FORMAT

Line 1:	 N, the number of miles on each side of the field.
Line 2..N+1:	 N characters with no spaces. 0 represents "ravaged for that block; 1 represents "ready to eat".
SAMPLE INPUT (file range.in)

6
101111
001111
111111
001111
101101
111001
OUTPUT FORMAT

Potentially several lines with the size of the square and the number of such squares that exist. Order them in ascending order from smallest to largest size.

SAMPLE OUTPUT (file range.out)

2 10
3 4
4 1  
*/

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



int main()
{
	string strProblemName = "range";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nFieldSize;
	fin >> nFieldSize;

	typedef std::vector< int > TIntVector;
	typedef std::vector< TIntVector > TFieldStat;

	const int MAX_FIELD_SIZE = 250;
	int tFieldStat[ MAX_FIELD_SIZE ][MAX_FIELD_SIZE];

	// 牧场的状态。
//	TFieldStat tFieldStat( nFieldSize, TIntVector( nFieldSize ) );

	for( int i=0; i<nFieldSize; ++i )
	{
		for( int k=0; k<nFieldSize; ++k )
		{
			char bRavaged;
			fin >> bRavaged;
			if( bRavaged == '0' || bRavaged == '1' )
			{
				tFieldStat[i][k] = bRavaged - '0';
			}
			else
			{
				--k;	// 跳过换行符。
			}
		}
	}

	// 可以放牧的方块的数目。下标是方块大小。
	TIntVector tRemainingSquaresNum( nFieldSize+1, 0 );

	// 存放是否可以放牧。
	TIntVector tMapCanGraze( MAX_FIELD_SIZE * MAX_FIELD_SIZE * MAX_FIELD_SIZE, 0 );
	int (abc)[10][10][10] ;

	// 新的方法。
	// 先求出


	for( int nY=0; nY<nFieldSize; ++nY )
	{
		for( int nX=0; nX<nFieldSize; ++nX )
		{
			int nSize = 1;
			while( 1 )
			{
				bool bCanGraze = true;
				// 检查右边。
				int nXR = nX + nSize-1;
				for( int nYR = nY; nYR < nY + nSize; ++nYR )
				{
					if( nXR >= nFieldSize || nYR >= nFieldSize )
					{
						bCanGraze = false;
						break;
					}
					if( !tFieldStat[ nXR ][ nYR ] )
					{
						bCanGraze = false;
						break;
					}
				}

				// 检查下边。
				if( bCanGraze )
				{
					int nYB = nY + nSize-1;
					for( int nXB=nX; nXB < nX + nSize; ++nXB )
					{
						if( nYB >= nFieldSize || nXB >= nFieldSize )
						{
							bCanGraze = false;
							break;
						}
						if( !tFieldStat[ nXB ] [ nYB ] )
						{
							bCanGraze = false;
							break;
						}
					}
				}

				if( bCanGraze )
				{
					tRemainingSquaresNum[ nSize ] ++;
				}
				else
				{
					break;
				}

				++nSize;
			}
		}
	}

	// 输出结果。
	for( int nSize = 2; nSize<=nFieldSize; ++nSize )
	{
		if( tRemainingSquaresNum[ nSize ] )
		{
			fout << nSize << " " << tRemainingSquaresNum[ nSize ] << endl;
		}
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
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
*	思路：
*	开始时觉得好简单，遍历所有的点，然后从这一点出发检测各个大小的可放牧农场方块是否存在，
*	如果存在，则计数加一。如果不存在，则跳出循环，检测下个点。
*	结果超时，超的不多。这时才计算了下时间复杂度，虽然有剪枝，但也是个4次方的。

*	重新考虑了下，发现逐个点检测是否可放牧的方法可以改进，改进为动态规划（DP）的样子：
*	1. 以各个点为左上角的大小是一的可放牧农场方块是否存在的状态就是当前农场的可放牧方块状态。
*	2. 如果知道以一个点（x，y）为左上角的大小是n的可放牧农场方块是存在的，则以这个点（x，y）为左上角的大小是 n+1 的可放牧农场方块是否存在需要4个条件都符合：

*		以（x+1,y）为左上角的大小是n的可放牧农场方块存在。（右移）
*		以（x，y+1）为左上角的大小是n的可放牧农场方块存在。（下移）
*		点（x+n，y+n）是可以放牧的。（向右向下晃两下后还剩下这个角）
*		以上所有的点都在农场内，没有越界。
*	3. 从1开始，用2进行迭代，直到算出所有大小可放牧农场方块的数目。
*	
*	这样，时间复杂度就变成了三次幂，很快了。
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
	// 牧场的状态。
	int tFieldStat[ MAX_FIELD_SIZE ][MAX_FIELD_SIZE];
	
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
	TIntVector tMapCanGraze( MAX_FIELD_SIZE * MAX_FIELD_SIZE , 0 );
	int (*parMapCanGraze)[MAX_FIELD_SIZE] = ( int(*)[MAX_FIELD_SIZE] )&tMapCanGraze[0];


	int nCurSize = 1;
	// 大小是1的可放牧的方块的情况就是牧场状况表。
	for( int nY=0; nY<nFieldSize; ++nY )
	{
		for( int nX=0; nX<nFieldSize; ++nX )
		{
			parMapCanGraze[nY][nX] = tFieldStat[nY][nX];
			tRemainingSquaresNum[nCurSize] += ( parMapCanGraze[nY][nX] ? 1 : 0 );
		}
	}

	
	while( nCurSize < nFieldSize )
	{
		for( int nY=0; nY<nFieldSize; ++nY )
		{
			for( int nX=0; nX<nFieldSize; ++nX )
			{
				if( parMapCanGraze[nY][nX] )
				{
					// 是否越界。
					if( nY >= nFieldSize - nCurSize || nX >= nFieldSize - nCurSize )
					{
						parMapCanGraze[nY][nX] = 0; 
						continue;
					}
					// 右边是否可以？
					if( !parMapCanGraze[nY][nX+1] )
					{
						parMapCanGraze[nY][nX] = 0; 
						continue;
					}

					// 下边是否可以？
					if( !parMapCanGraze[nY+1][nX] )
					{
						parMapCanGraze[nY][nX] = 0; 
						continue;
					}

					// 右下角那个是否可以？
					if( !tFieldStat[nY+nCurSize][nX+nCurSize] )
					{
						parMapCanGraze[nY][nX] = 0; 
						continue;
					}

					// 能执行到这里，说明这个格子可以。
					tRemainingSquaresNum[ nCurSize + 1 ] ++;

				}
			}
		}

		nCurSize ++;
	}

#if 0
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
#endif
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
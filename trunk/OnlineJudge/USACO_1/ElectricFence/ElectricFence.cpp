/*
ID: thinkin6
PROG: fence9
LANG: C++
*/

/** USA CO 3.4.4 :
Electric Fence
Don Piele
In this problem, `lattice points' in the plane are points with integer coordinates.

In order to contain his cows, Farmer John constructs a triangular electric fence by stringing a "hot" wire from the origin (0,0) to a lattice point [n,m] (0<=;n<32000, 0<m<32000), then to a lattice point on the positive x axis [p,0] (p>0), and then back to the origin (0,0).

A cow can be placed at each lattice point within the fence without touching the fence (very thin cows). Cows can not be placed on lattice points that the fence touches. How many cows can a given fence hold?

PROGRAM NAME: fence9

INPUT FORMAT

The single input line contains three space-separated integers that denote n, m, and p.

SAMPLE INPUT (file fence9.in)

7 5 10
OUTPUT FORMAT

A single line with a single integer that represents the number of cows the specified fence can hold.

SAMPLE OUTPUT (file fence9.out)

20
*/

/** 
思路：开始想复杂了，把每个坐标点看成一个个格子，结果得不出正确结果。
*	原来题目中坐标点就是点，不是格子。那些牛是真的很瘦很瘦，very very thin。
*	没有重写，还是基于数格子的方法，改为了数点。处理的很艰难，总出错。
*	就是数，从左到右数。
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

int Integrate( int startX, int startY, int endX, int endY, 
	int& firstCol, int& lastCol )
{
	firstCol = lastCol = 0;
	

	int area = 0;
	// 是正还是负。
	int sgn = ( startX < endX ) ? 1 : -1;
	// 水平每个点挨个算。
	int maxI = abs( endX - startX );
	for ( int x=startX; x != endX + sgn ; x+=sgn )
	{
		// y坐标。
		double y = startY + double ( x - startX ) * ( endY - startY ) / ( endX - startX );

		// 在线上的不算。但在排除的时候要排除掉。
		int picceArea = y - sgn * 0.000000001;
		picceArea = max( 0, picceArea );

		area += sgn * picceArea;

		if ( x==startX )
		{
			firstCol = picceArea;
		}
		if ( x == endX )
		{
			lastCol = picceArea;
		}
	}
	return area;
}

int main()
{
	string strProblemName = "fence9";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int n,m,p;
	fin >> n >> m >> p;

	int totalCowNum = 0;

	int first1,last1;
	// 从（0，0）到（n，m）的线段面积积分。
	totalCowNum += Integrate( 0, 0, n, m , first1, last1 );

	// 从(n，m) 到 (p,0) 的线段面积积分（如果n>p,那么面积应该是负的。）
	int first2,last2;
	int secondArea = Integrate( n,m, p,0, first2, last2 );
	totalCowNum += secondArea;
	if( secondArea >=0 )
	{
		// 当第二根线不是排除的时候，转折点所在纵轴会被多数一次。
		totalCowNum -= max( last1, first2 ); 
	}
	else
	{	// 当第二根线是向回拐，排除的时候，转折点（n，m）会被多排除一次，所以++。
		totalCowNum ++;
	}

	fout << totalCowNum << endl;

#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}
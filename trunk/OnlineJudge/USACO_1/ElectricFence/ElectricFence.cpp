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
˼·����ʼ�븴���ˣ���ÿ������㿴��һ�������ӣ�����ò�����ȷ�����
*	ԭ����Ŀ���������ǵ㣬���Ǹ��ӡ���Щţ����ĺ��ݺ��ݣ�very very thin��
*	û����д�����ǻ��������ӵķ�������Ϊ�����㡣����ĺܼ��ѣ��ܳ���
*	������������������
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
	// �������Ǹ���
	int sgn = ( startX < endX ) ? 1 : -1;
	// ˮƽÿ���㰤���㡣
	int maxI = abs( endX - startX );
	for ( int x=startX; x != endX + sgn ; x+=sgn )
	{
		// y���ꡣ
		double y = startY + double ( x - startX ) * ( endY - startY ) / ( endX - startX );

		// �����ϵĲ��㡣�����ų���ʱ��Ҫ�ų�����
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
	// �ӣ�0��0������n��m�����߶�������֡�
	totalCowNum += Integrate( 0, 0, n, m , first1, last1 );

	// ��(n��m) �� (p,0) ���߶�������֣����n>p,��ô���Ӧ���Ǹ��ġ���
	int first2,last2;
	int secondArea = Integrate( n,m, p,0, first2, last2 );
	totalCowNum += secondArea;
	if( secondArea >=0 )
	{
		// ���ڶ����߲����ų���ʱ��ת�۵���������ᱻ����һ�Ρ�
		totalCowNum -= max( last1, first2 ); 
	}
	else
	{	// ���ڶ���������عգ��ų���ʱ��ת�۵㣨n��m���ᱻ���ų�һ�Σ�����++��
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
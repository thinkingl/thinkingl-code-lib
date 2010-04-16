/*
ID: thinkin6
PROG: agrinet
LANG: C++
*/

/** USA CO 3.1.1 :
Agri-Net
Russ Cox
Farmer John has been elected mayor of his town! One of his campaign promises was to bring internet connectivity to all farms in the area. He needs your help, of course.

Farmer John ordered a high speed connection for his farm and is going to share his connectivity with the other farmers. To minimize cost, he wants to lay the minimum amount of optical fiber to connect his farm to all the other farms.

Given a list of how much fiber it takes to connect each pair of farms, you must find the minimum amount of fiber needed to connect them all together. Each farm must connect to some other farm such that a packet can flow from any one farm to any other farm.

The distance between any two farms will not exceed 100,000.

PROGRAM NAME: agrinet

INPUT FORMAT

Line 1:	 The number of farms, N (3 <= N <= 100).
Line 2..end:	 The subsequent lines contain the N x N connectivity matrix, where each element shows the distance from on farm to another. Logically, they are N lines of N space-separated integers. Physically, they are limited in length to 80 characters, so some lines continue onto others. Of course, the diagonal will be 0, since the distance from farm i to itself is not interesting for this problem.
SAMPLE INPUT (file agrinet.in)

4
0 4 9 21
4 0 8 17
9 8 0 16
21 17 16 0
OUTPUT FORMAT

The single output contains the integer length that is the sum of the minimum length of fiber required to connect the entire set of farms.

SAMPLE OUTPUT (file agrinet.out)

28
*/

/** 
思路：
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

const int PARENT_NULL = -1;

void Prim( const u32 arWeightMatrix[], int nVertexNum, int nMatrixSize, int arParent[] )
{
	u32* arDistance = new u32[nVertexNum];

	bool* arInTree = new bool[nVertexNum];

	for ( int i=0; i<nVertexNum; ++i )
	{
		arParent[i] = PARENT_NULL;
		arDistance[i] = -1;
		arInTree[i] = false;
	}

	arInTree[0] = true;
	
	// 寻找离当前树最近的外面的点。
	u32 nNearestVertexDistance = 0;
	int nNearestVertexIndex = 0;
	do
	{
		// 将新节点加入，更新最短距离。
		arInTree[ nNearestVertexIndex ]  = true;
		arDistance[ nNearestVertexIndex ] = 0;
		//		arParent[nNearestVertexIndex] = 
		for ( int i=0; i<nVertexNum; ++i )
		{
			if ( i!=nNearestVertexIndex && arWeightMatrix[ nNearestVertexIndex * nMatrixSize + i ] < arDistance[i] )
			{
				arDistance[i] = arWeightMatrix[ nNearestVertexIndex*nMatrixSize + i ];
				arParent[i] = nNearestVertexIndex;
			}
		}

		nNearestVertexDistance = -1;
		for ( int i=0; i<nVertexNum; ++i )
		{
			if ( !arInTree[i] )
			{
				if ( nNearestVertexDistance > arDistance[i] )
				{
					nNearestVertexDistance = arDistance[i];
					nNearestVertexIndex = i;
				}
			}
		}
		// 没找到，说明没了。
		if ( nNearestVertexDistance == -1 )
		{
			break;	// over
		}
		
		
	}
	while( nNearestVertexDistance != -1 );
	


	delete [] arDistance;
	delete [] arInTree;
}

int main()
{
	string strProblemName = "agrinet";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nFarmNum;
	fin >> nFarmNum;

	const int MAX_FARM_NUM = 100;

	u32 arWeight[MAX_FARM_NUM][MAX_FARM_NUM];

	for ( int i=0; i<nFarmNum; ++i )
	{
		for ( int k=0; k<nFarmNum; ++k )
		{
			int nWeight;
			fin >> nWeight;
			arWeight[i][k] = nWeight;
			arWeight[k][i] = nWeight;
		}
	}

	int arParent[MAX_FARM_NUM];
	Prim( (u32*)arWeight, nFarmNum, MAX_FARM_NUM, arParent );

	int nSum = 0;
	for ( int i=0; i<nFarmNum; ++i )
	{
		if ( arParent[i] != PARENT_NULL )
		{
			nSum += arWeight[i][arParent[i]];
		}
	}

	fout << nSum << endl;

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
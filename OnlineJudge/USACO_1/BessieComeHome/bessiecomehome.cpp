/*
ID: thinkin6
PROG: comehome
LANG: C++
*/

/** USA CO 2.4.5 :
Bessie Come Home
Kolstad & Burch
It's dinner time, and the cows are out in their separate pastures. Farmer John rings the bell so they will start walking to the barn. Your job is to figure out which one cow gets to the barn first (the supplied test data will always have exactly one fastest cow).

Between milkings, each cow is located in her own pasture, though some pastures have no cows in them. Each pasture is connected by a path to one or more other pastures (potentially including itself). Sometimes, two (potentially self-same) pastures are connected by more than one path. One or more of the pastures has a path to the barn. Thus, all cows have a path to the barn and they always know the shortest path. Of course, cows can go either direction on a path and they all walk at the same speed.

The pastures are labeled `a'..`z' and `A'..`Y'. One cow is in each pasture labeled with a capital letter. No cow is in a pasture labeled with a lower case letter. The barn's label is `Z'; no cows are in the barn, though.

PROGRAM NAME: comehome

INPUT FORMAT

Line 1:	 Integer P (1 <= P <= 10000) the number of paths that interconnect the pastures (and the barn)
Line 2..P+1:	Space separated, two letters and an integer: the names of the interconnected pastures/barn and the distance between them (1 <= distance <= 1000)
SAMPLE INPUT (file comehome.in)

5
A d 6
B d 3
C e 9
d Z 8
e Z 3
OUTPUT FORMAT

A single line containing two items: the capital letter name of the pasture of the cow that arrives first back at the barn, the length of the path followed by that cow.
SAMPLE OUTPUT (file comehome.out)

B 11
*/


/** 
思路：
*	晕死！大写的字母和小写的字母表示的是不同的Pasture！！！！我郁闷了。


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


const int MAX_PASTURE_NUM = 26*2;

const u32 MIN_WEIGHT_NOT_CONNECTED = 10001 * MAX_PASTURE_NUM * 2;

/** 
*
*
*
*/
void FloydWarshall( const u32 arWeightMatrix[], u32 arDistanceMatrix[], int nMatrixSize, int VertexNum )
{
	for ( int i=0; i<VertexNum; ++i )
	{
		for ( int j=0; j<VertexNum; ++j )
		{
			u32 nWeight = MIN_WEIGHT_NOT_CONNECTED;
			if ( i==j )
			{
				nWeight = 0;
			}
			else
			{
				nWeight = arWeightMatrix[ i * nMatrixSize + j ];
			}
			arDistanceMatrix[ i * nMatrixSize + j ] = nWeight;
		}
	}

	for ( int k=0; k<VertexNum; ++k )
	{
		for ( int i=0; i<VertexNum; ++i )
		{
			for ( int j=0; j<VertexNum; ++j )
			{
				u32 nDistThroughK = arDistanceMatrix[ i*nMatrixSize + k ] + arDistanceMatrix[ k*nMatrixSize + j ];
				if ( nDistThroughK < arDistanceMatrix[ i * nMatrixSize + j ] )
				{
					arDistanceMatrix[ i*nMatrixSize + j ] = nDistThroughK;
				}
			}
		}
	}
}

typedef pair<u32,u32> TIndexWeight;
typedef vector< TIndexWeight > TIndexWeightList;
typedef vector< TIndexWeightList > TVertexNeighbourWeightTable;


void Dijkstra( int nSourceIndex, const TVertexNeighbourWeightTable* pWeightTable, u32 arDistance[],
			  int arParent[] )
{
	typedef list<int> TIntList;
	TIntList tNotVisitedList; 
	int nVertexNum = pWeightTable->size();
	for ( int i=0; i<nVertexNum; ++i )
	{
		arDistance[i] = MIN_WEIGHT_NOT_CONNECTED;
		arParent[i] = -1;
		tNotVisitedList.push_back( i );
	}

	arDistance[nSourceIndex] = 0;

	while( !tNotVisitedList.empty() )
	{
		// 寻找最短路径。
		int nShortest = MIN_WEIGHT_NOT_CONNECTED;
		TIntList::iterator iterShortest;
		int nShortestIndex;
		for ( TIntList::iterator iter = tNotVisitedList.begin(); iter != tNotVisitedList.end(); ++iter )
		{
			if ( arDistance[ *iter ] < nShortest )
			{
				nShortest = arDistance[ *iter ];
				iterShortest = iter;
			}
		}

		if ( nShortest >= MIN_WEIGHT_NOT_CONNECTED )
		{
			cout << "Search over!" << endl;
			break;
		}
		else
		{
			nShortestIndex = *iterShortest;
			tNotVisitedList.erase( iterShortest );
		}

		for ( int i=0; i< (*pWeightTable)[nShortestIndex].size(); ++i )
		{
			int nIndex = (*pWeightTable)[ nShortestIndex ][i].first;
			int nWeight = (*pWeightTable)[ nShortestIndex][i].second;
			if ( arDistance[ nIndex ] > nWeight + nShortest )
			{
				arDistance[nIndex] = nWeight + nShortest;
				arParent[nIndex] = nShortestIndex;
			}
		}

	}

}

void DijkstraMain( ofstream& fout, ifstream& fin )
{
	int nPathNum;
	fin >> nPathNum;

	// 存放有牛的位置。
//	bool arHasCow[ MAX_PASTURE_NUM ] = {false};

	// 节点间的weight。
	u32 arWeightTable[MAX_PASTURE_NUM][MAX_PASTURE_NUM];
	for( int i=0; i<MAX_PASTURE_NUM; ++i )
	{
		for ( int k=0; k<MAX_PASTURE_NUM; ++k )
		{
			arWeightTable[i][k] = MIN_WEIGHT_NOT_CONNECTED;
		}
	}

	for ( int i=0; i<nPathNum; ++i )
	{
		unsigned char cPastureFirst, cPastureSecond;
		fin >> cPastureFirst >> cPastureSecond;
		u32 nWeight;
		fin >> nWeight;

// 		// 保存有牛的位置。
// 		bool bHasCow = isupper( cPastureFirst );
// 		if ( bHasCow )
// 		{
// 			cPastureFirst = 'z' + 1 + cPastureFirst - 'A';
// 		}
// 		arHasCow[ cPastureFirst - 'A' ] = bHasCow;
// 
// 		bHasCow = isupper( cPastureSecond ) ;		
// 		if ( bHasCow )
// 		{
// 			cPastureSecond = tolower( cPastureSecond );
// 		}
// 		arHasCow[ cPastureSecond - 'A' ] = bHasCow;
		if ( cPastureFirst <= 'Z' )
		{
			cPastureFirst = 'z' + 1 + cPastureFirst - 'A';
		}

		if ( cPastureSecond <= 'Z' )
		{
			cPastureSecond = 'z' + 1 + cPastureSecond - 'A';
		}
		

		if ( nWeight < arWeightTable[cPastureFirst - 'a'][cPastureSecond - 'a'] )
		{
			arWeightTable[cPastureFirst - 'a'][cPastureSecond - 'a'] = nWeight;
			arWeightTable[cPastureSecond - 'a'][cPastureFirst - 'a'] = nWeight;
		}		
	}

	// 节点间的weight。
	TVertexNeighbourWeightTable tWeightTable;
	tWeightTable.resize( MAX_PASTURE_NUM );
	for ( int i=0; i<MAX_PASTURE_NUM; ++i )
	{
		for ( int k=i+1; k<MAX_PASTURE_NUM; ++k )
		{
			if ( i != k && arWeightTable[i][k] < MIN_WEIGHT_NOT_CONNECTED )
			{
				// 
				tWeightTable[i].push_back( TIndexWeight( k, arWeightTable[i][k] ) );
				tWeightTable[k].push_back( TIndexWeight( i, arWeightTable[i][k] ) );
			}
		}
	}

	u32 arDistance[ MAX_PASTURE_NUM ];
	int arParent[MAX_PASTURE_NUM];
	Dijkstra( MAX_PASTURE_NUM-1, &tWeightTable, arDistance, arParent );


	u32 nShortestPath = MIN_WEIGHT_NOT_CONNECTED;
	int nNearestCowIndex = 0;
	for ( int i='z'-'a'+1; i<MAX_PASTURE_NUM-1; ++i )
	{
		if ( arDistance[i] < nShortestPath )
		{
			nShortestPath = arDistance[i];
			nNearestCowIndex = i;
		}
	}
	
	fout << (char)( nNearestCowIndex - 26 + 'A' ) << " " << nShortestPath << endl;
}

int main()
{
	string strProblemName = "comehome";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

#if 1
	DijkstraMain( fout, fin );
#else
	int nPathNum;
	fin >> nPathNum;

	// 存放有牛的位置。
	bool arHasCow[ MAX_PASTURE_NUM ] = {false};

	// 节点间的weight。
	u32 arWeightTable[MAX_PASTURE_NUM][MAX_PASTURE_NUM];
	for( int i=0; i<MAX_PASTURE_NUM; ++i )
	{
		for ( int k=0; k<MAX_PASTURE_NUM; ++k )
		{
			arWeightTable[i][k] = MIN_WEIGHT_NOT_CONNECTED;
		}
	}

	for ( int i=0; i<nPathNum; ++i )
	{
		char cPastureFirst, cPastureSecond;
		fin >> cPastureFirst >> cPastureSecond;
		u32 nWeight;
		fin >> nWeight;

		// 保存有牛的位置。
		if ( isupper( cPastureFirst ) )
		{
			cPastureFirst = tolower( cPastureFirst );
			arHasCow[ cPastureFirst - 'a' ] = true;
		}
		
		if ( isupper( cPastureSecond ) )
		{
			cPastureSecond = tolower( cPastureSecond );
			arHasCow[ cPastureSecond - 'a' ] = true;
		}

		if ( nWeight < arWeightTable[cPastureFirst - 'a'][cPastureSecond - 'a'] )
		{
			arWeightTable[cPastureFirst - 'a'][cPastureSecond - 'a'] = nWeight;
			arWeightTable[cPastureSecond - 'a'][cPastureFirst - 'a'] = nWeight;
		}		
	}

	// 节点间的distance.
	u32 arDistanceTable[MAX_PASTURE_NUM][MAX_PASTURE_NUM];
	FloydWarshall( (u32*)arWeightTable, (u32*)arDistanceTable, MAX_PASTURE_NUM, MAX_PASTURE_NUM );

	u32 nShortestPath = MIN_WEIGHT_NOT_CONNECTED;
	int nNearestCowIndex = 0;
	for ( int i=0; i<MAX_PASTURE_NUM-1; ++i )
	{
		if ( arHasCow[i] )
		{
			if ( arDistanceTable[25][i] < nShortestPath )
			{
				nShortestPath = arDistanceTable[25][i];
				nNearestCowIndex = i;
			}
		}
	}
	
	fout << (char)( nNearestCowIndex + 'A' ) << " " << nShortestPath << endl;
#endif

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
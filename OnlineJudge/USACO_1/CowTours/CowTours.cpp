/*
ID: thinkin6
PROG: cowtour
LANG: C++
*/

/** USA CO 2.4.4 :

Cow Tours
Farmer John has a number of pastures on his farm. Cow paths connect some pastures with certain other pastures, forming a field. But, at the present time, you can find at least two pastures that cannot be connected by any sequence of cow paths, thus partitioning Farmer John's farm into multiple fields.

Farmer John would like add a single a cow path between one pair of pastures using the constraints below.

A field's `diameter' is defined to be the largest distance of all the shortest walks between any pair of pastures in the field. Consider the field below with five pastures, located at the points shown, and cow paths marked by lines:

15,15   20,15
D       E
*-------*
|     _/|
|   _/  |
| _/    |
|/      |
*--------*-------*
A        B       C
10,10   15,10   20,10
The `diameter' of this field is approximately 12.07106, since the longest of the set of shortest paths between pairs of pastures is the path from A to E (which includes the point set {A,B,E}). No other pair of pastures in this field is farther apart when connected by an optimal sequence of cow paths.

Suppose another field on the same plane is connected by cow paths as follows:

      *F 30,15
	 / 
   _/  
 _/    
/      
*------ 
G      H
25,10   30,10
In the scenario of just two fields on his farm, Farmer John would add a cow path between a point in each of these two fields (namely point sets {A,B,C,D,E} and {F,G,H}) so that the joined set of pastures {A,B,C,D,E,F,G,H} has the smallest possible diameter.

Note that cow paths do not connect just because they cross each other; they only connect at listed points.

The input contains the pastures, their locations, and a symmetric "adjacency" matrix that tells whether pastures are connected by cow paths. Pastures are not considered to be connected to themselves. Here's one annotated adjacency list for the pasture {A,B,C,D,E,F,G,H} as shown above:

  A B C D E F G H
A 0 1 0 0 0 0 0 0
B 1 0 1 1 1 0 0 0
C 0 1 0 0 1 0 0 0
D 0 1 0 0 1 0 0 0
E 0 1 1 1 0 0 0 0
F 0 0 0 0 0 0 1 0
G 0 0 0 0 0 1 0 1
H 0 0 0 0 0 0 1 0
Other equivalent adjacency lists might permute the rows and columns by using some order other than alphabetical to show the point connections. The input data contains no names for the points.

The input will contain at least two pastures that are not connected by any sequence of cow paths.

Find a way to connect exactly two pastures in the input with a cow path so that the new combined field has the smallest possible diameter of any possible pair of connected pastures. Output that smallest possible diameter.

PROGRAM NAME: cowtour

INPUT FORMAT

Line 1:	 An integer, N (1 <= N <= 150), the number of pastures
Line 2-N+1:	 Two integers, X and Y (0 <= X ,Y<= 100000), that denote that X,Y grid location of the pastures; all input pastures are unique.
Line N+2-2*N+1:	 lines, each containing N digits (0 or 1) that represent the adjacency matrix as described above, where the rows' and columns' indices are in order of the points just listed.
SAMPLE INPUT (file cowtour.in)

8
10 10
15 10
20 10
15 15
20 15
30 15
25 10
30 10
01000000
10111000
01001000
01001000
01110000
00000010
00000101
00000010
OUTPUT FORMAT

The output consists of a single line with the diameter of the newly joined pastures. Print the answer to exactly six decimal places. Do not perform any special rounding on your output.

SAMPLE OUTPUT (file cowtour.out)

22.071068

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

const int MAX_PASTURE_NUM = 150;
const float MIN_WEIGHT_NOT_CONNECTED = 100000.0 * MAX_PASTURE_NUM * 2; 

typedef complex<float> TPasturePos;

/** Floyd-Warshall Algorithm.
*	float arWeightMatrix[] Weight矩阵，矩阵长宽都是nMatrixSize . 输入。
*	float arDistanceMatrix[] 距离矩阵，矩阵长宽都是nMatrixSize 。输出。
*	int nMatrixSize 矩阵大小
*	int nVertexNum 节点数目，矩阵中只有节点数目之间距离是有效的。
*
*	# dist(i,j) is "best" distance so far from vertex i to vertex j 

	# Start with all single edge paths.
	For i = 1 to n do
		For j = 1 to n do
			dist(i,j) = weight(i,j) 

	For k = 1 to n do	 # k is the `intermediate' vertex
		For i = 1 to n do
			For j = 1 to n do
				if (dist(i,k) + dist(k,j) < dist(i,j)) then	# shorter path?
				dist(i,j) = dist(i,k) + dist(k,j)
*/
void FloydWarshall( const float arWeightMatrix[], float arDistanceMatrix[], int nMatrixSize, int nVertexNum )
{
	for ( int i=0; i<nVertexNum; ++i )
	{
		for ( int k=0; k<nVertexNum; ++k )
		{
			arDistanceMatrix[i*nMatrixSize + k] = arWeightMatrix[i*nMatrixSize + k];
		}
	}

	for ( int i=0; i<nVertexNum; ++i )
	{
		for ( int j=0; j<nVertexNum; ++j )
		{
			for ( int k=0; k<nVertexNum; ++k )
			{
				if ( arDistanceMatrix[j*nMatrixSize+i] + arDistanceMatrix[i*nMatrixSize+k] 
				< arDistanceMatrix[ j*nMatrixSize + k ] )
				{
					arDistanceMatrix[ j*nMatrixSize + k ] = 
						arDistanceMatrix[j*nMatrixSize+i] + arDistanceMatrix[i*nMatrixSize+k];
				}
			}
		}
	}
}

int main()
{
	string strProblemName = "cowtour";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nPastureNum;
	fin >> nPastureNum;


	

	TPasturePos arPasturePos[ MAX_PASTURE_NUM ];
	for ( int i=0; i<nPastureNum; ++i )
	{
		int nX, nY;
		fin >> nX >> nY;
		arPasturePos[i] = TPasturePos( nX, nY );
	}

	float arWeightTable[MAX_PASTURE_NUM][MAX_PASTURE_NUM];

	float arDistanceTable[MAX_PASTURE_NUM][MAX_PASTURE_NUM];

	for ( int i=0; i<nPastureNum; ++i )
	{
		for ( int j=0; j<nPastureNum; ++j )
		{
			TPasturePos tWeight = arPasturePos[i] - arPasturePos[j];
			float fWeight = abs( tWeight );

			arWeightTable[i][j] = fWeight;
			arWeightTable[j][i] = fWeight;

			char cIn;
			fin >> cIn;
			cout << cIn;

			float fDistance;
			if ( '1' == cIn )
			{
				// 是连通的。
				fDistance = arWeightTable[i][j];
			}
			else if( i == j )
			{
				fDistance = 0.0;	// 同一个节点。
			}
			else
			{
				fDistance = MIN_WEIGHT_NOT_CONNECTED + 100;	// 不直接相连。
			}
			arDistanceTable[i][j] = fDistance;
			arDistanceTable[j][i] = fDistance;
		}
	}

	// Floyd - Warshall
	FloydWarshall( (float*)arDistanceTable, (float*)arDistanceTable, MAX_PASTURE_NUM, nPastureNum );

	// 求出每个节点的最大最短距离。
	float arLongestPath[ MAX_PASTURE_NUM ];
	for ( int i=0; i<nPastureNum; ++i )
	{
		arLongestPath[i] = 0.0;
		for ( int k=0; k<nPastureNum; ++k )
		{
			float fDist = arDistanceTable[ i ][k];
			if ( fDist < MIN_WEIGHT_NOT_CONNECTED && fDist > arLongestPath[i] )
			{
				arLongestPath[i] = fDist;
			}
		}
	}
	// 遍历所有的节点组合，寻找在所有不连通的两个节点组合中，两节点的最长最短距离加上两节点间的直线距离的和最小的那个，
	// 就是题目要求的那个。
	float fShortestPath = MIN_WEIGHT_NOT_CONNECTED;
	for( int i=0; i<nPastureNum; ++i )
	{
		for ( int k=0; k<nPastureNum; ++k )
		{
			if ( arDistanceTable[i][k] > MIN_WEIGHT_NOT_CONNECTED	// 不连通。
				&& arLongestPath[i] + arLongestPath[k] + arWeightTable[ i ][k] < fShortestPath )
			{
				fShortestPath = arLongestPath[i] + arLongestPath[k] + arWeightTable[i][k];
			}
		}
	}

	//fout << setw( 6 ) << fShortestPath << endl;
	char szRet[ 100 ];
	sprintf( szRet, "%f", fShortestPath );
	fout << szRet << endl;

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
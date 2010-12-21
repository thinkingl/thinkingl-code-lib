/*
ID: thinkin6
PROG: fence6
LANG: C++
*/

/** USA CO 4.1.3 :
Fence Loops
The fences that surround Farmer Brown's collection of pastures have gotten out of control. They are made up of straight segments from 1 through 200 feet long that join together only at their endpoints though sometimes more than two fences join together at a given endpoint. The result is a web of fences enclosing his pastures. Farmer Brown wants to start to straighten things out. In particular, he wants to know which of the pastures has the smallest perimeter.

Farmer Brown has numbered his fence segments from 1 to N (N = the total number of segments). He knows the following about each fence segment:

the length of the segment
the segments which connect to it at one end
the segments which connect to it at the other end.
Happily, no fence connects to itself.
Given a list of fence segments that represents a set of surrounded pastures, write a program to compute the smallest perimeter of any pasture. As an example, consider a pasture arrangement, with fences numbered 1 to 10 that looks like this one (the numbers are fence ID numbers):

1
+---------------+
|\             /|
2| \7          / |
|  \         /  |
+---+       /   |6
| 8  \     /10  |
3|     \9  /     |
|      \ /      |
+-------+-------+
4       5
The pasture with the smallest perimeter is the one that is enclosed by fence segments 2, 7, and 8.

PROGRAM NAME: fence6

INPUT FORMAT

Line 1:	 N (1 <= N <= 100)
Line 2..3*N+1:	
N sets of three line records:

The first line of each record contains four integers: s, the segment number (1 <= s <= N); Ls, the length of the segment (1 <= Ls <= 255); N1s (1 <= N1s <= 8) the number of items on the subsequent line; and N2sthe number of items on the line after that (1 <= N2s <= 8).
The second line of the record contains N1 integers, each representing a connected line segment on one end of the fence.
The third line of the record contains N2 integers, each representing a connected line segment on the other end of the fence.
SAMPLE INPUT (file fence6.in)

10
1 16 2 2
2 7
10 6
2 3 2 2
1 7
8 3
3 3 2 1
8 2
4
4 8 1 3
3
9 10 5
5 8 3 1
9 10 4
6
6 6 1 2 
5 
1 10
7 5 2 2 
1 2
8 9
8 4 2 2
2 3
7 9
9 5 2 3
7 8
4 5 10
10 10 2 3
1 6
4 9 5
OUTPUT FORMAT

The output file should contain a single line with a single integer that represents the shortest surrounded perimeter.
SAMPLE OUTPUT (file fence6.out)

12
 
*/

/** 
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

const int MaxSegmentLength = 255;
const int MaxSegmentNum = 100;
const int InvalidEndPointIndex = -1;
const int DistanceInfinity = MaxSegmentLength * MaxSegmentNum + 99999;	// ��ʾ����ͨ�ľ���

typedef std::set< int > TSegmentIndexSet;	// �߶���ż���.

typedef std::vector< int > TDistanceList;	// �˵���������
typedef std::vector< TDistanceList > TDistanceTable; // �˵������

// �߶ζ˵����Ϣ.
struct TEndPointInfo
{
	int m_nEndPointIndex;	// �˵����.
	TSegmentIndexSet m_tJoinedSegment;	// ���ӵ��߶�.

	TEndPointInfo()
	{
		m_nEndPointIndex = InvalidEndPointIndex;
	}
};

/** Djkstra �����.
*	
*/
int Djkstra( const TDistanceTable& tWeightTable, int src, int dst, int cutMaxDistance )
{
	//# distance(j) is distance from source vertex to vertex j
	//# parent(j) is the vertex that precedes vertex j in any shortest path
	//#                  (to reconstruct the path subsequently) 

	//1 For all nodes i
	//2     distance(i) = infinity         	# not reachable yet
	//3     visited(i) = False
	//4     parent(i) = nil	 # no path to vertex yet 
	typedef std::vector< bool > TVisitedTable;
	TVisitedTable tVisited( tWeightTable.size(), false );
	TDistanceList tDistance( tWeightTable.size(), DistanceInfinity );

	//5 distance(source) = 0	 # source -> source is start of all paths
	//6 parent(source) = nil

	tDistance[ src ] = 0;

	//7   8 while (nodesvisited < graphsize)
	//9     find unvisited vertex with min distance to source; call it vertex i
	//10     assert (distance(i) != infinity, "Graph is not connected") 

	//11     visited(i) = True	 # mark vertex i as visited 

	//# update distances of neighbors of i
	//12     For all neighbors j of vertex i
	//13         if distance(i) + weight(i,j) < distance(j) then
	//14             distance(j) = distance(i) + weight(i,j)
	//15             parent(j) = i
	while( 1 )
	{
		int minDistance = DistanceInfinity;
		int nearestVertex = -1;
		for ( int i=0; i<tDistance.size(); ++i )
		{
			if ( !tVisited[i] && tDistance[i] < minDistance )
			{
				minDistance = tDistance[i];
				nearestVertex = i;
			}
		}

		if ( minDistance == DistanceInfinity )
		{
			cout << "Graph is not connected!" << endl;
			return DistanceInfinity;
		}

		tVisited[ nearestVertex ] = true;

		// �ж��Ƿ񵽴���dst
		if ( nearestVertex == dst )
		{
			return minDistance;
		}
		// �ж��Ƿ�ü�֦��.
		if ( nearestVertex > cutMaxDistance )
		{
			return DistanceInfinity;
		}

		const TDistanceList& tNearestVertexNeighborWeight = tWeightTable[nearestVertex];
		for ( int i=0; i<tNearestVertexNeighborWeight.size(); ++i )
		{
			if ( tNearestVertexNeighborWeight[i] < DistanceInfinity )
			{
				if ( tDistance[ nearestVertex ] + tNearestVertexNeighborWeight[i] < tDistance[ i ] )
				{
					tDistance[ i ] = tDistance[ nearestVertex ] + tNearestVertexNeighborWeight[i];
				}
			}
		}
	}

	return 0;
}

int main()
{
	string strProblemName = "fence6";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nSegmentNum;
	fin >> nSegmentNum;

	// ��Ϊ��Ŀ��ÿ���˵㶼��2�����ϵ��߶�(���)����,���Զ˵���Ŀ���ֻ�����߶���.
	int nMaxPointNum = nSegmentNum;



	
	// ������ʼ��
	TDistanceTable tDistanceTable( nMaxPointNum, TDistanceList( nMaxPointNum, DistanceInfinity ) );

	typedef std::pair< TEndPointInfo, TEndPointInfo > TSegmentConnectState;	// �߶����˵��������.
	typedef std::vector< TSegmentConnectState > TSegmentConnectStateList;
	TSegmentConnectStateList tSegmentConnectStateList( nSegmentNum+1 );		// �߶������˵������������.

	int nCurEndPointIndex = 0;

	for ( int i=0; i<nSegmentNum; ++i )
	{
		int nSegmentIndex, nSegmentLen, nItemNum1, nItemNum2;
		fin >> nSegmentIndex >> nSegmentLen >> nItemNum1 >> nItemNum2;

		for ( int k=0; k<nItemNum1; ++k )
		{
			int nSegConnect;
			fin >> nSegConnect;
			
			TEndPointInfo& endPointInfo = tSegmentConnectStateList[ nSegmentIndex ].first;
			endPointInfo.m_tJoinedSegment.insert( nSegConnect );

			// �������֮ǰ�Ƿ���Ѿ�����.
			if ( tSegmentConnectStateList[ nSegConnect ].first.m_tJoinedSegment.find( nSegmentIndex )
				!= tSegmentConnectStateList[ nSegConnect ].first.m_tJoinedSegment.end() )
			{
				// �������.
				endPointInfo.m_nEndPointIndex 
					= tSegmentConnectStateList[ nSegConnect ].first.m_nEndPointIndex;
			}
			else if( tSegmentConnectStateList[ nSegConnect ].second.m_tJoinedSegment.find( nSegmentIndex )
				!= tSegmentConnectStateList[ nSegConnect ].second.m_tJoinedSegment.end() )
			{
				// �������.
				endPointInfo.m_nEndPointIndex 
					= tSegmentConnectStateList[ nSegConnect ].second.m_nEndPointIndex;
			}
			
		}

		if ( InvalidEndPointIndex == tSegmentConnectStateList[ nSegmentIndex ].first.m_nEndPointIndex )
		{
			tSegmentConnectStateList[ nSegmentIndex ].first.m_nEndPointIndex = nCurEndPointIndex;
			nCurEndPointIndex++;
		}

		for ( int k=0; k<nItemNum2; ++k )
		{
			int nSegConnect;
			fin >> nSegConnect;

			TEndPointInfo& endPointInfo = tSegmentConnectStateList[ nSegmentIndex ].second;
			endPointInfo.m_tJoinedSegment.insert( nSegConnect );

			// �������֮ǰӦ�þ��Ѿ�����.
			if ( tSegmentConnectStateList[ nSegConnect ].first.m_tJoinedSegment.find( nSegmentIndex )
				!= tSegmentConnectStateList[ nSegConnect ].first.m_tJoinedSegment.end() )
			{
				// �������.
				endPointInfo.m_nEndPointIndex 
					= tSegmentConnectStateList[ nSegConnect ].first.m_nEndPointIndex;
			}
			else if( tSegmentConnectStateList[ nSegConnect ].second.m_tJoinedSegment.find( nSegmentIndex )
				!= tSegmentConnectStateList[ nSegConnect ].second.m_tJoinedSegment.end() )
			{
				// �������.
				endPointInfo.m_nEndPointIndex 
					= tSegmentConnectStateList[ nSegConnect ].second.m_nEndPointIndex;
			}
				
		}

		if ( InvalidEndPointIndex == tSegmentConnectStateList[ nSegmentIndex ].second.m_nEndPointIndex )
		{
			tSegmentConnectStateList[ nSegmentIndex ].second.m_nEndPointIndex = nCurEndPointIndex;
			nCurEndPointIndex++;
		}

		// �ѵ�ǰ�߶εĳ��ȸ��½��˵�����.
		int p1 = tSegmentConnectStateList[ nSegmentIndex ].first.m_nEndPointIndex;
		int p2 = tSegmentConnectStateList[ nSegmentIndex ].second.m_nEndPointIndex;
		tDistanceTable[ p1 ][ p2 ] = nSegmentLen;
		tDistanceTable[ p2 ][ p1 ] = nSegmentLen;
	}

	int nMinPerimeter = DistanceInfinity;
	for ( int i=0; i<nMaxPointNum; ++i )
	{
		for ( int k=i+1; k<nMaxPointNum; ++k )
		{
			if ( tDistanceTable[ i ][ k ] < DistanceInfinity )
			{
				// ˵���� i �� k �����߶����ӵ�.
				// ȥ������߶κ�,i��k����̾������ ����i��k֮����߶γ���,�����߶�ik���ڵĶ���ε���С�ܳ�.
				int ikLen = tDistanceTable[i][k];
				tDistanceTable[i][k] = DistanceInfinity;

				int nCurPerimeter = ikLen + Djkstra( tDistanceTable, i, k, nMinPerimeter );

				nMinPerimeter = min( nCurPerimeter, nMinPerimeter );

				tDistanceTable[i][k] = ikLen;
			}
		}
	}

	fout << nMinPerimeter << endl;



#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}
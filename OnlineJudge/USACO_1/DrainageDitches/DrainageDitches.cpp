/*
ID: thinkin6
PROG: ditch
LANG: C++
*/

/** USA CO 4.2.1 :
Drainage Ditches
Hal Burch
Every time it rains on Farmer John's fields, a pond forms over Bessie's favorite clover patch. This means that the clover is covered by water for awhile and takes quite a long time to regrow. Thus, Farmer John has built a set of drainage ditches so that Bessie's clover patch is never covered in water. Instead, the water is drained to a nearby stream. Being an ace engineer, Farmer John has also installed regulators at the beginning of each ditch, so he can control at what rate water flows into that ditch.

Farmer John knows not only how many gallons of water each ditch can transport per minute but also the exact layout of the ditches, which feed out of the pond and into each other and stream in a potentially complex network. Note however, that there can be more than one ditch between two intersections.

Given all this information, determine the maximum rate at which water can be transported out of the pond and into the stream. For any given ditch, water flows in only one direction, but there might be a way that water can flow in a circle.

PROGRAM NAME: ditch

INPUT FORMAT

Line 1:	 Two space-separated integers, N (0 <= N <= 200) and M (2 <= M <= 200). N is the number of ditches that Farmer John has dug. M is the number of intersections points for those ditches. Intersection 1 is the pond. Intersection point M is the stream.
Line 2..N+1:	 Each of N lines contains three integers, Si, Ei, and Ci. Si and Ei (1 <= Si, Ei <= M) designate the intersections between which this ditch flows. Water will flow through this ditch from Si to Ei. Ci (0 <= Ci <= 10,000,000) is the maximum rate at which water will flow through the ditch.
SAMPLE INPUT (file ditch.in)

5 4
1 2 40
1 4 20
2 4 20
2 3 30
3 4 10
OUTPUT FORMAT

One line with a single integer, the maximum rate at which water may emptied from the pond.

SAMPLE OUTPUT (file ditch.out)

50
*/

/** 
*	Network Flow 问题.
*	需要注意的是,题目数据中有两个点之间有多条水管的情况,所以输入的时候用的是 += .
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

typedef std::vector< u32 > TFlowRateList;
typedef std::vector< TFlowRateList > TFlowRateTable;

u32 GetMaxNetwordFlow( const TFlowRateTable& flowRateTable, int source, int sink )
{
//	1   if (source = sink)
//		2     totalflow = Infinity
//		3     DONE 
	if ( source == sink )
	{
		return -1;
	}
//		4   totalflow = 0 

	u32 totalFlow = 0;
	TFlowRateTable capacityTable = flowRateTable;
//
//		5   while (True)
//		6 # find path with highest capacity from source to sink uses a modified djikstra's algorithm
//		8     for all nodes i
//		9       prevnode(i) = nil
//		10       flow(i) = 0
//		11       visited(i) = False
//		12     flow(source) = infinity 
//
//		13     while (True)
//		14       maxflow = 0
//		15       maxloc = nil 
//		16       # find the unvisited node with the highest capacity to it
//		17       for all nodes i
//		18         if (flow(i) > maxflow AND not visited(i))
//		19           maxflow = flow(i)
//		20           maxloc = i
//		21       if (maxloc = nil)
//		22         break inner while loop
//		23       if (maxloc = sink)
//		24         break inner while loop
//		24a      visited(maxloc) = true
//		25       # update its neighbors
//		26       for all neighbors i of maxloc
//		27         if (flow(i) < min(maxflow, capacity(maxloc,i)))
//		28           prevnode(i) = maxloc
//		29           flow(i) = min(maxflow,	capacity(maxloc,i)) 
//
//		30     if (maxloc = nil)         # no path break outer while loop 
//
//		32     pathcapacity = flow(sink)
//		33     totalflow = totalflow + pathcapacity 
//
	// find path with highest capacity from source to sink uses a modified djikstra's algorithm
	int nPointNum = flowRateTable.size();
	typedef std::vector< bool > TVisitedList;
	typedef std::vector< int > TNodeIndexList;
	const u32 INFINITE = -100;
	while ( true )
	{
		TFlowRateList tFlow( nPointNum, 0 );
		TVisitedList tVisit( nPointNum, false );
		TNodeIndexList tPreNode( nPointNum, -1 );

		tFlow[ source ] = INFINITE;

		int maxLoc = -1;
		while( true )
		{
			// find the unvisited node with the highest capacity to it
			u32 maxFlow = 0;
			maxLoc = -1;
			for ( int i=0; i<nPointNum; ++i )
			{
				if ( !tVisit[i] && tFlow[i] > maxFlow )
				{
					maxFlow = tFlow[i];
					maxLoc = i;
				}
			}
			if ( maxLoc == -1 || maxLoc == sink )
			{
				break;
			}

			tVisit[ maxLoc ] = true;

			// update its neighbors
			for ( int i=0; i<nPointNum; ++i )
			{
				u32 newFlow = min ( maxFlow, capacityTable[maxLoc][i] );
				if ( tFlow[ i ] < newFlow )
				{
					tFlow[i] = newFlow;
					tPreNode[i] = maxLoc;
				}
			}
		}

		if ( maxLoc == -1 )
		{
			break;	// no path break outer while loop 
		}

		u32 pathCapacity = tFlow[ sink ];
		totalFlow += pathCapacity;

		//# add that flow to the network, update capacity appropriately
		//		35     curnode = sink
		//# for each arc, prevnode(curnode), curnode on path:
		//		36     while (curnode != source)       
		//		38       nextnode = prevnode(curnode)
		//		39       capacity(nextnode,curnode) =capacity(nextnode,curnode) - pathcapacity
		//				capacity(curnode,nextnode) = capacity(curnode,nextnode) + pathcapacity
		//		43       curnode = nextnode 

		// add that flow to the network, update capacity appropriately
		int curNode = sink;
		while ( curNode != source )
		{
			int nextNode = tPreNode[ curNode ];
			capacityTable[ nextNode ][ curNode ] -= pathCapacity;
			capacityTable[ curNode ][ nextNode ] += pathCapacity;
			curNode = nextNode;
		}
	}

	return totalFlow;


}

int main()
{
	string strProblemName = "ditch";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nDitchNum, nPointNum;
	fin >> nDitchNum >> nPointNum;



	TFlowRateTable tWaterNet( nPointNum, TFlowRateList( nPointNum, 0 ) );
	for ( int i=0; i<nDitchNum; ++i )
	{
		int src, dst, rate;
		fin >> src >> dst >> rate;
		tWaterNet[ src-1 ][ dst-1 ] += rate;
	}

	u32 maxFlow = GetMaxNetwordFlow( tWaterNet, 0, nPointNum-1 );

	fout << maxFlow << endl;

#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}
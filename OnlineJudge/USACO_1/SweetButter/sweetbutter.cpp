/*
ID: thinkin6
PROG: butter
LANG: C++
*/

/** USA CO 3.2.6 :
Sweet Butter

Greg Galperin -- 2001
Farmer John has discovered the secret to making the sweetest butter in all of Wisconsin: sugar. By placing a sugar cube out in the pastures, he knows the N (1 <= N <= 500) cows will lick it and thus will produce super-sweet butter which can be marketed at better prices. Of course, he spends the extra money on luxuries for the cows.

FJ is a sly farmer. Like Pavlov of old, he knows he can train the cows to go to a certain pasture when they hear a bell. He intends to put the sugar there and then ring the bell in the middle of the afternoon so that the evening's milking produces perfect milk.

FJ knows each cow spends her time in a given pasture (not necessarily alone). Given the pasture location of the cows and a description of the paths the connect the pastures, find the pasture in which to place the sugar cube so that the total distance walked by the cows when FJ rings the bell is minimized. FJ knows the fields are connected well enough that some solution is always possible.

PROGRAM NAME: butter

INPUT FORMAT

Line 1: Three space-separated integers: N, the number of pastures: P (2 <= P <= 800), and the number of connecting paths: C (1 <= C <= 1,450). Cows are uniquely numbered 1..N. Pastures are uniquely numbered 1..P.
Lines 2..N+1: Each line contains a single integer that is the pasture number in which a cow is grazing. Cow i's pasture is listed on line i+1.
Lines N+2..N+C+1: Each line contains three space-separated integers that describe a single path that connects a pair of pastures and its length. Paths may be traversed in either direction. No pair of pastures is directly connected by more than one path. The first two integers are in the range 1..P; the third integer is in the range (1..225).
SAMPLE INPUT (file butter.in)

3 4 5
2
3
4
1 2 1
1 3 5
2 3 7
2 4 3
3 4 5

INPUT DETAILS

This diagram shows the connections geometrically:
P2  
P1 @--1--@ C1
\    |\
\   | \
5  7  3
\ |   \
\|    \ C3
C2 @--5--@
P3    P4
OUTPUT FORMAT

Line 1: A single integer that is the minimum distance the cows must walk to a pasture with a sugar cube.
SAMPLE OUTPUT (file butter.out)

8

OUTPUT DETAILS:

Putting the cube in pasture 4 means: cow 1 walks 3 units; cow 2 walks 5
units; cow 3 walks 0 units -- a total of 8.

*/

/** 
思路：
*	1. 图论问题.用Floyd-Warshall算法求出所有节点之间的距离,然后寻找到所有牛的距离的和最小的那个节点.
*	最多有800个节点,int数组会超出1M,用堆内存.
*	1. 会超时...改用Dijkstra算法,求有奶牛的pature到其它点的距离..
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

typedef std::vector<int> TIntVec;
typedef std::vector< TIntVec > TIntVecVec;

class CMatrix
{
public:
	/** 构造. */
	CMatrix( int nMatrixSize );
	~CMatrix();
	CMatrix( const CMatrix& another );
	CMatrix& operator = ( const CMatrix& another );

	/** 取行的地址. */
	TIntVec& operator[]( int nIndex );

	/** 矩阵大小. */
	int size() const;
private:
	void Release();
private:
	// 难以调试.
	//int *m_arMatrix;
	//int m_nMatrixSize;
	
	TIntVecVec m_tMatrix;
};

CMatrix::CMatrix( int nMatrixSize )
{
	this->m_tMatrix = TIntVecVec( nMatrixSize, TIntVec( nMatrixSize,0 ) );
}

CMatrix::~CMatrix()
{
	this->Release();
}

void CMatrix::Release()
{
	
}

CMatrix::CMatrix( const CMatrix& another )
{	
	*this = another;
}

CMatrix& CMatrix::operator =( const CMatrix& another )
{
	if ( this == &another )
	{
		return *this;
	}
	else
	{
		this->Release();
		this->m_tMatrix = another.m_tMatrix;
		return *this;
	}
}

TIntVec& CMatrix::operator [](int nIndex)
{
	return m_tMatrix[nIndex];
}

int CMatrix::size()const
{
	return this->m_tMatrix.size();
}


/** 表示不连通的距离. */
const u32 DISTANCE_NOT_CONNECT = 10000000;

/** 求出所有节点之间的距离. 
*	Floyd-Warshall算法.时间复杂度O(n的3次幂).
*	
*	const CMatrix& weightMatrix (IN) 节点之间的weight.
*	CMatrix& distanceMatrix		(OUT) 节点之间的Distance.
*/
void FloydWarshall( const CMatrix& weightMatrix, CMatrix& distanceMatrix, CMatrix& parentMatrix )
{
	// 距离矩阵初始化.
	distanceMatrix = weightMatrix;

	for ( int i=0; i<parentMatrix.size(); ++i )
	{
		parentMatrix[i] = TIntVec( parentMatrix.size(), -1 );
	}

	for ( int i=0; i<weightMatrix.size(); ++i ) // i作为中间节点.
	{
		for ( int j=0; j<weightMatrix.size();++j )
		{
			for ( int k=0; k<weightMatrix.size(); ++k )
			{
				// 通过中间节点连接更短.
				u32 nNewDistance = distanceMatrix[j][i] + distanceMatrix[i][k];
				if ( nNewDistance < distanceMatrix[j][k] )
				{
					distanceMatrix[j][k] = nNewDistance;
					distanceMatrix[k][j] = nNewDistance;

					parentMatrix[j][k] = i;
					parentMatrix[k][j] = i;
				}
			}
		}
	}
}

/** SPFA算法,算是对Bellman-ford算法的优化.
*	全名: Shortest Path Faster Algorithm.
*/
void SPFAAll( CMatrix& weightMatrix, CMatrix& distanceMatrix, CMatrix& parentMatrix )
{
	// 转换为相邻点的表示法.
	TIntVecVec arConnectedNode( weightMatrix.size() );
	for ( int i=0; i<weightMatrix.size(); ++i )
	{
		for ( int k=0; k<weightMatrix.size(); ++k )
		{
			if ( weightMatrix[i][k] < DISTANCE_NOT_CONNECT && i != k )
			{
				arConnectedNode[i].push_back( k );
			}
		}
	}

	

	// 初始化距离.
	distanceMatrix = weightMatrix;
	for ( int i=0; i<distanceMatrix.size(); ++i )
	{
		for ( int k=0; k<distanceMatrix.size(); ++k )
		{
//			if ( i != k )
			{
				distanceMatrix[i][k] = DISTANCE_NOT_CONNECT;
				distanceMatrix[k][i] = DISTANCE_NOT_CONNECT;
			}
		}
	}

	parentMatrix = distanceMatrix;


	for ( int i=0; i<weightMatrix.size(); ++i )
	{
		// 保存需要 relax 的节点.
		typedef std::queue<int> TIntQueue;
		TIntQueue tVetexNeedRelax;
		// 节点是否在队列中.
		const int MAX_NODE_NUM = 2000;
		bool arInQueue[ MAX_NODE_NUM ] = { false };

		distanceMatrix[i][i] = 0;
		tVetexNeedRelax.push( i );

		while ( !tVetexNeedRelax.empty() )
		{
			int nNode = tVetexNeedRelax.front();
			tVetexNeedRelax.pop();
			arInQueue[ nNode ] = false;

			for ( int k=0; k<arConnectedNode[ nNode ].size(); ++k )
			{
				int nRelaxNode = arConnectedNode[nNode][k];
				int nTmpDistance = distanceMatrix[i][nRelaxNode];

				int nWeight = weightMatrix[ nRelaxNode ][ nNode ];
				int nDis = distanceMatrix[ i ][ nNode ] ;
				int nDis2 = distanceMatrix[ i ][ nRelaxNode ];

				int nNewDistance = weightMatrix[ nRelaxNode ][ nNode ] + distanceMatrix[ i ][ nNode ];

				if ( nNewDistance < distanceMatrix[ i ][ nRelaxNode ] )
				{
//					distanceMatrix[ nRelaxNode ][ i ] = nNewDistance;
					distanceMatrix[ i ][ nRelaxNode ] = nNewDistance;
					if ( !arInQueue[ nRelaxNode ] )
					{
						tVetexNeedRelax.push( nRelaxNode );
						arInQueue[ nRelaxNode ] = true;
					}

					// 父亲.
					parentMatrix[i][nRelaxNode] = nNode;
//					parentMatrix[nRelaxNode][i] = nNode;
				}
			}
			

		}
	}

}

/** 优化的Djkstra算法. 
*	分别求各点到其余各点的距离.
*	const CMatrix& weightMatrix (IN) weight.
*	CMatrix& distanceMatrix (OUT) distance.
*	失败,效率很低.
*/
void DjkstraAllFast( CMatrix& weightMatrix, CMatrix& distanceMatrix )
{
	// 转换为相邻点的表示法.
	const int MAX_NODE_NUM = 800;
	typedef std::vector<int> TIntVector;
	TIntVector arConnectedNode[ MAX_NODE_NUM ];
	for ( int i=0; i<weightMatrix.size(); ++i )
	{
		for ( int k=0; k<weightMatrix.size(); ++k )
		{
			if ( weightMatrix[i][k] < DISTANCE_NOT_CONNECT )
			{
				arConnectedNode[i].push_back( k );
			}
		}
	}

	// 初始化距离.
	distanceMatrix = weightMatrix;

	// 对每一个点做Djkstra算法.
	for ( int i=0; i<weightMatrix.size(); ++i )
	{
		// 图外的点的距离树.
		typedef std::set<u32> TNodeSet;
		typedef std::map<u32, TNodeSet> TDistanceNodeMap;
		TDistanceNodeMap tDistanceNode;
		// 目标节点放入树中.
		tDistanceNode[0].insert( i );

		// 是否已经访问过的标志.
		bool arVisited[ MAX_NODE_NUM ] = { false };

		// 不断的寻找离i点最近的点.
		while( !tDistanceNode.empty() )
		{
			// 不在图上而距离图最近的点.
			int nNode = *( tDistanceNode.begin()->second.begin() );
			
			// 删除这个点.
			tDistanceNode.begin()->second.erase( tDistanceNode.begin()->second.begin() );
			if ( tDistanceNode.begin()->second.empty() )
			{
				tDistanceNode.erase( tDistanceNode.begin() );
			}
			
			// 已经访问.
			arVisited[ nNode ] = true;

			// 更新这个点的邻居的距离.
			for ( int w=0; w<arConnectedNode[nNode].size(); ++w )
			{
				u32 neighbour = arConnectedNode[nNode][w];
				u32 newDistance = distanceMatrix[nNode][i] + weightMatrix[nNode][neighbour];
				u32 oldDistance = distanceMatrix[i][neighbour];
				if ( newDistance < distanceMatrix[i][neighbour] )
				{
					distanceMatrix[i][neighbour] = newDistance;
					distanceMatrix[neighbour][i] = newDistance;
				}

				// 更新表中的距离.
				if ( !arVisited[ neighbour ] )
				{
					tDistanceNode[ newDistance ].insert( neighbour );

					if ( newDistance != oldDistance )
					{
						if ( tDistanceNode.find( oldDistance ) != tDistanceNode.end() )
						{
							tDistanceNode[ oldDistance ].erase( neighbour );
							if ( tDistanceNode[oldDistance].empty() )
							{
								tDistanceNode.erase( oldDistance );
							}
						}
					}
				}
			}

		}

		// 标记已经访问.

		// 更新这一点连接的点的距离.


	}
}

int main()
{
	string strProblemName = "butter";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nCowNum, nPastureNum, nPathNum;
	fin >> nCowNum >> nPastureNum >> nPathNum;

	// 哪几个Pasture有牛.
	const int MAX_COW_NUM = 500;
	int arCowPasture[ MAX_COW_NUM ];
	for ( int i=0; i<nCowNum; ++i )
	{
		fin >> arCowPasture[i];
		arCowPasture[i] --;
	}

	// Pasture之间的Path.
	CMatrix tPastureDistance( nPastureNum );
	for ( int i=0; i<nPastureNum; ++i )
	{
		for ( int k=0; k<nPastureNum; ++k )
		{
			int nWeight = DISTANCE_NOT_CONNECT;
			if( i==k )
			{
				nWeight = 0;
			}
			tPastureDistance[i][k] = nWeight;
			tPastureDistance[k][i] = nWeight;
		}
	}

	for ( int i=0; i<nPathNum; ++i )
	{
		int nPasture1, nPasture2, nWeight;
		fin >> nPasture1 >> nPasture2 >> nWeight;
		tPastureDistance[ nPasture1-1 ][ nPasture2-1 ] = nWeight;
		tPastureDistance[ nPasture2-1 ][ nPasture1-1 ] = nWeight;
	}

	// 获取最短距离.
#if 0
	FloydWarshall( tPastureDistance, tPastureDistance );
#elif 0
	CMatrix weightMatrix = tPastureDistance;
	DjkstraAllFast( weightMatrix, tPastureDistance );
#else
	CMatrix weightMatrix = tPastureDistance;
	CMatrix parentMatrix( weightMatrix.size() );
	SPFAAll( weightMatrix, tPastureDistance, parentMatrix );
#endif

#ifdef THINKINGLddd
	CMatrix distanceCheck = weightMatrix;
	CMatrix parentFloydMatrix( distanceCheck.size() );
	FloydWarshall( weightMatrix, distanceCheck, parentFloydMatrix );
	for ( int i=0; i<distanceCheck.size(); ++i )
	{
		for ( int k=0; k<distanceCheck.size(); ++k )
		{
			if( distanceCheck[i][k] != tPastureDistance[i][k] )
			{
				cout << "Error!!" << endl;
			}
		}
	}
#endif

	// 遍历,看哪个点最短.
	int nShortestSum = DISTANCE_NOT_CONNECT;
	for ( int i=0; i<nPastureNum; ++i )
	{
		int nSum = 0;
		for ( int k=0; k<nCowNum; ++k )
		{
			int nCowPos = arCowPasture[k];
			if ( tPastureDistance[i][nCowPos] < DISTANCE_NOT_CONNECT )
			{
				nSum += tPastureDistance[i][nCowPos];
			}
			else
			{
				nSum = DISTANCE_NOT_CONNECT;	// 不连通.这个题目里不应该有这种情况.
				break;
			}
		}

		if ( nSum < nShortestSum )
		{
			nShortestSum = nSum;
		}
	}
	
	fout << nShortestSum << endl;

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
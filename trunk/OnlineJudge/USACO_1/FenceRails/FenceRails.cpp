/*
ID: thinkin6
PROG: fence8
LANG: C++
*/

/** USA CO 4.1.2 :
Fence Rails
Burch, Kolstad, and Schrijvers
Farmer John is trying to erect a fence around part of his field. He has decided on the shape of the fence and has even already installed the posts, but he's having a problem with the rails. The local lumber store has dropped off boards of varying lengths; Farmer John must create as many of the rails he needs from the supplied boards.

Of course, Farmer John can cut the boards, so a 9 foot board can be cut into a 5 foot rail and a 4 foot rail (or three 3 foot rails, etc.). Farmer John has an `ideal saw', so ignore the `kerf' (distance lost during sawing); presume that perfect cuts can be made.

The lengths required for the rails might or might not include duplicates (e.g., a three foot rail and also another three foot rail might both be required). There is no need to manufacture more rails (or more of any kind of rail) than called for the list of required rails.

PROGRAM NAME: fence8

INPUT FORMAT

Line 1:	 N (1 <= N <= 50), the number of boards
Line 2..N+1:	 N lines, each containing a single integer that represents the length of one supplied board
Line N+2:	 R (1 <= R <= 1023), the number of rails
Line N+3..N+R+1:	 R lines, each containing a single integer (1 <= ri <= 128) that represents the length of a single required fence rail
SAMPLE INPUT (file fence8.in)

4
30
40
50
25
10
15
16
17
18
19
20
21
25
24
30
OUTPUT FORMAT

A single integer on a line that is the total number of fence rails that can be cut from the supplied boards. Of course, it might not be possible to cut all the possible rails from the given boards.
SAMPLE OUTPUT (file fence8.out)

7
 
*/

/** 采用迭代加深的深度优先搜索算法。
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

/** 长度列表。 */
typedef std::vector< int > TLengthList;

/** 深度限制搜索。 
*	nLenthLeft : 剩余深度。
*	TLengthList& boardList : 当前剩余的木板长度列表
*	TLengthList& railList : 要求的篱笆长度列表
*	int nRailCursor : 当前处理的rail序号
*/
bool SearchDeepthLimited( int nDeepthLeft, TLengthList& boardList, 
	TLengthList& railList, int nRailCursor,
	int lastBoardIndex, int lastRailLen,
	int nMaxWastedBoard, int nCurWastedBoard )
{
	// 深度用尽，返回。
	if ( nDeepthLeft == 0  )
	{
		return true;
	}


	// 在当前深度做一次搜索，在每个情况下进行递归。
	// 广度上，为每个rail尝试每种可能的board
	int railLen = railList[ nRailCursor ];

	// 优化.无法再使用的board碎片.
	int nWastedBoard = 0;

	// 优化,如果当前rail同之前的rail长度一样,那么就从上个rail所在的board序号开始尝试截取.
	int nBoardBegin = 0;
	if ( railLen == lastRailLen )
	{
		nBoardBegin = lastBoardIndex;
		nWastedBoard = nCurWastedBoard;
	}
	
	for ( int boardIndex=nBoardBegin; boardIndex<boardList.size(); ++boardIndex )
	{
		int& nBoardLen = boardList[ boardIndex ];

		// 跳过相同长度的board
		if ( boardIndex > 0 )
		{
			if ( nBoardLen == boardList[ boardIndex-1 ] )
			{
				continue;
			}
		}
		

		if ( nBoardLen >= railLen )
		{
			if ( ( nBoardLen - railLen ) < railLen )
			{
				if( ( nWastedBoard + ( nBoardLen - railLen ) ) > nMaxWastedBoard )
				{
					// 这个不行..
					continue;
				}
			}

			nBoardLen -= railLen;	// 尝试截掉rail长度。

			bool bRet = SearchDeepthLimited( nDeepthLeft-1, boardList, railList, nRailCursor+1,
				boardIndex, railLen, nMaxWastedBoard, nWastedBoard );

			nBoardLen += railLen;	// 恢复截掉的长度，尝试从别的board上截。

			if ( bRet )
			{
				return bRet;	// 成功了.
			}
		}
		else
		{
			// 这块board上剩余的部分已经不能再被使用.
			nWastedBoard += nBoardLen;
			if ( nWastedBoard > nMaxWastedBoard )
			{
				break;	// 超过了最多能浪费的木料数目,剪枝.
			}
		}
	}

	// 还有种情况，就是不截这段rail.因为rail已经被排序,所以这种情况不用考虑,因为后面的rail肯定能被前面不比他大的rail所代替.
//	u32 railNum = SearchDeepthLimited( nDeepthLeft-1, boardList, railList, nRailCursor+1 );
//	nMaxRailNum = max( nMaxRailNum, railNum );

	return false;
}

int main()
{
	string strProblemName = "fence8";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nBoardNum;
	fin >> nBoardNum;
	
	
	TLengthList tBoardSupplied;

	for ( int i=0; i<nBoardNum; ++i )
	{
		int nLen;
		fin >> nLen;
		tBoardSupplied.push_back( nLen );
	}

	int nRailNum;
	TLengthList tRailNeeded;
	fin >> nRailNum;
	for ( int i=0; i<nRailNum; ++i )
	{
		int nLen;
		fin >> nLen;
		tRailNeeded.push_back( nLen );
	}

	// 优化，将rail从小到大排序，这样便于做去重优化。
	std::sort( tRailNeeded.begin(), tRailNeeded.end() );

	// 优化,将board从大到小排序,这样便于做优化.
	std::sort( tBoardSupplied.rbegin(), tBoardSupplied.rend() );

	// 最大深度，在这个题目里，就是需要的rail数目。
	const int MaxLenth = nRailNum+1;

	int nTotalBoardLen = 0;
	for ( int i=0; i<tBoardSupplied.size(); ++i )
	{
		nTotalBoardLen += tBoardSupplied[i];
	}
	TLengthList tMaxWastedBoardList;

	int nTotalRailLenth = 0;
	for ( int i=0; i<tRailNeeded.size(); ++i )
	{
		nTotalRailLenth += tRailNeeded[i];
		tMaxWastedBoardList.push_back( nTotalBoardLen - nTotalRailLenth );
	}

	int maxRailNum = 0;
	for ( int nDeepth=1; nDeepth<MaxLenth; ++nDeepth )
	{
		// 最多能浪费的木料.
		int maxWastedBoard = tMaxWastedBoardList[ nDeepth-1 ];
		if ( maxWastedBoard < 0 )
		{
			break;	// 木料不够.
		}

		bool bRet = SearchDeepthLimited( nDeepth, tBoardSupplied, tRailNeeded, 0, 0, -1, maxWastedBoard, 0 );

		// 深度,就是rail数目.
		// 因为rail是从小到大排序的,所以,如果不能得到现在所有的rail的时候,后面更大的rail更是得不到.
		// 所以可以直接剪枝.
		if ( !bRet )
		{
			break;
		}
		maxRailNum = nDeepth;
	}

	fout << maxRailNum << endl;
	

#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}
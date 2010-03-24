/*
ID: thinkin6
PROG: nocows
LANG: C++
*/

/** USA CO 2.3.2 :

Cow Pedigrees

Silviu Ganceanu -- 2003
Farmer John is considering purchasing a new herd of cows. In this new herd, each mother cow gives birth to two children. The relationships among the cows can easily be represented by one or more binary trees with a total of N (3 <= N < 200) nodes. The trees have these properties:

The degree of each node is 0 or 2. The degree is the count of the node's immediate children.
The height of the tree is equal to K (1 < K <100). The height is the number of nodes on the longest path from the root to any leaf; a leaf is a node with no children.
How many different possible pedigree structures are there? A pedigree is different if its tree structure differs from that of another pedigree. Output the remainder when the total number of different possible pedigrees is divided by 9901.

PROGRAM NAME: nocows

INPUT FORMAT

Line 1: Two space-separated integers, N and K.
SAMPLE INPUT (file nocows.in)

5 3
OUTPUT FORMAT

Line 1: One single integer number representing the number of possible pedigrees MODULO 9901.
SAMPLE OUTPUT (file nocows.out)

2
OUTPUT DETAILS

Two possible pedigrees have 5 nodes and height equal to 3:
@                   @      
/ \                 / \
@   @      and      @   @
/ \                     / \
@   @                   @   @
*/

/** 
思路：
 

*/


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

#ifdef _WIN32
#define THINKINGL 1
#endif

int Log2( int nNum )
{
// 	if ( nNum == 0 )
// 	{
// 		return -9999999;
// 	}
// 	int nRet = 0;
// 	while( nNum != 0x1 )
// 	{
// 		nNum >>= 1;
// 		++nRet;
// 	}
	float fRet = log( (float)nNum ) / log( 2.0 );
	int nRet = fRet + 0.5;
	return nRet;
}

int Count( int nCowNum, int nTreeHeight )
{
	if ( nCowNum <= 3 || nTreeHeight <= 2 )
	{
		return 1;
	}
	
	// - root cow.
	nCowNum -= 1;
	// height left.
	nTreeHeight -= 1;

	int nMinCowToHeight = ( nTreeHeight << 1 ) - 1; // <<1 == *2.
	int nMaxCowToHeight = ( 1 << ( nTreeHeight ) ) - 1;

	int nCount = 0;

	// nToHeightCow one side to reach nTreeHeight height.
	for ( int nToHeightCow = nMinCowToHeight; nToHeightCow <= nMaxCowToHeight; nToHeightCow += 2 )
	{
		int nToHeightCount = Count( nToHeightCow, nTreeHeight );

		int nLeftCow = nCowNum - nToHeightCow;
		if ( nLeftCow < 1 || nLeftCow > nMaxCowToHeight ) // 
		{
			break;
		}

		int nMinHeight = Log2( nLeftCow + 1 );
		int nMaxHeight = ( nLeftCow + 1 ) >> 1; // >> 1 == /2.

		nMaxHeight = min( nMaxHeight, nTreeHeight );

		int nAnotherCount = 0;
		for ( int nAnotherHeight = nMinHeight; nAnotherHeight <= nMaxHeight; ++ nAnotherHeight )
		{
			nAnotherCount += Count( nLeftCow, nAnotherHeight );
		}

		if ( nLeftCow == nToHeightCow )
		{
			// 减去限制长度那边的被对称的，每种都会有两种对称情况，一种是中间轴对称，一种是平移全等。
			// 可能不对。
			nCount += ( nToHeightCount * nToHeightCount*2 - 2 * nToHeightCount); 
		}
		else if( nLeftCow <= nMaxCowToHeight && nLeftCow >= nMinCowToHeight )
		{
			nCount += ( nToHeightCount * nAnotherCount  );
		}
		else
		{
			nCount += ( nToHeightCount * nAnotherCount * 2 );	// 数目不相等，肯定不会对称。
		}		
	}	

	// 
	if ( nCount < 1 )
	{
		nCount = 1;
	}
	return nCount;
}

int main()
{
	string strProblemName = "nocows";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nCowNum, nTreeHeight;
	fin >> nCowNum >> nTreeHeight;

	int nCount = Count( nCowNum, nTreeHeight );

	fout << nCount << endl;


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
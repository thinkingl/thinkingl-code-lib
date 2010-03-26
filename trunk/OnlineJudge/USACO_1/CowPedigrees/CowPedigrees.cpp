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
*	对高度做限制求组合的话太乱了，我搞了好久也没搞出来。
*	转换为高度为K的树的数目=高度不小于K的树的数目-高度不小于K-1的树的数目。
*	高度不小于K的树的数目相对来说简单多了。
*	对于每个数，看左右两边，左边从1到N-2，剩余的放在右边，求两边子树的数目。

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


u64 g_arCount[100][200];

u64 CountDP(  int nCowNum, int nTreeHeight )
{
	if ( nCowNum <= 1 || nTreeHeight <= 1 )
	{
		if ( nCowNum == 1 )
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if ( (u64)-1 != g_arCount[nTreeHeight][nCowNum] )
	{
		return g_arCount[nTreeHeight][nCowNum];
	}
	else
	{
		u64 count = 0;
		--nCowNum;
		for ( int i=1; i<nCowNum; ++i )
		{
			u64 countLeft = CountDP( i, nTreeHeight - 1 );
			u64 countRight = CountDP( nCowNum - i, nTreeHeight - 1 );
			
			count += countLeft * countRight;
		}

		count %= 9901;
		g_arCount[nTreeHeight][nCowNum+1] = count;

		return count;
	}

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

	memset( g_arCount, -1, sizeof( g_arCount ) );

	int nCowNum, nTreeHeight;
	fin >> nCowNum >> nTreeHeight;

	u64 count1 = CountDP( nCowNum, nTreeHeight );
	u64 count2 = CountDP( nCowNum, nTreeHeight - 1 );
	u64 nCount = count1 - count2 + 9901; // maybe count2 < count1.

	nCount %= 9901;

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
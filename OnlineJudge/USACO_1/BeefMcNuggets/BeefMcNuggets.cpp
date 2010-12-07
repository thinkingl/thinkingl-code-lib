/*
ID: thinkin6
PROG: nuggets
LANG: C++
*/

/** USA CO 4.1.1 :
Beef McNuggets
Hubert Chen
Farmer Brown's cows are up in arms, having heard that McDonalds is considering the introduction of a new product: Beef McNuggets. The cows are trying to find any possible way to put such a product in a negative light.

One strategy the cows are pursuing is that of `inferior packaging'. ``Look,'' say the cows, ``if you have Beef McNuggets in boxes of 3, 6, and 10, you can not satisfy a customer who wants 1, 2, 4, 5, 7, 8, 11, 14, or 17 McNuggets. Bad packaging: bad product.''

Help the cows. Given N (the number of packaging options, 1 <= N <= 10), and a set of N positive integers (1 <= i <= 256) that represent the number of nuggets in the various packages, output the largest number of nuggets that can not be purchased by buying nuggets in the given sizes. Print 0 if all possible purchases can be made or if there is no bound to the largest number.

The largest impossible number (if it exists) will be no larger than 2,000,000,000.

PROGRAM NAME: nuggets

INPUT FORMAT

Line 1:	 N, the number of packaging options
Line 2..N+1:	The number of nuggets in one kind of box
SAMPLE INPUT (file nuggets.in)

3
3
6
10
OUTPUT FORMAT

The output file should contain a single line containing a single integer that represents the largest number of nuggets that can not be represented or 0 if all possible purchases can be made or if there is no bound to the largest number.
SAMPLE OUTPUT (file nuggets.out)

17

*/

/** 
*	思路：当两个数 m,n 最大公约数为1的时候（就是互质），方程 am + bn = c 在 c> m,n的最小公倍数的时候可以有正整数的 a，b 能满足条件。
*	这道题目的规模小，最大256，所以不需要用高级算法。
*	首先，找互质的一对，如果找不到，那么将没有最大的不能构成的数。
*	如果至少存在互质的一对，那么开始穷尽组合出可能的数字，在最大的数的平方时剪枝停止循环。
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

/** 求最大公约数。 */
template< class _Type >
_Type gcd( _Type a, _Type b )
{
	while ( a != b )
	{
		if ( a < b )
		{
			b -= a;
		}
		else
		{
			a -= b;
		}
	}
	return a;
}

int main()
{
	string strProblemName = "nuggets";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nPakageNum;
	fin >> nPakageNum;

	typedef std::set< int > TIntSet;
	TIntSet allPossibleSequence;

	typedef std::vector<int> TIntVector;
	TIntVector allPackageSequence;
	for ( int i=0; i<nPakageNum; ++i )
	{
		int nPackageSize;
		fin >> nPackageSize;
		allPackageSequence.push_back( nPackageSize );
		allPossibleSequence.insert( nPackageSize );
	}

	std::sort( allPackageSequence.begin(), allPackageSequence.end() );

	// 先找它们的最大公约数，如果存在一对数字，它们的最大公约数为1，那么题目有结果。。
	// 否则没有题目要求的结果。
	bool bPass = false;
	for ( int i=0; i<allPackageSequence.size() && !bPass; ++i )
	{		
		for ( int k=i+1; k<allPackageSequence.size() && !bPass; ++k )
		{
			int gcdRet = gcd( allPackageSequence[i], allPackageSequence[k] );
			if ( gcdRet == 1 )
			{
				bPass = true;
			}
		}		
	}

	if ( !bPass )
	{
		fout << "0" << endl;
	}
	else
	{
		// 寻找最大的可能组合。
		int nCurMaxPackage = *( allPackageSequence.end() - 1 );

		// 题目要求的 不可能得到的组合 不会大于这个数字。
		int nPackageRange = nCurMaxPackage * nCurMaxPackage;

		TIntSet::iterator itPossible = allPossibleSequence.begin();

		while ( itPossible != allPossibleSequence.end() )
		{
			for ( int i=0; i<allPackageSequence.size(); ++i )
			{
				int newPackage = allPackageSequence[i] + *itPossible;
				if ( newPackage < nPackageRange )
				{
					// 有效。
					allPossibleSequence.insert( newPackage );
				}
			}

			++itPossible;
		}

		// 寻找最大的。
		TIntSet::reverse_iterator riter = allPossibleSequence.rbegin();
		int nCurNum = *riter;
		int nMaxImposible = 0;
		for ( ++riter; riter != allPossibleSequence.rend(); ++riter )
		{
			nCurNum--;
			if ( *riter != nCurNum )
			{
				// 找到了。
				nMaxImposible = nCurNum;
				break;
			}			
		}

		if ( 0==nMaxImposible )
		{
			nMaxImposible = *allPossibleSequence.begin() - 1;
		}
		
		fout << nMaxImposible << endl;
	}

#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}
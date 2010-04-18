/*
ID: thinkin6
PROG: humble
LANG: C++
*/

/** USA CO 3.1.3 :
Humble Numbers
For a given set of K prime numbers S = {p1, p2, ..., pK}, consider the set of all numbers whose prime factors are a subset of S. This set contains, for example, p1, p1p2, p1p1, and p1p2p3 (among others). This is the set of `humble numbers' for the input set S. Note: The number 1 is explicitly declared not to be a humble number.

Your job is to find the Nth humble number for a given set S. Long integers (signed 32-bit) will be adequate for all solutions.

PROGRAM NAME: humble

INPUT FORMAT

Line 1:	 Two space separated integers: K and N, 1 <= K <=100 and 1 <= N <= 100,000.
Line 2:	 K space separated positive integers that comprise the set S.
SAMPLE INPUT (file humble.in)

4 19
2 3 5 7
OUTPUT FORMAT

The Nth humble number from set S printed alone on a line.
SAMPLE OUTPUT (file humble.out)

27

*/

/** 
思路：
*	地毯式从前向后滚动相乘，一直乘到题目要求的个数。
*	根据题目要求限制最大结果，进行剪枝。数字集合大小到达题目要求的数目后，新的结果如果大于最后一个积，那么就剪掉，因为剩下的肯定是不符合要求的。
*	如果不用stl的话，会难一些。
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



int main()
{
	string strProblemName = "humble";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nPrimNumCount, nTargetCount;
	fin >> nPrimNumCount >> nTargetCount;

	typedef std::set<u64> TNumSet;
	TNumSet tNumSet;
	typedef std::vector<u32> TNumVector;

	TNumVector tPrimList;

	u32 nMaxPrim = 0;
	for ( int i=0; i<nPrimNumCount; ++i )
	{
		u32 nNum;
		fin >> nNum;
		tNumSet.insert( nNum );
		tPrimList.push_back( nNum );

		if ( nMaxPrim < nNum )
		{
			nMaxPrim = nNum;
		}
	}

	
	TNumSet::iterator iter = tNumSet.begin();
	int nCount = 0;
	u32 nMaxNum = -1;
	while ( nCount < nTargetCount )
	{
		for ( int i=0; i<tPrimList.size(); ++i )
		{
			if ( tPrimList[i] > *iter || iter == tNumSet.end() )
			{
				break;
			}
			// 求取新的积，继续放入队列。
			u64 nNewNum = tPrimList[i] * *iter;		

			// 剪枝。当数字集合的数目已经满足条件的时候，最后一个是最大的，新生成的数字如果比它还大，那么肯定不符合要求。
			// 剩下的符合要求的应该比最后一个数字小。
			if ( tNumSet.size() >= nTargetCount )
			{
				TNumSet::iterator iterEnd = tNumSet.end();
				--iterEnd;
				nMaxNum = *iterEnd;
			}
			if ( nNewNum > nMaxNum )
			{
				break;
			}

			tNumSet.insert( nNewNum );	

			// 保持数字集合的大小为题目要求的个数。
			if ( tNumSet.size() > nTargetCount )
			{
				TNumSet::iterator iterEnd = tNumSet.end();
				--iterEnd;
				if ( iterEnd != iter )
				{
					tNumSet.erase( iterEnd );
				}
				
			}
			
		}
				
		++iter;
		nCount ++;
	}

	// 因为上面在计数nCount++之前iter++了。所以指向的是下一个数字。
	--iter;

	fout << *iter << endl;


#if 0 // 下面的算法会超时。
	// 自己估计的一个最大数。
	const u32 MAX_NUM = 1000000000;
	
	TNumSet::iterator iter = tNumSet.begin();
	int nCount = 0;
	while ( nCount < nTargetCount )
	{
		for ( TNumSet::iterator subIter = tNumSet.begin(); *subIter <= *iter; ++subIter )
		{
			// 求取新的积，继续放入队列。
			u32 nNewNum = *subIter * *iter;
			if ( nNewNum > MAX_NUM )
			{
				break;	// 这个数太大了，肯定不是题目需要的。
			}
			tNumSet.insert( nNewNum );	
		}
		++iter;
		nCount ++;
	}

	// 因为上面在计数nCount++之前iter++了。所以指向的是下一个数字。
	--iter;

	fout << *iter << endl;

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
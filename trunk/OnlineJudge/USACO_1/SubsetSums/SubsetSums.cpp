/*
ID: thinkin6
PROG: subset
LANG: C++
*/

/** USA CO 2.2.4 :
Subset Sums

Subset Sums
JRM
For many sets of consecutive integers from 1 through N (1 <= N <= 39), one can partition the set into two sets whose sums are identical.

For example, if N=3, one can partition the set {1, 2, 3} in one way so that the sums of both subsets are identical:

{3} and {1,2}
This counts as a single partitioning (i.e., reversing the order counts as the same partitioning and thus does not increase the count of partitions).

If N=7, there are four ways to partition the set {1, 2, 3, ... 7} so that each partition has the same sum:

{1,6,7} and {2,3,4,5}
{2,5,7} and {1,3,4,6}
{3,4,7} and {1,2,5,6}
{1,2,4,7} and {3,5,6}
Given N, your program should print the number of ways a set containing the integers from 1 through N can be partitioned into two sets whose sums are identical. Print 0 if there are no such ways.

Your program must calculate the answer, not look it up from a table.

PROGRAM NAME: subset

INPUT FORMAT

The input file contains a single line with a single integer representing N, as above.
SAMPLE INPUT (file subset.in)

7
OUTPUT FORMAT

The output file contains a single line with a single integer that tells how many same-sum partitions can be made from the set {1, 2, ..., N}. The output file should contain 0 if there are no ways to make a same-sum partition.

SAMPLE OUTPUT (file subset.out)

*/

/** 
思路：
从1加到题目给出的最大的数，就可以得到总的和，除以2，就可以得到两组分别的和S。
然后从1到N/2，从小到大组合所有的数，如果组合出和，就意味着找到了一个结果，计数，继续找。

优化：在寻找的时候使用递归。
判断当前和和目标和的大小，如果小于S，继续找。如果大于S，截至。

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


typedef std::vector<int> TNumList;
typedef std::vector< TNumList > TNumListSet;

int g_arNumCountByNumCount[40] = {0};

int SumTest( /*TNumListSet& tNumListSet, TNumList& tCurNum,*/ 
			int nCurSum, int nLastNum, int nCurNumCount, const int nMaxNumCount, const int nMaxNum, const int nSumTarget )
{
	int nPartitionNum = 0;


	if ( nCurSum < nSumTarget )
	{
		if ( nCurNumCount >= nMaxNumCount )
		{
			return 0;
		}
		// 继续。
		//int nLastNum = 0;
		//if( !tCurNum.empty() )
		//{
		//	nLastNum = tCurNum[ tCurNum.size() - 1 ];
		//}

		for ( int i=nLastNum + 1; i<=nMaxNum; ++i )
		{
//			TNumList tNewOne = tCurNum;
//			tNewOne.push_back( i );

			nPartitionNum += SumTest( /*tNumListSet, tNewOne, */
				nCurSum + i, i, nCurNumCount + 1,
				nMaxNumCount, nMaxNum, nSumTarget );
		}
		return nPartitionNum;
	}
	else if( nCurSum > nSumTarget )
	{
		return nPartitionNum;
	}
	else
	{
		// find it!
	//	tNumListSet.push_back( tCurNum );

		//cout << "find one partition!   ";
		//for ( int i=0; i<tCurNum.size(); ++i )
		//{
		//	cout << tCurNum[i] << " " ;
		//}
		//cout << endl;
		g_arNumCountByNumCount[ nCurNumCount ] ++;

		nPartitionNum = 1;
	}
	return nPartitionNum;
}

int main()
{
    string strProblemName = "subset";

    string strInFile = strProblemName + ".in";
    string strOutFile = strProblemName + ".out";


    ofstream fout ( strOutFile.c_str() );
    ifstream fin ( strInFile.c_str() );

    if( !fin )
    {
        cout << "open input file fail!" << endl;
        return 0;
    }

	int nMaxNum;
	fin >> nMaxNum;

	int nSumAll = 0;
	for ( int i=1; i<=nMaxNum; ++i )
	{
		nSumAll += i;
	}

	int nSumEach = nSumAll / 2;

	int nCount = nMaxNum / 2 ;

	TNumListSet tSet;
	TNumList tCurList;
	int nSetNum = SumTest( /*tSet, tCurList,*/ 0, 0, 0, nCount, nMaxNum, nSumEach );

	// 偶数数目的数字时，最后一次组合会多组合一半。
	if( ( nMaxNum & 0x01 ) == 0 )
	{
		nSetNum -= ( g_arNumCountByNumCount[ nCount ] / 2 );
	}

	fout << nSetNum << endl;

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
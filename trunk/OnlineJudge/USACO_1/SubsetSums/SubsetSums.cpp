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

问题：31时的规模，C15 31 = 300540195 太大了，看来有必要进一步做算法优化。

算法2：
本题结果规模非常大，如果像上面算法那样穷举遍历铁定超时，所以存在偷懒算法，只得到题目要求的结果，不穷举真正的可能组合。
设函数 F(i,s) 表示 数字1到i组成的集合的所有子集中，所有元素和等于s的子集的数目。
则 F( i+1, s ) 就是在 1到i 组成的集合的基础上增加 i+1. 之前的 F(i,s) 是在新加入的 i+1 不参与的情况下符合条件的
子集的数目。而 i+1 参与的情况下, 前面i个数的子集应该提供和 s-(i+1) 子集的数目应该就是 F( i, s-(i+1) ).

即： F( i+1,s ) = F( i,s ) + F( i,s-(i+1) )
可以知道，对于所有的数i，都有F( i,0 ) = 1,这样，我们就可以用数学归纳的办法一级级得到子集的数目。

题目要求的是拆成两组和相同的不同拆发，也就等于求 F( i,S/2 ) / 2.

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

const int MAX_NUM = 39;
const int MAX_SUM = ( 1 + MAX_NUM ) * MAX_NUM / 2;

u64 CountSubset( int nMaxNum )
{
	int nSum = ( 1 + nMaxNum ) * nMaxNum / 2;
	// 奇数的话无解。
	if ( ( nSum & 1 ) != 0)
	{
		return 0;
	}
	// 我们要得到的目标。
	nSum >>= 1;

	// 存放数目的数组。
	u64 arSubsetNum[MAX_NUM+1][MAX_SUM+1];
	int nSize = sizeof( arSubsetNum );

	memset( arSubsetNum, 0, nSize );

	for ( int i=0; i<MAX_NUM; ++i )
	{
		arSubsetNum[i][0] = 1;
	}

	for ( int i=1; i<=nMaxNum; ++i )
	{
		for ( int s=1; s<=nSum; ++s )
		{
			arSubsetNum[i][s] = arSubsetNum[i-1][s] + arSubsetNum[i-1][s-i];
		}
	}
	u64 uSubsetNum = arSubsetNum[nMaxNum][nSum] >> 1;
	return uSubsetNum;

}


typedef std::vector<int> TNumList;
typedef std::vector< TNumList > TNumListSet;

int g_arNumCountByNumCount[40] = {0};
/** 最大的个数，超过此个数剪枝，不算入。 */
int g_nMaxNumCount = 0;
/** 目标，是所有数的和的一半。 */
int g_nSumTarget = 0;
/** 最大的数，题目输入。 */
int g_nMaxNum;
/** 在从大到小的顺序寻找时，前一部分所有的数的和小于目标和的时候就没有必要再继续找了。
*	这就是那个截至点。
*/
int g_nStopNum;

/** 从到大到小的顺序寻找时，到轮到一个数的时候，如果这个数和前面所有的数的和都小于差额，就不必再尝试了。*/
int g_arStopPoints[40] = {-1};


/** 从大到小的顺序来寻找。 */
int SumTest2( /*TNumListSet& tNumListSet, TNumList& tCurNum,*/ 
			 int nCurSum, int nLastNum, int nCurNumCount )
{
	int nPartitionNum = 0;


	if ( nCurSum < g_nSumTarget )
	{


		if ( nCurNumCount >= g_nMaxNumCount )
		{
			return 0;
		}
		// 继续。
		//int nLastNum = 0;
		//if( !tCurNum.empty() )
		//{
		//	nLastNum = tCurNum[ tCurNum.size() - 1 ];
		//}

		for ( int i=nLastNum ; i>0; --i )
		{
			//			TNumList tNewOne = tCurNum;
			//			tNewOne.push_back( i );

			// 剩下的数字不足以得到这个和。
			// 神奇的剪枝！
			if ( nCurSum < g_arStopPoints[i] )
			{
				return nPartitionNum;
			}

			nPartitionNum += SumTest2( /*tNumListSet, tNewOne, */
				nCurSum + i, i-1, nCurNumCount + 1 );
		}
		return nPartitionNum;
	}
	else if( nCurSum > g_nSumTarget )
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


int SumTest( /*TNumListSet& tNumListSet, TNumList& tCurNum,*/ 
			int nCurSum, int nLastNum, int nCurNumCount )
{
	int nPartitionNum = 0;


	if ( nCurSum < g_nSumTarget )
	{
		if ( nCurNumCount >= g_nMaxNumCount )
		{
			return 0;
		}
		// 继续。
		//int nLastNum = 0;
		//if( !tCurNum.empty() )
		//{
		//	nLastNum = tCurNum[ tCurNum.size() - 1 ];
		//}

		for ( int i=nLastNum + 1; i<=g_nMaxNum; ++i )
		{
			//			TNumList tNewOne = tCurNum;
			//			tNewOne.push_back( i );

			nPartitionNum += SumTest( /*tNumListSet, tNewOne, */
				nCurSum + i, i, nCurNumCount + 1 );
		}
		return nPartitionNum;
	}
	else if( nCurSum > g_nSumTarget )
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

	g_nMaxNum = nMaxNum;
	g_nSumTarget = nSumEach;
	g_nMaxNumCount = nCount;

	int nSumStop = 0;
	g_nStopNum = 0;
	for ( int i=0; nSumStop < g_nSumTarget; i++ )
	{
		nSumStop += i;
		g_nStopNum = i;
		g_arStopPoints[i] = g_nSumTarget - nSumStop;
	}


	TNumListSet tSet;
	TNumList tCurList;
// 	int nSetNum = SumTest2( 0, g_nMaxNum, 0 );	
// 	//	int nSetNum = SumTest( /*tSet, tCurList,*/ 0, 0, 0 );
// 
// 	// 偶数数目的数字时，最后一次组合会多组合一半。
// 	if( ( nMaxNum & 0x01 ) == 0 )
// 	{
// 		nSetNum -= ( g_arNumCountByNumCount[ nCount ] / 2 );
// 	}
	u64 nSetNum = CountSubset( nMaxNum );

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
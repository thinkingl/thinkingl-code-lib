/*
ID: thinkin6
PROG: stamps
LANG: C++
*/

/** USA CO 3.1.6 :
Stamps
Given a set of N stamp values (e.g., {1 cent, 3 cents}) and an upper limit K to the number of stamps that can fit on an envelope, calculate the largest unbroken list of postages from 1 cent to M cents that can be created.

For example, consider stamps whose values are limited to 1 cent and 3 cents; you can use at most 5 stamps. It's easy to see how to assemble postage of 1 through 5 cents (just use that many 1 cent stamps), and successive values aren't much harder:

6 = 3 + 3
7 = 3 + 3 + 1
8 = 3 + 3 + 1 + 1
9 = 3 + 3 + 3
10 = 3 + 3 + 3 + 1
11 = 3 + 3 + 3 + 1 + 1
12 = 3 + 3 + 3 + 3
13 = 3 + 3 + 3 + 3 + 1.
However, there is no way to make 14 cents of postage with 5 or fewer stamps of value 1 and 3 cents. Thus, for this set of two stamp values and a limit of K=5, the answer is M=13.

The most difficult test case for this problem has a time limit of 3 seconds.

PROGRAM NAME: stamps

INPUT FORMAT

Line 1:	 Two integers K and N. K (1 <= K <= 200) is the total number of stamps that can be used. N (1 <= N <= 50) is the number of stamp values.
Lines 2..end:	N integers, 15 per line, listing all of the N stamp values, each of which will be at most 10000.
SAMPLE INPUT (file stamps.in)

5 2
1 3
OUTPUT FORMAT

Line 1:	One integer, the number of contiguous postage values starting at 1 cent that can be formed using no more than K stamps from the set.
SAMPLE OUTPUT (file stamps.out)

13
*/

/** 
思路：
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

/** 最大邮票数。 */
const int MAX_STAMPS_NUM = 200;
/** 最多面值数目。 */
const int MAX_STAMP_VALUE_NUM = 50;
/** 最大面值金额。 */
const int MAX_STAMP_VALUE = 10000;

/** 邮票可能的最大金额和. */
const int MAX_VALUE_SUM = MAX_STAMPS_NUM * MAX_STAMP_VALUE;

/** 邮票面值表。 */
static u32 arStampValue[MAX_STAMP_VALUE_NUM] = { -1 };

int main()
{
	string strProblemName = "stamps";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nMaxStampNum, nStampValueNum;
	fin >> nMaxStampNum >> nStampValueNum;
	for ( int i=0; i<nStampValueNum; ++i )
	{
		fin >> arStampValue[ i ];
	}

	int *arValueSumFlag = new int[MAX_VALUE_SUM+1];
	memset( arValueSumFlag, 0, sizeof( arValueSumFlag[0] ) * MAX_VALUE_SUM+1 );

	typedef std::vector<int> TIntVector;
	TIntVector tCurSumValue;
	tCurSumValue.push_back( 0 );

	for ( int i=0; i<nMaxStampNum; ++i )
	{
		TIntVector tNewSumValue;
		for ( int k=0; k<tCurSumValue.size(); ++k )
		{
			for ( int j = 0; j<nStampValueNum; ++j )
			{
				int nSum = tCurSumValue[k] + arStampValue[ j ];
				if ( !arValueSumFlag[nSum] )
				{
					tNewSumValue.push_back( nSum );
					arValueSumFlag[nSum] = true;
//					tAllSumValue.insert( nSum );
				}
			}

		}
		tCurSumValue = tNewSumValue;
	}

	// 找出连续最多的.
	int nMaxContinueNum = 0;
	int nMaxContinueTail = 0;
	int nLastNum = -1;
	int nCurContinueNum = 1;
	//	int nCurContinueHead = 0;

	for ( int i=0; i<MAX_VALUE_SUM; ++i  )
	{
		if ( !arValueSumFlag[i] )
		{
			continue;
		}
		int nCurNum = i;

		if ( nLastNum+1 == nCurNum )
		{
			// 连续.
			nCurContinueNum ++;
		}
		else
		{
			// 不连续了.	
			nCurContinueNum = 1;
		}

		nLastNum = nCurNum;

		if ( nCurContinueNum > nMaxContinueNum )
		{
			nMaxContinueNum = nCurContinueNum;
			nMaxContinueTail = nLastNum;
		}


	}
	
	delete[] arValueSumFlag;

	fout << nMaxContinueTail << endl;

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
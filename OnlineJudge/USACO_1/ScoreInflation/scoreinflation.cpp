/*
ID: thinkin6
PROG: inflate
LANG: C++
*/

/** USA CO 3.1.2 :
Score Inflation
The more points students score in our contests, the happier we here at the USACO are. We try to design our contests so that people can score as many points as possible, and would like your assistance.

We have several categories from which problems can be chosen, where a "category" is an unlimited set of contest problems which all require the same amount of time to solve and deserve the same number of points for a correct solution. Your task is write a program which tells the USACO staff how many problems from each category to include in a contest so as to maximize the total number of points in the chosen problems while keeping the total solution time within the length of the contest.

The input includes the length of the contest, M (1 <= M <= 10,000) (don't worry, you won't have to compete in the longer contests until training camp) and N, the number of problem categories, where 1 <= N <= 10,000.

Each of the subsequent N lines contains two integers describing a category: the first integer tells the number of points a problem from that category is worth (1 <= points <= 10000); the second tells the number of minutes a problem from that category takes to solve (1 <= minutes <= 10000).

Your program should determine the number of problems we should take from each category to make the highest-scoring contest solvable within the length of the contest. Remember, the number from any category can be any nonnegative integer (0, one, or many). Calculate the maximum number of possible points.

PROGRAM NAME: inflate

INPUT FORMAT

Line 1:	M, N -- contest minutes and number of problem classes
Lines 2-N+1:	 Two integers: the points and minutes for each class
SAMPLE INPUT (file inflate.in)

300 4
100 60
250 120
120 100
35 20
OUTPUT FORMAT

A single line with the maximum number of points possible given the constraints.
SAMPLE OUTPUT (file inflate.out)

605
(Take two problems from #2 and three from #4.) 
*/

/** 
思路：
*	之前做过至少一道背包问题了，但我没有意识到。直到这道题，我绞尽脑汁想，突然灵光一现，想到了背包问题。
	和其它背包问题一样，动态规划可以解决。
	如果用 M（i，t）来表示用i个catalogies中的问题凑t分钟的比赛能得到的最高分，那么
	M（i+1,t） = max{ M（i+1， t- Time（i+1） ） + Points（i+1）） ， M（i，t） }
	因为，增加一个category后有两种情况，一种是新的category能参与，则至少有一个，是前面的表达式。
	另一种情况是新的第（i+1）个category不参与，则由前i个组成t分钟，是后面的表达式。
	两种情况中大的那个，就是用i+1种catalogies组成不少于t分钟题目的最大分数。
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
	string strProblemName = "inflate";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nContentTime, nCategoryNum;
	fin >> nContentTime >> nCategoryNum;

	const int MAX_CATEGORY_NUM = 10000+1;
	const int MAX_TIME = 10000+1;
	int arCategoryTime[ MAX_CATEGORY_NUM ];
	int arCategoryPoint[ MAX_CATEGORY_NUM ];
	for ( int i=0; i<nCategoryNum; ++i )
	{
		fin >> arCategoryPoint[i] >> arCategoryTime[i];
	}

	u32 arMaxPoint[MAX_CATEGORY_NUM] = { 0 };
	
	for ( int nTime = 0; nTime <= nContentTime; ++nTime )
	{
		u32 lastPoint = 0;
		for ( int nNum=0; nNum < nCategoryNum; ++ nNum )
		{
			if ( nTime - arCategoryTime[nNum] < 0 )
			{
				arMaxPoint[nTime] = lastPoint;
			}
			else
			{
				arMaxPoint[nTime] = max(
					arMaxPoint[ nTime - arCategoryTime[nNum] ] + arCategoryPoint[ nNum ] ,
					lastPoint
					);
			}
			lastPoint = arMaxPoint[ nTime ];
		}
	}

	fout << arMaxPoint[ nContentTime ] << endl;

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
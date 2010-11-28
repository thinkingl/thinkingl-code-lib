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
思路：
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




#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}
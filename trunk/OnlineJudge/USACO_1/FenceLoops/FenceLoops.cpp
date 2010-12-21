/*
ID: thinkin6
PROG: fence6
LANG: C++
*/

/** USA CO 4.1.3 :
Fence Loops
The fences that surround Farmer Brown's collection of pastures have gotten out of control. They are made up of straight segments from 1 through 200 feet long that join together only at their endpoints though sometimes more than two fences join together at a given endpoint. The result is a web of fences enclosing his pastures. Farmer Brown wants to start to straighten things out. In particular, he wants to know which of the pastures has the smallest perimeter.

Farmer Brown has numbered his fence segments from 1 to N (N = the total number of segments). He knows the following about each fence segment:

the length of the segment
the segments which connect to it at one end
the segments which connect to it at the other end.
Happily, no fence connects to itself.
Given a list of fence segments that represents a set of surrounded pastures, write a program to compute the smallest perimeter of any pasture. As an example, consider a pasture arrangement, with fences numbered 1 to 10 that looks like this one (the numbers are fence ID numbers):

1
+---------------+
|\             /|
2| \7          / |
|  \         /  |
+---+       /   |6
| 8  \     /10  |
3|     \9  /     |
|      \ /      |
+-------+-------+
4       5
The pasture with the smallest perimeter is the one that is enclosed by fence segments 2, 7, and 8.

PROGRAM NAME: fence6

INPUT FORMAT

Line 1:	 N (1 <= N <= 100)
Line 2..3*N+1:	
N sets of three line records:

The first line of each record contains four integers: s, the segment number (1 <= s <= N); Ls, the length of the segment (1 <= Ls <= 255); N1s (1 <= N1s <= 8) the number of items on the subsequent line; and N2sthe number of items on the line after that (1 <= N2s <= 8).
The second line of the record contains N1 integers, each representing a connected line segment on one end of the fence.
The third line of the record contains N2 integers, each representing a connected line segment on the other end of the fence.
SAMPLE INPUT (file fence6.in)

10
1 16 2 2
2 7
10 6
2 3 2 2
1 7
8 3
3 3 2 1
8 2
4
4 8 1 3
3
9 10 5
5 8 3 1
9 10 4
6
6 6 1 2 
5 
1 10
7 5 2 2 
1 2
8 9
8 4 2 2
2 3
7 9
9 5 2 3
7 8
4 5 10
10 10 2 3
1 6
4 9 5
OUTPUT FORMAT

The output file should contain a single line with a single integer that represents the shortest surrounded perimeter.
SAMPLE OUTPUT (file fence6.out)

12
 
*/

/** 
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


int main()
{
	string strProblemName = "fence6";
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
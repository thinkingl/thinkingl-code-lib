/*
ID: thinkin6
PROG: heritage
LANG: C++
*/

/** USA CO 3.4.3 :
American Heritage
Farmer John takes the heritage of his cows very seriously. He is not, however, a truly fine bookkeeper. He keeps his cow genealogies as binary trees and, instead of writing them in graphic form, he records them in the more linear `tree in-order' and `tree pre-order' notations.

Your job is to create the `tree post-order' notation of a cow's heritage after being given the in-order and pre-order notations. Each cow name is encoded as a unique letter. (You may already know that you can frequently reconstruct a tree from any two of the ordered traversals.) Obviously, the trees will have no more than 26 nodes.

Here is a graphical representation of the tree used in the sample input and output:

C
/   \
/     \
B       G
/ \     /
A   D   H
/ \
E   F
The in-order traversal of this tree prints the left sub-tree, the root, and the right sub-tree.

The pre-order traversal of this tree prints the root, the left sub-tree, and the right sub-tree.

The post-order traversal of this tree print the left sub-tree, the right sub-tree, and the root.

PROGRAM NAME: heritage

INPUT FORMAT

Line 1:	The in-order representation of a tree.
Line 2:	 The pre-order representation of that same tree.
SAMPLE INPUT (file heritage.in)

ABEDFCHG
CBADEFGH
OUTPUT FORMAT

A single line with the post-order representation of the tree.
SAMPLE OUTPUT (file heritage.out)

AEFDBHGC 
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
	string strProblemName = "heritage";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	

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
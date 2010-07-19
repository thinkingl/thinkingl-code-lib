/*
ID: thinkin6
PROG: game1
LANG: C++
*/

/** USA CO 3.3.6 :
A Game
IOI'96 - Day 1
Consider the following two-player game played with a sequence of N positive integers (2 <= N <= 100) laid onto a game board. Player 1 starts the game. The players move alternately by selecting a number from either the left or the right end of the sequence. That number is then deleted from the board, and its value is added to the score of the player who selected it. A player wins if his sum is greater than his opponents.

Write a program that implements the optimal strategy. The optimal strategy yields maximum points when playing against the "best possible" opponent. Your program must further implement an optimal strategy for player 2.

PROGRAM NAME: game1

INPUT FORMAT

Line 1:	N, the size of the board
Line 2-etc:	 N integers in the range (1..200) that are the contents of the game board, from left to right
SAMPLE INPUT (file game1.in)

6
4 7 2 9
5 2
OUTPUT FORMAT

Two space-separated integers on a line: the score of Player 1 followed by the score of Player 2.
SAMPLE OUTPUT (file game1.out)

18 11
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
	string strProblemName = "game1";
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
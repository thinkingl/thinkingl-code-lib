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
思路：好有趣的一道题目。
数列每次被取走一个数字后，都是一个新的数列。所有的新数列都是原有数列的一部分，可以用子数列的起始在原数列的位置和子数列的长度两个参数表示一个子数列。

设 BestOffensive（ begin， len ）表示做先手的时候对于一个子数列的最优解。
又 BestDefensive（ begin， len） 表示做后手的时候对于一个子数列的最优解。
又 SequenceValue（ i ） 表示数列上第 i 项的值。

则在这个子数列的基础上再次取一个数字的时候，有两种选择，左边或者右边。即第 begin 个 或 第 begin + len - 1 个。这两个选择中分值高的就是最优解。
先手在选择一个数字之后，就变成了后手，所以当选择左边的时候，得分是：
ScoreLeft = SequenceValue( begin ) + BestDefensive( begin+1, len-1 )
类似的方法分析，当选择右边的时候，得分是：
ScoreRight = SequenceValue( begin + len - 1 ) + BestDefensive( begin, len-1 )

先手自然会选择两个选择中更大的那个，也就是：
BestOffensive( begin, len ) = max( ScoreLeft, ScoreRight )

而后手呢，因为被动，在先手做选择时就注定了后手的悲惨命运，它只能被迫接受接下来的先手中较差的情况，也就是
BestDefensive( begin, len ) = min( BestOffensive（ begin, len-1 ), BestOffensive( begin+1, len-1 ) )

通过上面的公式，就有了一个动态规划（DP）迭代的过程。
迭代始于 len 为1的情况。这时先手得到唯一的数列中的数字，后手为0。

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

	typedef std::vector<int> TIntVec;
	
	// 题目中最大是100，我怕数组越界，就多搞了点，下面数组操作的时候就不用判断了。
	const int MAX_SEQUENCE_LEN = 100+100;

	int nSequenceLen;
	fin >> nSequenceLen;

	TIntVec tNumberSequence;
	for( int i=0; i<nSequenceLen; ++i )
	{
		int nNum;
		fin >> nNum;
		tNumberSequence.push_back( nNum );
	}

	// 用vector申请内存。
	TIntVec tBestOffensive( MAX_SEQUENCE_LEN * MAX_SEQUENCE_LEN, 0 );
	TIntVec tBestDefensive( MAX_SEQUENCE_LEN * MAX_SEQUENCE_LEN, 0 );

	// 二维数组指针。Array[begin][length]含义是以begin开始的length长度的数字串能得到的最优化的先手成绩。
	int (*parBestOffensive)[MAX_SEQUENCE_LEN] = ( int (*)[MAX_SEQUENCE_LEN] ) &tBestOffensive[0];
	// 最优后手成绩。
	int (*parBestDefensive)[MAX_SEQUENCE_LEN] = ( int (*)[MAX_SEQUENCE_LEN] ) &tBestDefensive[0];

	int nLen = 1;
	while( nLen <= nSequenceLen )
	{
		for( int i=0; i<nSequenceLen-nLen+1; ++i )
		{
			// 选一边时最好的先手结果就是选择的这边的值加上剩下的队列中最好的后手结果。
			int nBestPickLeft = tNumberSequence[i] + parBestDefensive[i+1][nLen-1];
			int nBestPickRight = tNumberSequence[i+nLen-1] + parBestDefensive[i][nLen-1];
			parBestOffensive[i][nLen] = max( nBestPickLeft, nBestPickRight );

			// 因为先手的人很厉害，所以最好的后手结果就是让先手拿走一个后剩下的数列情况下最坏的先手结果。
			parBestDefensive[i][nLen] = min( parBestOffensive[i][nLen-1], parBestOffensive[i+1][nLen-1]);
		}

		nLen++;
	}

	// 输出结果。
	fout << parBestOffensive[0][nSequenceLen] << " " << parBestDefensive[0][nSequenceLen] << endl;

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
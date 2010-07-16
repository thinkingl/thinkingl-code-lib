/*
ID: thinkin6
PROG: camelot
LANG: C++
*/

/** USA CO 3.3.4 :
Camelot
IOI 98
Centuries ago, King Arthur and the Knights of the Round Table used to meet every year on New Year's Day to celebrate their fellowship. In remembrance of these events, we consider a board game for one player, on which one chesspiece king and several knight pieces are placed on squares, no two knights on the same square.

This example board is the standard 8x8 array of squares:

The King can move to any adjacent square from  to  as long as it does not fall off the board:

A Knight can jump from  to , as long as it does not fall off the board:


During the play, the player can place more than one piece in the same square. The board squares are assumed big enough so that a piece is never an obstacle for any other piece to move freely.

The player's goal is to move the pieces so as to gather them all in the same square - in the minimal number of moves. To achieve this, he must move the pieces as prescribed above. Additionally, whenever the king and one or more knights are placed in the same square, the player may choose to move the king and one of the knights together from that point on, as a single knight, up to the final gathering point. Moving the knight together with the king counts as a single move.

Write a program to compute the minimum number of moves the player must perform to produce the gathering. The pieces can gather on any square, of course.

PROGRAM NAME: camelot

INPUT FORMAT

Line 1:	 Two space-separated integers: R,C, the number of rows and columns on the board. There will be no more than 26 columns and no more than 30 rows.
Line 2..end:	 The input file contains a sequence of space-separated letter/digit pairs, 1 or more per line. The first pair represents the board position of the king; subsequent pairs represent positions of knights. There might be 0 knights or the knights might fill the board. Rows are numbered starting at 1; columns are specified as upper case characters starting with `A'.
SAMPLE INPUT (file camelot.in)

8 8
D 4
A 3 A 8
H 1 H 8
The king is positioned at D4. There are four knights, positioned at A3, A8, H1, and H8.

OUTPUT FORMAT

A single line with the number of moves to aggregate the pieces.
SAMPLE OUTPUT (file camelot.out)

10
SAMPLE OUTPUT ELABORATION

They gather at B5. 
Knight 1: A3 - B5 (1 move) 
Knight 2: A8 - C7 - B5 (2 moves) 
Knight 3: H1 - G3 - F5 - D4 (picking up king) - B5 (4 moves) 
Knight 4: H8 - F7 - D6 - B5 (3 moves) 
1 + 2 + 4 + 3 = 10 moves. 
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

const int MOVE_DIRECTION_NUM = 8;

/** 用复数表示向量的棋子走法。 */
typedef std::complex<int> TIntComplex;
typedef std::vector< TIntComplex > TPosVector;

const TIntComplex END_DIRECTION = TIntComplex( 0, 0 );

/** 骑士可能走的步骤。 最终以0，0作为结束的标志。 */
static TIntComplex s_arKnightDirection[] = 
{
	TIntComplex( 1, 2 ),
	TIntComplex( 1, -2),
	TIntComplex( -1, 2 ),
	TIntComplex( -1, -2 ),
	TIntComplex( 2, 1 ),
	TIntComplex( 2, -1 ),
	TIntComplex( -2, 1 ),
	TIntComplex( -2, -1 ),
	END_DIRECTION
};

/** 国王可能的走法，最终以0，0作为结束的标志。 */
static TIntComplex s_arKingDirection[] = 
{
	TIntComplex( 1, 1 ),
	TIntComplex( 1, -1),
	TIntComplex( 0, 1 ),
	TIntComplex( 0, -1 ),
	TIntComplex( 1, 0 ),
	TIntComplex( -1, 1 ),
	TIntComplex( -1, 0 ),
	TIntComplex( -1, -1 ),
	END_DIRECTION
};

/** 用Flood Fill求一个点到其它所有点的距离。 
*	TIntComplex nPos 起始点。
*	TIntComplex* directionStep 棋子一步的走法。END_DIRECTION 结束。
*	int nRowNum, int nColNum 棋盘的高度和宽度。
*	u32 *parDistance 存放距离/棋子需要最少步数的数组。
*/
void FloodFillStep( TIntComplex nPos, TIntComplex* directionStep, int nRowNum, int nColNum, u32 *parDistance )
{
	// 开始计算点 序号nFirstIndex，坐标( nFirstCol, nFirstRow ) 到棋盘所有点上的距离。
	// 算法是逐步向外扩散的Flood fill算法。时间复杂度似乎是 点×边，也就是棋盘大小（<=780） × 可能步数(=8)

	int nFirstIndex = nPos.real() + nPos.imag() * nColNum;
	// 当前距离。
	int nCurStep = 0;
	
	TPosVector tCurStepPos;
	tCurStepPos.push_back( nPos );
	parDistance[ nFirstIndex ] = nCurStep;

	while( !tCurStepPos.empty() ) // 当前步数下有新的节点可以到达。
	{
		// 下一轮以此轮为基础，步数+1.
		nCurStep++;

		TPosVector tNextStepPos;
		for( int i=0; i<tCurStepPos.size(); ++i )
		{
			TIntComplex tNewPos = tCurStepPos[i];

			// 找从这个点可以到达的没去过的其它点。
			int nDirectionIndex = 0;
			while( s_arKnightDirection[ nDirectionIndex ] != END_DIRECTION )
			{
				TIntComplex tNextPos = tNewPos + s_arKnightDirection[ nDirectionIndex ];
				if( tNextPos.real() >= 0 && tNextPos.imag() >= 0 
					&& tNextPos.real() < nColNum && tNextPos.imag() < nRowNum ) 
				{
					// 没有越界。检查是否访问过。
					int nNextPosIndex = tNextPos.real() + tNextPos.imag() * nColNum;
					if( -1 == parDistance[ nNextPosIndex ] )
					{
						// 更新距离步数
						parDistance[ nNextPosIndex ] = nCurStep; 

						tNextStepPos.push_back( tNextPos );
					}
				}

				++nDirectionIndex;
			}

		}

				
		// 更新下轮可到达的点。
		tCurStepPos = tNextStepPos;
	}
}

int main()
{
	string strProblemName = "camelot";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nRow, nCol;
	fin >> nRow >> nCol;

	TIntComplex tKingPos;
	
	char tmpx;
	int tmpy;
	fin >> tmpx >> tmpy;
	tmpx -= 'A';
	tmpy -= 1;
	tKingPos = TIntComplex( tmpx, tmpy );

	
	TPosVector tKnightPosList;

	while( fin >> tmpx >> tmpy )
	{
		tmpx -= 'A';
		tmpy -= 1;
		tKnightPosList.push_back( TIntComplex( tmpx, tmpy ) );
	}

	// 求以骑士的走法，棋盘上两点间的距离/需要的最小步数。

	// 初始化棋盘上两点间对于骑士的距离。
	int nMatrixSize = nRow * nCol;
	//unsigned int *arKnightDistance = new unsigned int[ nMatrixSize * nMatrixSize ];
	// 因为指向数组的指针不好调试，所以改为vector。
	// stl标准中规定，vector中元素是连续的，所以不改变vector大小的情况下可以直接用指针访问其中的数据。
	typedef std::vector< u32 > TUIntVector;
	TUIntVector arKnightDistance( nMatrixSize * nMatrixSize, -1 );


	int nFirstIndex = 0;
	// 存放当前点到其他点距离的数组。
	u32 *parCurDistance = &arKnightDistance[0];
	for( int nFirstCol=0; nFirstCol<nCol; ++nFirstCol )
	{
		for( int nFirstRow=0; nFirstRow<nRow; ++ nFirstRow )
		{
			FloodFillStep( TIntComplex( nFirstCol, nFirstRow ), s_arKnightDirection, nRow, nCol, parCurDistance );
			
			// 距离存放位置偏移。
			parCurDistance += nMatrixSize;
			// 序号更新到下一个。
			++nFirstIndex;
		}
	}

	// 求国王自己走的时候，到所有点需要的步骤/距离。
	TUIntVector arKingDistance(  nMatrixSize, -1 );
	FloodFillStep( tKingPos, s_arKingDirection, nRow, nCol, &arKingDistance[0] );

	// 尝试所有的点作为集合地。
	u32 nMinAllStep = -1;	// 最终的最少步数。
	for( int nCamelot = 0; nCamelot<nMatrixSize; ++nCamelot )
	{
		// 尝试所有的点作为国王搭便车的点。
		// 让国王先走到此点，然后搭乘骑士的便车，求 国王的步数+骑士多走的步数 最小的。
		int nKnightWithKing = -1;

		// 国王可以不依靠骑士，靠自己,所以国王自己和搭骑士便车多走的步数 最大也就是国王自己走的步数。
		u32 nMinKingStepAndKnightStepPlus = arKingDistance[ nCamelot ];
		for( int i=0; i<nMatrixSize; ++i )
		{
			int nKingStep = arKingDistance[ i ];

			

			// 要所有骑士都试一下。
			for( int nKnight=0; nKnight<tKnightPosList.size(); ++nKnight )
			{
				int nKnightPosIndex = tKnightPosList[ nKnight ].real() + tKnightPosList[ nKnight ].imag() * nCol;
				u32 nNewKnightStep = arKnightDistance[ nKnightPosIndex * nMatrixSize + i ] 
										+ arKnightDistance[ i * nMatrixSize + nCamelot ];
				int nKnightStepPlus = nNewKnightStep - arKnightDistance[ nKnightPosIndex * nMatrixSize + nCamelot ];

				// 看是不是让这个骑士带国王最合适（国王花费的步数+骑士比直达多浪费的步数之和 是不是比其它的小。
				if( nMinKingStepAndKnightStepPlus > nKingStep + nKnightStepPlus )
				{
					nMinKingStepAndKnightStepPlus = nKingStep + nKnightStepPlus;

					nKnightWithKing = nKnight;
				}
			}
		}

		// 以这个点做集合地的距离就出来了。
		// 国王和国王让骑士多跑的这段加上所有骑士本来要跑的距离。
		int nCurCamelotAllStep = nMinKingStepAndKnightStepPlus;
		for( int nKnight=0; nKnight<tKnightPosList.size(); ++nKnight )
		{
			int nKnightPosIndex = tKnightPosList[ nKnight ].real() + tKnightPosList[ nKnight ].imag() * nCol;
			nCurCamelotAllStep += arKnightDistance[ nKnightPosIndex * nMatrixSize + nCamelot ];
		}

		if( nMinAllStep > nCurCamelotAllStep )// 看这个点做集合地是不是比其它的强
		{
			nMinAllStep = nCurCamelotAllStep; 
		}
	}
	
	fout << nMinAllStep << endl;

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
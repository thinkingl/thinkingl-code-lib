/*
ID: thinkin6
PROG: maze1
LANG: C++
*/

/** USA CO 2.4.3 :

Overfencing
Kolstad and Schrijvers
Farmer John went crazy and created a huge maze of fences out in a field. Happily, he left out two fence segments on the edges, and thus created two "exits" for the maze. Even more happily, the maze he created by this overfencing experience is a `perfect' maze: you can find a way out of the maze from any point inside it.

Given W (1 <= W <= 38), the width of the maze; H (1 <= H <= 100), the height of the maze; 2*H+1 lines with width 2*W+1 characters that represent the maze in a format like that shown later - then calculate the number of steps required to exit the maze from the `worst' point in the maze (the point that is `farther' from either exit even when walking optimally to the closest exit). Of course, cows walk only parallel or perpendicular to the x-y axes; they do not walk on a diagonal. Each move to a new square counts as a single unit of distance (including the move "out" of the maze.

Here's what one particular W=5, H=3 maze looks like:

+-+-+-+-+-+
|         |
+-+ +-+ + +
|     | | |
+ +-+-+ + +
| |     |  
+-+ +-+-+-+
Fenceposts appear only in odd numbered rows and and odd numbered columns (as in the example). The format should be obvious and self explanatory. Each maze has exactly two blank walls on the outside for exiting.

PROGRAM NAME: maze1

INPUT FORMAT

Line 1:	W and H, space separated
Lines 2 through 2*H+2:	2*W+1 characters that represent the maze
SAMPLE INPUT (file maze1.in)

5 3
+-+-+-+-+-+
|         |
+-+ +-+ + +
|     | | |
+ +-+-+ + +
| |     |  
+-+ +-+-+-+
OUTPUT FORMAT

A single integer on a single output line. The integer specifies the minimal number of steps that guarantee a cow can exit the maze from any possible point inside the maze.
SAMPLE OUTPUT (file maze1.out)

9
The lower left-hand corner is *nine* steps from the closest exit. 
*/


/** 
思路：
很典型的最短路径问题。
1. 演练Floyd-Warshall算法。服务器上限制内存，内存申请失败！



*/
#include <stdio.h>
#include <stdlib.h>

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


#ifdef _WIN32
#define THINKINGL 1
#endif

const int MAX_WIDTH = 38;
const int MAX_HEIGHT = 100;
const int MAX_MAZE_SIZE = MAX_WIDTH * MAX_HEIGHT;

void FloydWarshall( unsigned char (*arMaze)[MAX_MAZE_SIZE], int nVerticeNum )
{
	for ( int k=0; k<nVerticeNum; ++k )
	{
		for ( int i=0; i<nVerticeNum; ++i )
		{
			for ( int j=0; j<nVerticeNum; ++j )
			{
				if ( arMaze[ i ][ k ] != -1 && arMaze[ k ][ j ] != -1 
					&& arMaze[ i ][ k ] + arMaze[ k ][ j ] < arMaze[ i ][ j ] )
				{
					arMaze[ i ][ j ] = arMaze[ i ][ k ] + arMaze[ k ][ j ];
				}
			}
		}
	}
}

class CInput
{
public:
	CInput( ifstream * in );

	CInput& operator >>( char& cValue );
protected:
private:
	ifstream * m_in;
};

CInput::CInput( ifstream * in )
{
	m_in = in;
}

CInput& CInput::operator >>( char& cValue )
{
	noskipws( *m_in );
	while( 1 )
	{
		*m_in >> cValue;

		static int count = 0;
		cout << ( count ++ ) << " : " << int(cValue) << endl;

		if ( cValue != '\r' && cValue != '\n' )
		{
			break;
		}		 
	}
	return *this;
}

int main()
{
	string strProblemName = "maze1";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}



	enum
	{
		FenceConnected = ' ',	// can walk through.
	};

	

	unsigned char (*arMaze)[MAX_MAZE_SIZE] = new unsigned char[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
	if ( arMaze == 0 )
	{
		cout << "alloc memery fail!" << endl;
	}
	memset( arMaze, -1 , sizeof( arMaze[0][0] ) * MAX_MAZE_SIZE * MAX_MAZE_SIZE );
	for ( int i=0; i<MAX_MAZE_SIZE; ++i )
	{
		arMaze[i][i] = 0;
	}

	int nWidth, nHeight;
	fin >> nWidth >> nHeight;

	int nHorizontalFenceNum = 2 * nWidth + 1;
	int nVerticalFenceNum = 2 * nHeight + 1;

	vector<int> tExitList;

//	noskipws( fin );
	CInput input( &fin );

	for ( int h=0; h<nHeight; ++h )
	{
		for ( int w=0; w<nWidth; ++w )
		{
			int nPos = h * nWidth + w;
			int nUpPos = nPos - nWidth;
			char cLeftTop, cTop;
			input >> cLeftTop >> cTop;
			if ( h > 0 )
			{				
				unsigned int nWeight = -1;
				if ( FenceConnected == cTop )
				{					
					nWeight = 1;
				}
				arMaze[ nPos ][ nUpPos ] = nWeight;
				arMaze[ nUpPos ][ nPos ] = nWeight;
			}
			else
			{
				if ( FenceConnected == cTop )
				{
					// exits.
					tExitList.push_back( nPos );
				}
			}
		}
		char end;
		input >> end;

		for ( int w=0; w<nWidth; ++w )
		{
			int nPos = h * nWidth + w;
			int nLeftPos = nPos - 1;

			char cLeft, cCenter;
			input >> cLeft >> cCenter;
			unsigned int nWeight = -1;
			if ( w > 0 )
			{
				if ( FenceConnected == cLeft )
				{
					nWeight = 1;
				}
				arMaze[ nPos ][ nLeftPos ] = nWeight;
				arMaze[ nLeftPos ][ nPos ] = nWeight;
			}
			else
			{
				// left exits.
				if ( FenceConnected == cLeft )
				{
					tExitList.push_back( nPos );
				}
			}
		}
		input >> end;
		if ( FenceConnected == end )
		{
			int nPos = h * nWidth + nWidth - 1;
			// right exits.
			tExitList.push_back( nPos );
		}

	}

	for ( int i=0; i<nWidth; ++i )
	{
		char cLeftBottom, cBottom;
		input >> cLeftBottom >> cBottom;
		if ( cBottom == FenceConnected )
		{
			// bottom exits.
			int nPos = nWidth * nHeight + i - nWidth;
			tExitList.push_back( nPos );
		}
	}

	// use Floyd-Warshall algorithm. 
	FloydWarshall( arMaze, nWidth * nHeight );

	// find the longest path .
	int nShortestPath = 0;
	for ( int i=0; i<nWidth * nHeight; ++i )
	{
		int nToExits1 = arMaze[i][ tExitList[0] ];
		int nToExits2 = arMaze[i][ tExitList[1] ];

		int nMinToExits = min( nToExits1, nToExits2 );

		if ( nMinToExits > nShortestPath )
		{
			nShortestPath = nMinToExits;
		}
	}

	fout << nShortestPath + 1 << endl;

	delete[] arMaze;

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
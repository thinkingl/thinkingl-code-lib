/*
ID: thinkin6
PROG: ttwo
LANG: C++
*/

/** USA CO 2.4.2 :

The Tamworth Two
BIO '98 - Richard Forster
A pair of cows is loose somewhere in the forest. Farmer John is lending his expertise to their capture. Your task is to model their behavior.

The chase takes place on a 10 by 10 planar grid. Squares can be empty or they can contain:

an obstacle,
the cows (who always travel together), or
Farmer John.
The cows and Farmer John can occupy the same square (when they `meet') but neither the cows nor Farmer John can share a square with an obstacle.
Each square is
represented
as follows:

. Empty square
* Obstacle
C Cows
F Farmer
Here is a sample grid:
*...*.....
......*...
...*...*..
..........
...*.F....
*.....*...
...*......
..C......*
...*.*....
.*.*......
The cows wander around the grid in a fixed way. Each minute, they either move forward or rotate. Normally, they move one square in the direction they are facing. If there is an obstacle in the way or they would leave the board by walking `forward', then they spend the entire minute rotating 90 degrees clockwise.

Farmer John, wise in the ways of cows, moves in exactly the same way.

The farmer and the cows can be considered to move simultaneously during each minute. If the farmer and the cows pass each other while moving, they are not considered to have met. The chase ends when Farmer John and the cows occupy the same square at the end of a minute.

Read a ten-line grid that represents the initial state of the cows, Farmer John, and obstacles. Each of the ten lines contains exactly ten characters using the coding above. There is guaranteed to be only one farmer and one pair of cows. The cows and Farmer John will not initially be on the same square.

Calculate the number of minutes until the cows and Farmer John meet. Assume both the cows and farmer begin the simulation facing in the `north' direction. Print 0 if they will never meet.

PROGRAM NAME: ttwo

INPUT FORMAT

Lines 1-10:	Ten lines of ten characters each, as explained above
SAMPLE INPUT (file ttwo.in)

*...*.....
......*...
...*...*..
..........
...*.F....
*.....*...
...*......
..C......*
...*.*....
.*.*......
OUTPUT FORMAT

A single line with the integer number of minutes until Farmer John and the cows meet. Print 0 if they will never meet.
SAMPLE OUTPUT (file ttwo.out)

49


/** 
思路：
模拟。
如果出现人和牛（或者直接可以认为是两头牛）同一时间的坐标和方向在之前出现过，则此为一个永远的轮回，就可以判定他们不会再见面了。
这样的话，最坏的情况下的重复次数是 100 * 4 * 100 * 4 = 160000,时间没有问题。


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



typedef std::complex< short > TShortComplex;
class CPos : public TShortComplex
{
public:
	CPos()
	{

	}
	CPos( const TShortComplex& another ) : TShortComplex( another )
	{

	}
	CPos( short r, short i )
		: TShortComplex( r, i )
	{

	}
	bool operator < ( const CPos& another ) const;
protected:
private:
};

bool CPos::operator <( const CPos& another ) const
{
	return ( this->real() + ( this->imag() << 16 ) ) < ( another.real() + ( another.imag() << 16 ) ) ;
}
 
typedef std::pair< CPos, CPos > TPosDirectoryPair;
typedef std::pair< TPosDirectoryPair, TPosDirectoryPair > TFarmerCowPaire;
typedef std::set< TFarmerCowPaire > TPosPairSet;

enum
{
	GridObstacle = '*',
	GridEmptySquare = '.',
	GridCow = 'C',
	GridFarmer = 'F',
};

const int FARM_SIZE = 10;
char g_arFarm[FARM_SIZE][FARM_SIZE];



bool ShouldTurn( CPos pos )
{
	if ( pos.real() < 0 || pos.real() >= FARM_SIZE || pos.imag() < 0 || pos.imag() >= FARM_SIZE )
	{
		return true;
	}
	if ( GridObstacle == g_arFarm[ pos.imag() ][ pos.real() ] )
	{
		return true;
	}
	return false;
}

int main()
{
	string strProblemName = "ttwo";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	
	CPos farmerPos, cowPos;
	for ( int i=0; i < FARM_SIZE ; ++i )
	{
		for ( int k=0; k<FARM_SIZE; ++k )
		{
			char cValue;
			fin >> cValue;
			g_arFarm[i][k] = cValue;
			if ( GridFarmer == cValue )
			{
				farmerPos = CPos( k, i );
			}
			if ( GridCow == cValue )
			{
				cowPos = CPos( k, i );
			}
		}		
	}
 
 	TPosPairSet tAllTriedPos;

	// face north.
	CPos farmerDirectry( 0, -1 );

	CPos cowDirectory( 0, -1 );



	bool bFindCow = false;
	int nStep = 0;
	// 保证双方没有出现过这个位置。
	while( tAllTriedPos.find( TFarmerCowPaire(  TPosDirectoryPair( farmerPos, farmerDirectry ) , 
		TPosDirectoryPair( cowPos, cowDirectory ) ) ) == tAllTriedPos.end() )
	{
		tAllTriedPos.insert( TFarmerCowPaire(  TPosDirectoryPair( farmerPos, farmerDirectry ) , 
			TPosDirectoryPair( cowPos, cowDirectory ) ) );

		// 检测。
		if ( farmerPos == cowPos )
		{
			bFindCow = true;
			break;
		}

		// 走一步。
		CPos nextPosFarmer = farmerPos + farmerDirectry;
		CPos nextPosCow = cowPos + cowDirectory;

		// check pos .
		if ( ShouldTurn( nextPosFarmer ) )
		{
			// turn.
			farmerDirectry *= ( CPos( 0, 1 ) ); // 
		}
		else
		{
			farmerPos = farmerPos + farmerDirectry;
		}

		if ( ShouldTurn( nextPosCow ) )
		{
			// turn.
			cowDirectory *= ( CPos( 0, 1 ) );
		}
		else
		{
			cowPos = cowPos + cowDirectory;
		}

		++nStep;

	}

	if ( bFindCow )
	{
		fout << nStep << endl;
	}
	else
	{
		fout << 0 << endl;
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
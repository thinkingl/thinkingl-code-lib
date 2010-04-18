/*
ID: thinkin6
PROG: rect1
LANG: C++
*/

/** USA CO 3.1.4 :
Shaping Regions

N opaque rectangles (1 <= N <= 1000) of various colors are placed on a white sheet of paper whose size is A wide by B long. The rectangles are put with their sides parallel to the sheet's borders. All rectangles fall within the borders of the sheet so that different figures of different colors will be seen.

The coordinate system has its origin (0,0) at the sheet's lower left corner with axes parallel to the sheet's borders.

PROGRAM NAME: rect1

INPUT FORMAT

The order of the input lines dictates the order of laying down the rectangles. The first input line is a rectangle "on the bottom".
Line 1:	 A, B, and N, space separated (1 <= A,B <= 10,000)
Lines 2-N+1:	Five integers: llx, lly, urx, ury, color: the lower left coordinates and upper right coordinates of the rectangle whose color is `color' (1 <= color <= 2500) to be placed on the white sheet. The color 1 is the same color of white as the sheet upon which the rectangles are placed.
SAMPLE INPUT (file rect1.in)

20 20 3
2 2 18 18 2
0 8 19 19 3
8 0 10 19 4
INPUT EXPLANATION

Note that the rectangle delineated by 0,0 and 2,2 is two units wide and two high. Here's a schematic diagram of the input:
11111111111111111111
33333333443333333331
33333333443333333331
33333333443333333331
33333333443333333331
33333333443333333331
33333333443333333331
33333333443333333331
33333333443333333331
33333333443333333331
33333333443333333331
33333333443333333331
11222222442222222211
11222222442222222211
11222222442222222211
11222222442222222211
11222222442222222211
11222222442222222211
11111111441111111111
11111111441111111111
The '4's at 8,0 to 10,19 are only two wide, not three (i.e., the grid contains a 4 and 8,0 and a 4 and 8,1 but NOT a 4 and 8,2 since this diagram can't capture what would be shown on graph paper).
OUTPUT FORMAT

The output file should contain a list of all the colors that can be seen along with the total area of each color that can be seen (even if the regions of color are disjoint), ordered by increasing color. Do not display colors with no area.
SAMPLE OUTPUT (file rect1.out)

1 91
2 84
3 187
4 38
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

/** 长方形区域类。 */
class CRectangle
{
//	CRect ;
public:
	CRectangle();
	CRectangle( int left, int bottom, int right, int top );
	/** 求面积。 */
	int Area() const;
	/** 求交集，交集肯定也是个矩形。 */
	CRectangle IntersectionRect( const CRectangle& another );
protected:
private:
	int m_nLeft;
	int m_nBottom;
	int m_nRight;
	int m_nTop;
};

CRectangle::CRectangle()
{
	this->m_nLeft = 0;
	this->m_nRight = 0;
	this->m_nBottom = 0;
	this->m_nTop = 0;
}
CRectangle::CRectangle(int left, int bottom, int right, int top)
{
	this->m_nLeft = left;
	this->m_nBottom = bottom;
	this->m_nRight = right;
	this->m_nTop = top;
}

int CRectangle::Area() const
{
	return (m_nRight-m_nLeft)*(m_nTop-m_nBottom);
}

CRectangle CRectangle::IntersectionRect( const CRectangle& another )
{
	int nLeft = max( this->m_nLeft, another.m_nLeft );
	int nRight = min( this->m_nRight, another.m_nRight );
	int nTop = min( this->m_nTop, another.m_nTop );
	int nBottom = max( this->m_nBottom, another.m_nBottom );
	return CRectangle( nLeft, nBottom, nRight, nTop );
}

int main()
{
	string strProblemName = "rect1";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	const int MAX_COLOR_NUM = 2500+1;

	int nPaperWide, nPaperLong, nColorNum;
	fin >> nPaperWide >> nPaperLong >> nColorNum;

	CRectangle arRect[ MAX_COLOR_NUM ];
	arRect[0] = CRectangle( 0, 0, nPaperWide, nPaperLong );

	for ( int i=0; i<nColorNum; ++i )
	{
		int nLeft, nBottom, nRight, nTop, nCol;
		fin>>nLeft>>nBottom>>nRight>>nTop>>nCol;
		arRect[nCol-1] = CRectangle( nLeft, nBottom, nRight, nTop );
	}

	// 加上白色。
	nColorNum ++;

	// 求取面积。
	int arArea[MAX_COLOR_NUM] = {0};
	for ( int i=0; i<nColorNum; ++i )
	{
		arArea[i] = arRect[i].Area();
	}



	// 求取所有颜色两两之间的交集面积。
	int arAreaIntersection[ MAX_COLOR_NUM ][MAX_COLOR_NUM];
	for ( int i=0; i<nColorNum; ++i )
	{
		for ( int k=0; k<nColorNum; ++k )
		{
			CRectangle rcIntersection = arRect[i].IntersectionRect( arRect[k] );
			arAreaIntersection[i][k] = rcIntersection.Area();
		}
	}

	// 求取所有颜色的面积。
	// 颜色面积 = 原始面积 - 所有在它之上
	int arColorArea[MAX_COLOR_NUM] = {0};
	memcpy( arColorArea, arArea, sizeof( arColorArea ) );
	for ( int i=0; i<nColorNum; ++i )
	{

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
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
*	模拟过程。
*	每次放一个新的颜色的时候，将下面的方块分割成小的方块。
+--------+      +-+--+--+
|        |      | |2 |  |
|        |      + +--+  |
|  +-+   |  --> | |  |  |
|  +-+   |      |1|  |3 |
|        |      | +--+  |
|        |      | | 4|  |
+--------+      +-+--+--+
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
	typedef std::vector< CRectangle > TRectList;

	CRectangle();
	CRectangle( int left, int bottom, int right, int top, int nColor );
	/** 求面积。 */
	int Area() const;

	/** 分割一个方框。
	*	将两个方块的交集部分从目标方块中挖去，然后将剩余部分旋转切割成多个方块。
	*/
	bool CutRect( const CRectangle& another, TRectList& tRectList );

	/** 是否相交。 */
	bool HasIntersection( const CRectangle& another );

	int GetColor() const
	{
		return m_nColor;
	}

	/** 求交集，交集肯定也是个矩形。 */
//	BOOL IntersectionRect( const CRectangle& another ,CRectangle& intersection );
protected:
private:
	int m_nLeft;
	int m_nBottom;
	int m_nRight;
	int m_nTop;
	int m_nColor;
};

CRectangle::CRectangle()
{
	this->m_nLeft = 0;
	this->m_nRight = 0;
	this->m_nBottom = 0;
	this->m_nTop = 0;
	this->m_nColor = 0;
}
CRectangle::CRectangle(int left, int bottom, int right, int top, int color)
{
	this->m_nLeft = left;
	this->m_nBottom = bottom;
	this->m_nRight = right;
	this->m_nTop = top;
	this->m_nColor = color;

	if ( left == right || bottom == top )
	{
		int dfw = 0;
	}
}

int CRectangle::Area() const
{
	return (m_nRight-m_nLeft)*(m_nTop-m_nBottom);
}

bool CRectangle::HasIntersection( const CRectangle& another )
{
	// 注意！在这一题中，相等时也是不交叉的！
	bool bx = ( this->m_nLeft >= another.m_nRight ) || ( this->m_nRight <= another.m_nLeft );	// 水平没有交叉。
	bool by = ( this->m_nBottom >= another.m_nTop ) || ( this->m_nTop <= another.m_nBottom );	// 垂直没有交叉。

	if ( bx || by )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CRectangle::CutRect(const CRectangle &another, CRectangle::TRectList &tRectList)
{
	if( this->HasIntersection( another ) )
	{
		// 旋转切割。

		// 左边，从上到下。
		if ( this->m_nLeft > another.m_nLeft && this->m_nLeft < another.m_nRight )
		{
			tRectList.push_back( CRectangle( another.m_nLeft, another.m_nBottom, this->m_nLeft, another.m_nTop, another.m_nColor ) );
		}

		// 上边，从左边上面切过的地方到右头。
		if ( this->m_nTop > another.m_nBottom && this->m_nTop < another.m_nTop )
		{
			int nLeft = max( this->m_nLeft, another.m_nLeft );
			tRectList.push_back( CRectangle( nLeft, this->m_nTop, another.m_nRight, another.m_nTop, another.m_nColor ) );
		}

		// 右边，从上边切过的地方到下头。
		if ( this->m_nRight > another.m_nLeft && this->m_nRight < another.m_nRight )
		{
			int nTop = min( this->m_nTop, another.m_nTop );
			tRectList.push_back( CRectangle( this->m_nRight, another.m_nBottom, another.m_nRight, nTop, another.m_nColor) );
		}

		// 下边，从左边切过的地方到右边切过的地方。
		if ( this->m_nBottom > another.m_nBottom && this->m_nBottom < another.m_nTop )
		{
			int nLeft = max( this->m_nLeft, another.m_nLeft );
			int nRight = min( this->m_nRight, another.m_nRight );
			tRectList.push_back( CRectangle( nLeft, another.m_nBottom, nRight, this->m_nBottom, another.m_nColor ) );
		}

		return true;
	}
	else
	{
		return false;
	}
	
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

	CRectangle::TRectList tPaintList;
	tPaintList.push_back( CRectangle( 0, 0, nPaperWide, nPaperLong, 1 ) );	// 白色底色。

	// 其它颜色。
	for ( int i=0; i<nColorNum; ++i )
	{
		int nLeft, nBottom, nRight, nTop, nCol;
		fin>>nLeft>>nBottom>>nRight>>nTop>>nCol;
		tPaintList.push_back( CRectangle( nLeft, nBottom, nRight, nTop, nCol ) );
	}

	CRectangle::TRectList tResultRectangleList;
	
	// 一层层向上加层。
	for ( int i=0; i<tPaintList.size(); ++i )
	{
		CRectangle& curColorRect = tPaintList[i];
		CRectangle::TRectList tNewRectList;
		for ( int k=0; k<tResultRectangleList.size(); ++k )
		{
			// 用当前颜色切割前面的所有颜色区域。
			// 同时把当前颜色覆盖的部分从中挖去。			
			CRectangle::TRectList tList;
			if ( curColorRect.CutRect( tResultRectangleList[k], tList ) )
			{
				tNewRectList.insert( tNewRectList.end(), tList.begin(), tList.end() );	// 之前的区域被分割成几个小的区域。
			}
			else
			{
				tNewRectList.push_back( tResultRectangleList[k] );	// 之前的区域没有被分割，保持原样。
			}
			
		}
		// 更新当前所有方块列表。
		tResultRectangleList = tNewRectList;
		// 当前颜色区域在最上面，完整的。		
		tResultRectangleList.push_back( curColorRect );	// 加入所有颜色列表中。
	}

	vector<int> tArea( MAX_COLOR_NUM+1, 0 );
	for ( int i=0; i<tResultRectangleList.size(); ++i )
	{
		int nColor = tResultRectangleList[i].GetColor();
		tArea[ nColor ] += tResultRectangleList[i].Area();
	}

	for ( int i=0; i<tArea.size(); ++i )
	{
		if ( tArea[i] > 0 )
		{
			fout << i << " " << tArea[i] << endl;
		}
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
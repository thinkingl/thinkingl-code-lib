/*
ID: thinkin6
PROG: fence4
LANG: C++
*/

/** USA CO 3.4.2 :
Closed Fences
A closed fence in the plane is a set of non-crossing, connected line segments with N corners (3 < N < 200). The corners or vertices are each distinct and are listed in counter-clockwise order in an array {xi, yi}, i in (1..N). 
Every pair of adjacent vertices defines a side of the fence. Thus {xi yi xi+1 yi+1} is a side of the fence for all i in (1..N). For our purposes, N+1 = 1, so that the first and last vertices making the fence closed. 
Here is a typical closed fence and a point x,y: 
* x3,y3
x5,y5  / \
x,y *          *   /   \
/ \ /     \
/   *       \
x6,y6*   x4,y4     \
|              \
|               \
x1,y1*----------------* x2,y2

Write a program which will do the following: 
Test an ordered list of vertices {xi,yi}, i in (1..N) to see if the array is a valid fence. Find the set of fence sides that a person (with no height) who is standing in the plane at position (x,y) can "see" when looking at the fence. The location x,y may fall anywhere not on the fence. A fence side can be seen if there exists a ray that connects (x,y) and any point on the side, and the ray does not intersect any other side of the fence. A side that is parallel to the line of sight is not considered visible. In the figure, above the segments x3,y3-x4,y4; x5,y5-x6,y6; and x6-y6-x1,y1 are visible or partially visible from x,y. 
PROGRAM NAME: fence4
INPUT FORMAT
Line 1: N, the number of corners in the fence Line 2: Two space-separated integers, x and y, that are the location of the observer. Both integers will fit into 16 bits.Line 3-N+2: A pair of space-separated integers denoting the X,Y location of the corner. The pairs are given in counterclockwise order. Both integers are no larger than 1000 in magnitude.NOTE: I have added anNew test case #12 for this task. Let me know if you think it's wrong. Rob Be sure to include USACO in your mail subject! SAMPLE INPUT (file fence4.in) 
13
5 5
0 0
7 0
5 2
7 5
5 7
3 5
4 9
1 8
2 5
0 9
-2 7
0 3
-3 1 

OUTPUT FORMAT
If the sequence is not a valid fence, the output is a single line containing the word "NOFENCE". 
Otherwise, the output is a listing of visible fence segments, one per line, shown as four space-separated integers that represent the two corners. Express the points in the segment by showing first the point that is earlier in the input, then the point that is later. Sort the segments for output by examining the last point and showing first those points that are earlier in the input. Use the same rule on the first of the two points in case of ties. 
SAMPLE OUTPUT (file fence4.out)
7
0 0 7 0
5 2 7 5
7 5 5 7
5 7 3 5
-2 7 0 3
0 0 -3 1
0 3 -3 1

*/

/** 
思路：用角度范围解决这个问题。
寻找完全没有被挡住的边，把这个边映射出的角度范围加到已遮挡角度范围内。
然后去掉这条边，寻找下个完全没有被挡住的边。

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

/** 取符号。 +by thinkingl@2010/11/25 */
template<class _Type>
int sgn( _Type num )
{
	if ( num > 0.00000001 )
	{
		return 1;
	}
	if ( num < -0.00000001 )
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

/** 点。 */
template<class _Type>
class CPoint
{
public:
	_Type x;
	_Type y;

	bool operator < ( const CPoint<_Type>& another ) const;

	double GetDistance( const CPoint<_Type>& another ) const;
protected:
private:
};

template<class _Type>
bool CPoint<_Type>::operator<( const CPoint<_Type>& another ) const
{
	if ( y != another.y )
	{
		return y < another.y;
	}
	else 
	{
		return x < another.x;
	}
}

template<class _Type>
double CPoint<_Type>::GetDistance( const CPoint<_Type>& another )const
{
	double yl = another.y - y;
	double xl = another.x - x;
	return sqrt( yl * yl + xl * xl );
}

/** 直线。 */
template<class _Type>
class CLine
{
public:
	/** 两点确定一条直线。 */
	CLine( const CPoint<_Type>& point1, const CPoint<_Type>& point2 );

	/** 两个点是否在直线的同一侧。 */
	bool IsSameSide( const CPoint<_Type>& point1, const CPoint<_Type>& point2 ) const;

	/** 点在直线的哪一侧或在直线上。 */
	int GetSide( const CPoint<_Type>& point ) const;

private:
	/** 方程系数。 ax+by+c = 0*/
	_Type m_a;
	_Type m_b;
	_Type m_c;
};

template<class _Type>
CLine<_Type>::CLine( const CPoint<_Type>& point1, const CPoint<_Type>& point2 )
{
	// 过两点的方程可以表示为： (y0 - y1) x -(x0 - x1) y + x0y1-x1y0 = 0
	m_a = point1.y - point2.y;
	m_b = -( point1.x - point2.x );
	m_c = point1.x * point2.y - point2.x * point1.y;
}

template<class _Type>
bool CLine<_Type>::IsSameSide( const CPoint<_Type>& point1, const CPoint<_Type>& point2 )const
{
	// 直线把一个平面分成2部分。
	// 一部分 ax+by+c>0 , 另一部分 ax+by+c<0. 直线上 = 0.
	bool bSameSide = ( this->GetSide( point1 ) == this->GetSide( point2 ) );
	return bSameSide;
}

template<class _Type>
int CLine<_Type>::GetSide( const CPoint<_Type>& point )const
{
	_Type p1 = m_a * point.x + m_b * point.y + m_c;
	return sgn( p1 );
}

/** 线段。 */
template<class _Type>
class CLineSegment
{
public:
	/** 两个端点确定一条线段 */
	CLineSegment( const CPoint<_Type>& point1, const CPoint<_Type>& point2 );


	/** 两线段是否绝对交叉，一个端点在上面的不算。 */
	bool IsIntersectAbsolutely( const CLineSegment<_Type>& another )const;

	/** 与直线是否绝对交叉，一个端点在上面的不算。 */
	bool IsIntersectAbsolutely( const CLine<_Type>& anotherLine )const;

	/** 两线段是否有交点。 */
	bool IsIntersect( const CLineSegment<_Type>& another )const;

	/** 与直线是否有交点。 */
	bool IsIntersect( const CLine<_Type>& anotherLine )const;

	/** 求交点。 */
	bool GetIntersection( const CLineSegment< _Type >& another, CPoint< double >& intersection ) const;

private:
	CPoint<_Type> m_p1;
	CPoint<_Type> m_p2;
};

template<class _Type>
 CLineSegment<_Type>::CLineSegment( const CPoint<_Type>& point1, const CPoint<_Type>& point2 )
 {
 	this->m_p1 = point1;
 	this->m_p2 = point2;
 }


template<class _Type>
bool CLineSegment<_Type>::IsIntersect( const CLineSegment<_Type>& another )const
{
	// In two dimensions, two line segments AB and CD intersect if and only if A and B are on opposite sides of 
	// the line CD and C and D are on opposite sides of line AB. 
	CLine<_Type> lineA( m_p1, m_p2 );
	CLine<_Type> lineB( another.m_p1, another.m_p2 );
	if ( lineA.IsSameSide( another.m_p1, another.m_p2 ) )
	{
		return false;
	}
	if ( lineB.IsSameSide( m_p1, m_p2 ) )
	{
		return false;
	}
	return true;
}

template<class _Type>
bool CLineSegment<_Type>::IsIntersect( const CLine<_Type>& anotherLine )const
{
	// 只要判断两点是否在直线的同一侧。
	bool bSameSide = anotherLine.IsSameSide( this->m_p1, this->m_p2 );
	return !bSameSide;
}

template<class _Type>
bool CLineSegment<_Type>::IsIntersectAbsolutely( const CLine<_Type>& anotherLine ) const
{
	// 必须在直线的两侧才行。
	bool bAbIntersect = ( anotherLine.GetSide( m_p1 ) != 0 &&
		anotherLine.GetSide( m_p1 ) == -anotherLine.GetSide( m_p2 ) );
	return bAbIntersect;
}

template<class _Type>
bool CLineSegment<_Type>::IsIntersectAbsolutely( const CLineSegment<_Type>& another ) const
{
	CLine<_Type> lineA( m_p1, m_p2 );
	CLine<_Type> lineB( another.m_p1, another.m_p2 );
	
	bool bAbIntersect = ( another.IsIntersectAbsolutely( lineA ) && this->IsIntersectAbsolutely( lineB ) );
	
	return bAbIntersect;
}

template< class _Type>
bool CLineSegment<_Type>::GetIntersection( const CLineSegment< _Type >& another, CPoint< double >& intersection ) const
{
	if ( !IsIntersect( another ) )
	{
		return false;
	}

	// 求交点。
	/*For the lines AB and CD in two dimensions, the most straight-forward way to calculate the intersection of them is to solve the system of two equations and two unknowns:

	Ax + (Bx - Ax)i = Cx + (Dx - Cx) j
	Ay + (By - Ay)i = Cy + (Dy - Cy) j
	The point of intersection is:
	(Ax + (Bx - Ax) i, Ay + (By - Ay) i)*/
	double Ax = m_p1.x;
	double Ay = m_p1.y;
	double Bx = m_p2.x;
	double By = m_p2.y;
	double Cx = another.m_p1.x;
	double Cy = another.m_p1.y;
	double Dx = another.m_p2.x;
	double Dy = another.m_p2.y;

	double i = double( (Dy-Cy)*(Cx-Ax) + (Dx-Cx)*(Ay-Cy) )/( (Bx-Ax)*(Dy-Cy) - (By-Ay)*(Dx-Cx) );

	intersection.x = Ax + i * ( Bx-Ax );
	intersection.y = Ay + i * ( By-Ay );

	return true;

}

typedef CPoint< double > TDBPoint;
typedef CLineSegment< double > TDBLineSegment;
typedef std::vector< TDBPoint > TPosVector;
typedef std::vector< TDBLineSegment > TLineSegmentVector;
typedef std::set< int > TIndexSet;


void PrintFence( ofstream& fs, const TDBPoint& pt1, const TDBPoint& pt2 )
{
	fs << (int)pt1.x << " " << (int)pt1.y << " " << (int)pt2.x << " " << (int)pt2.y << endl;

}

int main()
{
	string strProblemName = "fence4";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	

	int nFenceNum;
	fin >> nFenceNum;
	TDBPoint observer;
	fin >> observer.x >> observer.y;

	TPosVector cornerList;
	TLineSegmentVector fenceList;

	typedef std::set< double > TDoubleSet;
	TDoubleSet allObserverAngle;	// 所有的观察视角。

	for ( int i=0; i<nFenceNum; ++i )
	{
		TDBPoint corner;
		fin >> corner.x >> corner.y;
		cornerList.push_back( corner );

		if ( i > 0 )
		{
			fenceList.push_back( TDBLineSegment( cornerList[i-1], corner ) );
		}

		// 求这个顶点的视角。
		double dbAng;
		double dbtg = double( corner.y - observer.y )/( corner.x - observer.x );
// 		if ( dbtg > 100 )
// 		{
// 			double dbctan = double( corner.x - observer.x ) / ( corner.y - observer.y );
// 			dbAng = tan
// 		}
// 		else
		{
			dbAng = atan( dbtg );
		}

		// 将角度修正到0-2pi
		#define M_PI       3.14159265358979323846
		if ( corner.x - observer.x < 0  )
		{
			if ( corner.y - observer.y > 0 )
			{
				dbAng += M_PI;
			}
			else
			{
				dbAng -= M_PI;
			}			
		}
		if ( dbAng < 0 )
		{
			dbAng += 2 * M_PI;
		}
		allObserverAngle.insert( dbAng );
	}
	fenceList.push_back( TDBLineSegment( cornerList[ nFenceNum -1 ], cornerList[0] ) );

	// 是否有篱笆互相交叉。
	bool bFenceValid = true;
	for ( int i=0; i<nFenceNum; ++i )
	{
		for ( int k=0; k<nFenceNum; ++k )
		{
			if ( i!=k )
			{
				if ( fenceList[i].IsIntersectAbsolutely( fenceList[k] ) )
				{
					bFenceValid = false;
					break;
				}
			}
		}
	}

	if ( bFenceValid )
	{
		// 所有能看到的篱笆的index。
		TIndexSet allFenceCanSaw;

		TDoubleSet::iterator iter = allObserverAngle.begin();
		TDoubleSet::iterator iterPre = iter;
		++ iter;
		bool bFinish = false;
		while ( !bFinish )
		{
			// 最后一个，用第一个和最后一个端点。
			if ( iter == allObserverAngle.end() )
			{
				iter = allObserverAngle.begin();
				bFinish = true;
			}

			// 远端点的位置。因为这道题目的范围是200内。
			const int REMOTE_LEN = 200000;

			// 取锐角部分的角平分线。
			double dbAngle = ( *iter + *iterPre );
			dbAngle /= 2;
			// 一个角在在第四象限
			if ( max( *iterPre, *iter ) > 2 * M_PI * 3 / 4  )
			{
				// 另一个角在第一或第四象限。
				if ( min( *iterPre, *iter ) < M_PI / 2 )
				{
					dbAngle -= M_PI;	// 这时要减pi才能保证得到的是锐角的角平分线。
				}				
			}

			// 一个在第四象限，另一个在第一象限。


			// 看在中间角度这条射线和那条边最先相交。
			
			TDBPoint remotePoint;
			remotePoint.x = observer.x + REMOTE_LEN * cos( dbAngle );
			remotePoint.y = observer.y + REMOTE_LEN * sin( dbAngle );


			// 看这个线段同其它边的交点。找最近的，最近的这条边就是能看到的边。
			TDBLineSegment viewLineSegment( observer, remotePoint );
			int fenceIndexCanSaw = -1;
			double dbMinDistance = REMOTE_LEN;
			for ( int i=0; i<nFenceNum; ++i )
			{
				TDBPoint intersection;
				if ( viewLineSegment.GetIntersection( fenceList[i], intersection ) )
				{
					// 求距离。
					double distance = observer.GetDistance( intersection );

					if ( distance < dbMinDistance )
					{
						dbMinDistance = distance;
						fenceIndexCanSaw = i;
					}
				}
			}

			if ( -1 != fenceIndexCanSaw )
			{
				allFenceCanSaw.insert( fenceIndexCanSaw );
			}
			else
			{
				int fwe = 232;
			}

			iterPre = iter;
			++iter;
		}

		fout << allFenceCanSaw.size() << endl;
		bool bHaveLastFence = false;
		bool bHave2thRevFence = false;
		for ( TIndexSet::iterator iter = allFenceCanSaw.begin(); iter != allFenceCanSaw.end(); ++iter )
		{
			int fenceIndex = *iter;
			if ( fenceIndex < nFenceNum -2 )
			{
				// 普通输出。
				PrintFence( fout, cornerList[ fenceIndex ] , cornerList[ fenceIndex+1 ] );
			}
			else if( fenceIndex == nFenceNum -1 )
			{
				bHaveLastFence = true;
			}
			else if( fenceIndex == nFenceNum -2 )
			{
				bHave2thRevFence = true;
			}
		}

		if ( bHaveLastFence )
		{
			PrintFence( fout, cornerList[0], cornerList[ nFenceNum-1 ] );
		}
		if ( bHave2thRevFence )
		{
			PrintFence( fout, cornerList[ nFenceNum -2 ], cornerList[ nFenceNum -1 ] );
		}
	}
	else
	{
		fout << "NOFENCE" << endl;
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
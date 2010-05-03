/*
ID: thinkin6
PROG: msquare
LANG: C++
*/

/** USA CO 3.2.5 :
Magic Squares
IOI'96
Following the success of the magic cube, Mr. Rubik invented its planar version, called magic squares. This is a sheet composed of 8 equal-sized squares:

1	2	3	4
8	7	6	5
In this task we consider the version where each square has a different color. Colors are denoted by the first 8 positive integers. A sheet configuration is given by the sequence of colors obtained by reading the colors of the squares starting at the upper left corner and going in clockwise direction. For instance, the configuration of Figure 3 is given by the sequence (1,2,3,4,5,6,7,8). This configuration is the initial configuration.

Three basic transformations, identified by the letters `A', `B' and `C', can be applied to a sheet:

'A': exchange the top and bottom row,
'B': single right circular shifting of the rectangle,
'C': single clockwise rotation of the middle four squares.
Below is a demonstration of applying the transformations to the initial squares given above:

A:	
8	7	6	5
1	2	3	4
B:	
4	1	2	3
5	8	7	6
C:	
1	7	2	4
8	6	3	5
All possible configurations are available using the three basic transformations.

You are to write a program that computes a minimal sequence of basic transformations that transforms the initial configuration above to a specific target configuration.

PROGRAM NAME: msquare

INPUT FORMAT

A single line with eight space-separated integers (a permutation of (1..8)) that are the target configuration.

SAMPLE INPUT (file msquare.in)

2 6 8 4 5 7 3 1 
OUTPUT FORMAT

Line 1:	A single integer that is the length of the shortest transformation sequence.
Line 2:	 The lexically earliest string of transformations expressed as a string of characters, 60 per line except possibly the last line.
SAMPLE OUTPUT (file msquare.out)

7
BCABCCB
*/

/** 
˼·��
*	��Ϊһ���ͼ�����,��Ϊ��vector��squareЧ�ʵ�,���Ըĳ���u64.��ʵû�б�Ҫ.
ģ��+�����������+ȥ��.
��Ϊֻ��8������,�������Ŀ���״ֻ̬��8! = 40302��,ֻҪ��square״̬ȥ��,����������.
��Ϊһ���ͼ�����(��ȥ��set�в���Ļ÷�����,���û��ȥ�ص�Ч��),������ΪЧ����Ҫ�Ż�,��֮ǰvector�洢square�ĳ���u64��ʾ.һ��8λ(char)��ʾһ������,64λ�����ñ�ʾ��Ŀ��8�������һ��square.����չ��û����.
�����������,�õ��ĵ�һ�����������ĿҪ���.
3�ֱ任���ü���,ֱ����Ԥ�������.
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

const int SQUARE_SIZE = 8;
const int TRANSFORMATION_NUM  = 3;

typedef std::vector<int> TIntVector;
class CMagicSquare
{
public:
	CMagicSquare();
	CMagicSquare( char * );
	bool operator < ( const CMagicSquare& another ) const;
	/** �任. */
	void Transform( char transformName, CMagicSquare& result ) const;
	bool operator == ( const CMagicSquare& another ) const;
private:
	u64 m_nSquare;
};

CMagicSquare::CMagicSquare()
{
	m_nSquare = 0;
}
CMagicSquare::CMagicSquare( char *square )
{
	m_nSquare = 0;
	m_nSquare = *((u64*)square);
}

bool CMagicSquare::operator < ( const CMagicSquare& another ) const
{
	return this->m_nSquare < another.m_nSquare;
}

bool CMagicSquare::operator == ( const CMagicSquare& another ) const
{
	return this->m_nSquare == another.m_nSquare;
}

void CMagicSquare::Transform( char transformName, CMagicSquare& result ) const
{
	// �仯��.
	static int s_arTransformTable[TRANSFORMATION_NUM][SQUARE_SIZE] =
	{
		{	7,6,5,4,3,2,1,0	},
		{	3,0,1,2,5,6,7,4	},
		{	0,6,1,3,4,2,5,7	}
	};

	u64 nNewSquare=0;
	char *pSquare = (char*)&nNewSquare;
	int nTransform = transformName - 'A';
	for ( int i=0; i<SQUARE_SIZE; ++i )
	{
		int nIndex = s_arTransformTable[nTransform][i];
		pSquare[i] = ((char*)&m_nSquare)[nIndex];
	}	
	result.m_nSquare = nNewSquare;
}

typedef std::set< CMagicSquare > TMagicSquareSet;



int main()
{
	string strProblemName = "msquare";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	// Ŀ��.
	char arGoal[SQUARE_SIZE];
	for ( int i=0; i<SQUARE_SIZE; ++i )
	{
		fin >> arGoal[i];
	}

	// Ŀ��.
	CMagicSquare tGoalSquare( arGoal );

	// �Ѿ������Ļ÷�����.
	TMagicSquareSet tCheckedSquareSet;

	// ԭʼ�÷�.
	char arInit[SQUARE_SIZE];
	for ( int i=0; i<SQUARE_SIZE; ++i )
	{
		arInit[i] = i + '0' + 1;
	}

	// ������ȱ仯�÷�,����Ŀ��÷�.
	typedef std::pair< CMagicSquare, string > TMagicSquareAndTransform;
	typedef std::queue< TMagicSquareAndTransform > TMagicSquareQueue;
	TMagicSquareQueue tWaitTransform;
	tWaitTransform.push( TMagicSquareAndTransform( CMagicSquare(arInit), "" ) );

	string strTransform; // �仯�б�.
	
	while ( !tWaitTransform.empty() )
	{
		// �ȴ��б���ȡ��һ��.
		TMagicSquareAndTransform sqandTrans = tWaitTransform.front();
		CMagicSquare& sq = sqandTrans.first;
		tWaitTransform.pop();
		tCheckedSquareSet.insert( sq );
		// �Ƿ���Ŀ��.
		if ( sq == tGoalSquare )
		{
			// �ҵ���!
			strTransform = sqandTrans.second;
			break;
		}
		// �仯.
		for ( int i=0; i<TRANSFORMATION_NUM; ++i )
		{
			// �任.
			char transName = 'A' + i;
			CMagicSquare tNextSquare;
			sq.Transform( transName, tNextSquare );
			// ���֮ǰû�д����,����ȴ�����б�.
			if ( tCheckedSquareSet.find( tNextSquare ) == tCheckedSquareSet.end() )
			{
				tCheckedSquareSet.insert( tNextSquare );

				string strTrans = sqandTrans.second;
				strTrans.push_back( transName );
				tWaitTransform.push( TMagicSquareAndTransform( tNextSquare, strTrans )  );
			}
		}


	}

	// ������.�ַ���ÿ�����60��.
	fout << strTransform.size() << endl;
	for ( int i=0; i<strTransform.size(); ++i )
	{
		if ( i%60 == 0 && i>0 )
		{
			fout << endl;
		}
		fout << strTransform[i];
	}

	fout << endl;

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
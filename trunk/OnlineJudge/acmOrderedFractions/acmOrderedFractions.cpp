/*
ID: thinkin6
PROG: frac1
LANG: C++
*/

/** USA CO 2.1.4 problem 64 :
Ordered Fractions
Consider the set of all reduced fractions between 0 and 1 inclusive with denominators less than or equal to N.

Here is the set when N = 5:

0/1 1/5 1/4 1/3 2/5 1/2 3/5 2/3 3/4 4/5 1/1
Write a program that, given an integer N between 1 and 160 inclusive, prints the fractions in order of increasing magnitude.

PROGRAM NAME: frac1

INPUT FORMAT

One line with a single integer N.
SAMPLE INPUT (file frac1.in)

5
OUTPUT FORMAT

One fraction per line, sorted in order of magnitude.
SAMPLE OUTPUT (file frac1.out)

0/1
1/5
1/4
1/3
2/5
1/2
3/5
2/3
3/4
4/5
1/1
*/

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

typedef std::pair< int, int > TFraction;
//	typedef float TValue;

class CValue
{
public:
	bool operator < ( const CValue& another ) const
	{
		// the minimum of the difference is 1/160*160 = 0.0000390625.
		return ( another.m_fValue - this->m_fValue > 0.0000001 );
	}

	CValue( float fValue )
	{
		(*this) = fValue;
	}

	CValue& operator = ( float fValue )
	{
		m_fValue = fValue;
		return *this;
	}

private:
	float m_fValue;
};

typedef CValue TValue;

typedef std::map< TValue, TFraction > TFractionsList;

int main()
{
	string strProblemName = "frac1";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nNum;
	fin >> nNum;



	TFractionsList tAllFractions;
	for ( int nNumerator=1; nNumerator<nNum; ++nNumerator )
	{
		for ( int nDenominator = nNumerator+1;  nDenominator<=nNum; ++nDenominator )
		{
			float fValue = float( nNumerator ) / nDenominator;

			if ( tAllFractions.find( fValue ) == tAllFractions.end() )
			{
				tAllFractions[ fValue ] = TFraction( nNumerator, nDenominator );
				cout << "Insert value: " << nNumerator << " / " << nDenominator << endl;
			}
			
		}
	}

	// output the result:
	fout << "0/1" << endl;

	for ( TFractionsList::iterator iter = tAllFractions.begin(); iter != tAllFractions.end(); ++iter )
	{
		fout << iter->second.first << "/" << iter->second.second << endl;
	}

	fout << "1/1" << endl;
	

#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	system( "pause" );
#endif



	return 0;
}
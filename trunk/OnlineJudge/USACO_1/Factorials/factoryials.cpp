/*
ID: thinkin6
PROG: fact4
LANG: C++
*/

/** USA CO 3.2.1 :
Factorials
The factorial of an integer N, written N!, is the product of all the integers from 1 through N inclusive. The factorial quickly becomes very large: 13! is too large to store in a 32-bit integer on most computers, and 70! is too large for most floating-point variables. Your task is to find the rightmost non-zero digit of n!. For example, 5! = 1 * 2 * 3 * 4 * 5 = 120, so the rightmost non-zero digit of 5! is 2. Likewise, 7! = 1 * 2 * 3 * 4 * 5 * 6 * 7 = 5040, so the rightmost non-zero digit of 7! is 4.

PROGRAM NAME: fact4

INPUT FORMAT

A single positive integer N no larger than 4,220.
SAMPLE INPUT (file fact4.in)

7
OUTPUT FORMAT

A single line containing but a single digit: the right most non-zero digit of N! .
SAMPLE OUTPUT (file fact4.out)

4
*/

/** 
思路：
*	实现大数乘法.
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

class CHugeInt
{
public:
	// CHugeInt();

	CHugeInt( int nNum );

	/** 赋值. */
	CHugeInt& operator = ( int nNum );

	/** 获取10进制形式的字符串表示. */
	string ToDecString() const;

	/** 乘法. */
	CHugeInt& operator *=( const CHugeInt& another  );
private:
	
	typedef std::vector<char> TCharVector;
	TCharVector m_tAllNum;
};

CHugeInt::CHugeInt( int nNum )
{
	*this = nNum;
}
CHugeInt& CHugeInt::operator = ( int nNum )
{
	while ( nNum > 0 )
	{
		m_tAllNum.push_back( nNum % 10 );
		nNum /= 10;
	}
	return *this;
}

string CHugeInt::ToDecString() const
{
	int nLen = m_tAllNum.size();
	string strNum;
	for ( int i=0; i<nLen; ++i )
	{
		strNum.push_back( m_tAllNum[nLen - i - 1] + '0' );
	}
	return strNum;
}

CHugeInt& CHugeInt::operator *=( const CHugeInt& another )
{
	typedef std::vector< TCharVector > TCharVecVec;
	TCharVecVec tAllPruduct;
	for ( int k= 0; k<another.m_tAllNum.size(); ++k )
	{
		int nNum = another.m_tAllNum[k];

		int nAdd = 0; // 进位.
		TCharVector tcv( m_tAllNum.size(), 0 );
		for ( int i=0; i<m_tAllNum.size(); ++i )
		{

			int nProduct = m_tAllNum[i] * nNum;
			nProduct += nAdd;

			nAdd = nProduct / 10;

			tcv[i] = ( nProduct % 10 ); // 进位后的余数.
		}

		if ( nAdd )
		{
			tcv.push_back( nAdd ); // 最高位进位.
		}

		tAllPruduct.push_back( tcv );
	}

	// 把它们加起来.
	m_tAllNum = tAllPruduct[0];
	for ( int i=1; i<tAllPruduct.size(); ++i )
	{
		int nAdd = 0; // 进位.
		for ( int k=0; k<tAllPruduct[i].size(); ++k )
		{
			int nIndex = k + i;
			while ( m_tAllNum.size() <= nIndex )
			{
				m_tAllNum.push_back( 0 ); // 不够长了.
			}
			int nSum = m_tAllNum[ nIndex ] + tAllPruduct[i][k];
			nSum += nAdd;

			m_tAllNum[ nIndex ] = nSum % 10;
			nAdd = nSum / 10;
			
		}
	}

	// 优化,去掉末尾在题目中没用的0.
	// 数字右边,是数列的地位端.
	// 数字高位的那些数字用处也不大,只保留100位.
	for ( int i=0; i<m_tAllNum.size(); ++i )
	{
		if ( m_tAllNum[i] != 0 )
		{
			TCharVector tTemp;
			tTemp.assign( m_tAllNum.begin() + i, m_tAllNum.begin() + min( (int)m_tAllNum.size(), i + 100 ) );
			m_tAllNum = tTemp;
			break;
		}
	}

	
	return *this;
}

int main()
{
	string strProblemName = "fact4";
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

	CHugeInt hi = 1;

	for ( int i=1; i<=nNum; ++i )
	{
		hi *= i;
	}

	string strNum = hi.ToDecString();
	cout << strNum << endl;

	int nLen = strNum.size();
	for ( int i=0; i<strNum.size(); ++i )
	{
		if ( strNum[ nLen - i-1 ] != '0' )
		{
			fout << strNum[ nLen - i-1 ] << endl;
			break;
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
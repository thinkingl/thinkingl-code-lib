/*
ID: thinkin6
PROG: contact
LANG: C++
*/

/** USA CO 3.1.5 :
Contact
IOI'98
The cows have developed a new interest in scanning the universe outside their farm with radiotelescopes. Recently, they noticed a very curious microwave pulsing emission sent right from the centre of the galaxy. They wish to know if the emission is transmitted by some extraterrestrial form of intelligent life or if it is nothing but the usual heartbeat of the stars.

Help the cows to find the Truth by providing a tool to analyze bit patterns in the files they record. They are seeking bit patterns of length A through B inclusive (1 <= A <= B <= 12) that repeat themselves most often in each day's data file. They are looking for the patterns that repeat themselves most often. An input limit tells how many of the most frequent patterns to output.

Pattern occurrences may overlap, and only patterns that occur at least once are taken into account.

PROGRAM NAME: contact

INPUT FORMAT

Line 1:	Three space-separated integers: A, B, N; (1 <= N < 50)
Lines 2 and beyond:	A sequence of as many as 200,000 characters, all 0 or 1; the characters are presented 80 per line, except potentially the last line.
SAMPLE INPUT (file contact.in)

2 4 10
01010010010001000111101100001010011001111000010010011110010000000

In this example, pattern 100 occurs 12 times, and pattern 1000 occurs 5 times. The most frequent pattern is 00, with 23 occurrences.

OUTPUT FORMAT

Lines that list the N highest frequencies (in descending order of frequency) along with the patterns that occur in those frequencies. Order those patterns by shortest-to-longest and increasing binary number for those of the same frequency. If fewer than N highest frequencies are available, print only those that are.

Print the frequency alone by itself on a line. Then print the actual patterns space separated, six to a line (unless fewer than six remain).

SAMPLE OUTPUT (file contact.out)

23
00
15
01 10
12
100
11
11 000 001
10
010
8
0100
7
0010 1001
6
111 0000
5
011 110 1000
4
0001 0011 1100
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

class CMyString 
{
public:
	CMyString( )
	{

	}

	CMyString( const char * sz ) 
	{
		m_str = sz;
	}


	bool operator < ( const CMyString& another ) const
	{
		if ( this->m_str.size() == another.m_str.size() )
		{
			return this->m_str < another.m_str;
		}
		else
		{
			return this->m_str.size() < another.m_str.size();
		}
	}

	const string& GetStr() const
	{
		return m_str;
	}

	string m_str;
};
int main()
{
	string strProblemName = "contact";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nMinLen, nMaxLen, nNeededNum;
	fin >> nMinLen >> nMaxLen >> nNeededNum;

	const int MAX_CHARACTERS_NUM = 200000;
	char arCharacterSeq[ MAX_CHARACTERS_NUM+1 ] = { 0 };

	int nSeqLen = 0;
	while( !fin.eof() )
	{
		fin >> arCharacterSeq[ nSeqLen ];
		++nSeqLen;
	}

	const int MAX_PATTERN_LEN = 12;
	const int MAX_SEQUENCE_NUM = 1 << MAX_PATTERN_LEN;
	int nFrequence[ MAX_SEQUENCE_NUM+1 ][ MAX_PATTERN_LEN+1 ];	
	memset( nFrequence, 0, sizeof( nFrequence ) );

	for ( int nParternLen = nMinLen; nParternLen <= nMaxLen; ++nParternLen )
	{
		if ( arCharacterSeq[ nParternLen - 1 ] == 0 )
		{
			break;
		}

		int nNum = 0;
		for ( int i=0; i<nParternLen; ++i )
		{
			if ( arCharacterSeq[i] == '1' )
			{
				nNum += ( 1 << ( nParternLen - i - 1 ) );
			}
		}


		int nCursor = nParternLen;
		
		int nMask = 0;
		for ( int i=0; i<nParternLen; ++i )
		{
			nMask += 1 << i;
		}
		// 一直处理到队列末尾。
		while( 1 )
		{
			// 为对应数字计数。
			nFrequence[ nNum ][nParternLen] ++;

			if ( arCharacterSeq[ nCursor ] == 0 )
			{
				break;	// 到了末尾。
			}

			// 求nCursor对应数字为结尾的字串表示的数字。
			nNum <<= 1;
			nNum += ( arCharacterSeq[ nCursor ] - '0' );
			nNum &= nMask;

			// 下一个数字。
			++ nCursor;
		}
	}

	// 整理结果，排序。
	typedef std::set< CMyString > TStringSet;
	typedef std::map< int, TStringSet > TNumCountTable;

	TNumCountTable tNumTable;

	for ( int i=0; i<=nMaxLen; ++i )
	{
		for ( int k=0; k<MAX_SEQUENCE_NUM; ++k )
		{
			int nNum = nFrequence[k][i];
			if ( nNum > 0 )
			{
				char szNum[ MAX_PATTERN_LEN + 1 ] = { 0 };
				for ( int j=0; j<i; ++j )
				{
					szNum[j] = '0' + ( 1 & ( k >> ( i - j - 1 ) ) );
				}
				tNumTable[ nNum ].insert( szNum );
			}
		}
	}

	// 输出结果。

	int nCount = 0;
	TNumCountTable::iterator iter = tNumTable.end();
	while( nCount < nNeededNum && iter != tNumTable.begin() )
	{
		--iter;

		fout << iter->first << endl;
		
		int nTmpCount = iter->second.size();
		int nToSix = 0;
		for ( TStringSet::iterator iterStr = iter->second.begin(); 
			iterStr != iter->second.end(); ++iterStr, --nTmpCount )
		{
			++ nToSix;

			fout << iterStr->GetStr() ;
			if ( nTmpCount > 1 && nToSix < 6 ) // 每行最多6个。
			{
				fout << " ";
			}
			else
			{
				fout << endl;
				nToSix = 0;
			}
		}

		nCount ++;
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
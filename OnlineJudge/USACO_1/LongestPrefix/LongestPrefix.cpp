/*
ID: thinkin6
PROG: prefix
LANG: C++
*/

/** USA CO 2.3.1 :

Longest Prefix
IOI'96
The structure of some biological objects is represented by the sequence of their constituents denoted by uppercase letters. Biologists are interested in decomposing a long sequence into shorter ones called primitives.

We say that a sequence S can be composed from a given set of primitives P if there is a some sequence of (possibly repeated) primitives from the set whose concatenation equals S. Not necessarily all primitives need be present. For instance the sequence ABABACABAABcan be composed from the set of primitives

{A, AB, BA, CA, BBC}
The first K characters of S are the prefix of S with length K. Write a program which accepts as input a set of primitives and a sequence of constituents and then computes the length of the longest prefix that can be composed from primitives.

PROGRAM NAME: prefix

INPUT FORMAT

First, the input file contains the list (length 1..200) of primitives (length 1..10) expressed as a series of space-separated strings of upper-case characters on one or more lines. The list of primitives is terminated by a line that contains nothing more than a period (`.'). No primitive appears twice in the list. Then, the input file contains a sequence S (length 1..200,000) expressed as one or more lines, none of which exceed 76 letters in length. The "newlines" are not part of the string S.
SAMPLE INPUT (file prefix.in)

A AB BA CA BBC
.
ABABACABAABC
OUTPUT FORMAT

A single line containing an integer that is the length of the longest prefix that can be composed from the set P.
SAMPLE OUTPUT (file prefix.out)

11
*/

/** 
思路：
穷举
剪枝的原因：前面的匹配方式不影响后面的匹配。不管经过哪种组合匹配至序列中的一点k，那么不管k之前是
如何用primitive匹配的，后面的匹配都不受影响。
所以用已经匹配过sequence中的某位置进行剪枝。
这样最多匹配 序列长度 次。

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

const int MAX_CHARACTER_NUM = 26;
const int MAX_SEQUENCE_LEN = 200000;

typedef string TPrimitive;
typedef vector< TPrimitive > TPrimitiveList;

typedef vector< TPrimitiveList > TPrimitiveTable;

// read primitives.
TPrimitiveTable g_tPrimitiveTable( MAX_CHARACTER_NUM );

const char * g_pSequence = 0;
const char * g_pLastMatchedSequence = 0;

typedef vector< bool > TBoolVector;

TBoolVector g_tVisitFlag;

void Decompose( const char* pSequence )
{
	int nIndex = pSequence - g_pSequence;
	if ( g_tVisitFlag[ nIndex ] )
	{
		return;
	}
	else
	{
		g_tVisitFlag[ nIndex ] = true;
	}

	char curChar = *pSequence;
	if ( curChar )
	{
		const TPrimitiveList& tPrimitiveList = g_tPrimitiveTable[ curChar - 'A' ];
		for ( size_t i=0; i<tPrimitiveList.size(); ++i )
		{
			const TPrimitive& tPrimitive = tPrimitiveList[i];
//			char *pPrimitive = tPrimitive.c_str();
			bool bMatch = true;
			for ( size_t k=0; k<tPrimitive.length(); ++k )
			{
				if ( *( pSequence + k ) != tPrimitive[k] )
				{
					bMatch = false;
					break;
				}
			}

			if ( bMatch )
			{
				/*char *pNext =*/ Decompose( pSequence + tPrimitive.length() );
			}
			else
			{
				if ( pSequence > g_pLastMatchedSequence )
				{
					g_pLastMatchedSequence = pSequence;
				}
//				return pSequence;
			}
		}
//		return 0;
	}
	else
	{
		g_pLastMatchedSequence = pSequence;		// it's the last of the sequence.
//		return pSequence;
	}
}

int main()
{
	string strProblemName = "prefix";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	

	TPrimitive tPrim;
	
	while( ( fin >> tPrim ), tPrim != "." )
	{
		char firstChar = tPrim[0];
		int nIndex = firstChar - 'A';
		g_tPrimitiveTable[nIndex].push_back( tPrim );

//		fin >> tPrim;
	}

	string strSequence, strSequenceLine;
	while( fin >> strSequenceLine )
	{
		strSequence += strSequenceLine;	
	}

	g_pSequence = strSequence.c_str();
	g_pLastMatchedSequence = g_pSequence;
	g_tVisitFlag = TBoolVector( strSequence.length(), false );

//	
	Decompose( g_pSequence );

	int nMaxPrefix = g_pLastMatchedSequence - strSequence.c_str();
	fout << nMaxPrefix << endl;


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
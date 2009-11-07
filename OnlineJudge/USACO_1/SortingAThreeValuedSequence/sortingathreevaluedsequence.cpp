/*
ID: thinkin6
PROG: sort3
LANG: C++
*/

/** USA CO 2.1.5 :
Sorting a Three-Valued Sequence 
IOI'96 - Day 2
Sorting is one of the most frequently performed computational tasks. Consider the special sorting problem in which the records to be sorted have at most three different key values. This happens for instance when we sort medalists of a competition according to medal value, that is, gold medalists come first, followed by silver, and bronze medalists come last.

In this task the possible key values are the integers 1, 2 and 3. The required sorting order is non-decreasing. However, sorting has to be accomplished by a sequence of exchange operations. An exchange operation, defined by two position numbers p and q, exchanges the elements in positions p and q.

You are given a sequence of key values. Write a program that computes the minimal number of exchange operations that are necessary to make the sequence sorted.

PROGRAM NAME: sort3

INPUT FORMAT

Line 1:	 N (1 <= N <= 1000), the number of records to be sorted
Lines 2-N+1:	 A single integer from the set {1, 2, 3}
SAMPLE INPUT (file sort3.in)

9
2
2
1
3
3
3
2
3
1
OUTPUT FORMAT

A single line containing the number of exchanges required
SAMPLE OUTPUT (file sort3.out)

4

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


int main()
{
	string strProblemName = "sort3";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nNumberCount;
	fin >> nNumberCount;

	const int conMaxValue = 1000;

	int arNumberSequece[ conMaxValue ] = {0};

	// read the input sequence and count the number of three value.
	int arNumCount[3] = {0};
	for ( int i=0; i< nNumberCount; ++ i )
	{
		fin >> arNumberSequece[ i ];

		arNumCount[ arNumberSequece[i] - 1 ] ++;
	}

	// get the pairs needed to be exchange.
	typedef std::pair< int, int > TExchangePair;
//	typedef std::vector< TExchangePair > TExchangeList;
	typedef std::map< TExchangePair, int > TExchangeTable;

	TExchangeTable tExchangeTable;
	for ( int i=0; i<nNumberCount; ++i )
	{
		int nTargetNum = ( i< arNumCount[0] ? 1 : ( i < arNumCount[0] + arNumCount[1] ? 2 : 3 ) );

		if ( arNumberSequece[i] != nTargetNum )
		{
			tExchangeTable[ TExchangePair( arNumberSequece[i], nTargetNum ) ] ++;
		}
	}

	int nExchangeStep = 0;

	// first turn, find all the paire direct opposite
	for ( TExchangeTable::iterator iter = tExchangeTable.begin(); iter!= tExchangeTable.end(); ++iter )
	{
		const TExchangePair& tPair = iter->first;
		
		if ( tPair.first != tPair.second )
		{
			TExchangePair tOppositePair( tPair.second, tPair.first );
			int nOppositePairNum = tExchangeTable[ tOppositePair ];

			int nThisPairNum = iter->second;

			int nStep = min( nThisPairNum, nOppositePairNum );
			nExchangeStep += nStep;

			iter->second -= nStep;
			tExchangeTable[ tOppositePair ] -= nStep;
		}		
	}

	// then, count the left pairs, get the step.
	int nLeftNum = 0;
	for ( TExchangeTable::iterator iter = tExchangeTable.begin(); iter!= tExchangeTable.end(); ++iter )
	{
		nLeftNum += iter->second;	
	}
	nExchangeStep += ( 2*nLeftNum ) / 3;

	fout << nExchangeStep <<endl;


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
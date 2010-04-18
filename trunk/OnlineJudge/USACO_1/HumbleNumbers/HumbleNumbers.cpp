/*
ID: thinkin6
PROG: humble
LANG: C++
*/

/** USA CO 3.1.3 :
Humble Numbers
For a given set of K prime numbers S = {p1, p2, ..., pK}, consider the set of all numbers whose prime factors are a subset of S. This set contains, for example, p1, p1p2, p1p1, and p1p2p3 (among others). This is the set of `humble numbers' for the input set S. Note: The number 1 is explicitly declared not to be a humble number.

Your job is to find the Nth humble number for a given set S. Long integers (signed 32-bit) will be adequate for all solutions.

PROGRAM NAME: humble

INPUT FORMAT

Line 1:	 Two space separated integers: K and N, 1 <= K <=100 and 1 <= N <= 100,000.
Line 2:	 K space separated positive integers that comprise the set S.
SAMPLE INPUT (file humble.in)

4 19
2 3 5 7
OUTPUT FORMAT

The Nth humble number from set S printed alone on a line.
SAMPLE OUTPUT (file humble.out)

27

*/

/** 
˼·��
*	��̺ʽ��ǰ��������ˣ�һֱ�˵���ĿҪ��ĸ�����
*	������ĿҪ����������������м�֦�����ּ��ϴ�С������ĿҪ�����Ŀ���µĽ������������һ��������ô�ͼ�������Ϊʣ�µĿ϶��ǲ�����Ҫ��ġ�
*	�������stl�Ļ�������һЩ��
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



int main()
{
	string strProblemName = "humble";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nPrimNumCount, nTargetCount;
	fin >> nPrimNumCount >> nTargetCount;

	typedef std::set<u64> TNumSet;
	TNumSet tNumSet;
	typedef std::vector<u32> TNumVector;

	TNumVector tPrimList;

	u32 nMaxPrim = 0;
	for ( int i=0; i<nPrimNumCount; ++i )
	{
		u32 nNum;
		fin >> nNum;
		tNumSet.insert( nNum );
		tPrimList.push_back( nNum );

		if ( nMaxPrim < nNum )
		{
			nMaxPrim = nNum;
		}
	}

	
	TNumSet::iterator iter = tNumSet.begin();
	int nCount = 0;
	u32 nMaxNum = -1;
	while ( nCount < nTargetCount )
	{
		for ( int i=0; i<tPrimList.size(); ++i )
		{
			if ( tPrimList[i] > *iter || iter == tNumSet.end() )
			{
				break;
			}
			// ��ȡ�µĻ�������������С�
			u64 nNewNum = tPrimList[i] * *iter;		

			// ��֦�������ּ��ϵ���Ŀ�Ѿ�����������ʱ�����һ�������ģ������ɵ������������������ô�϶�������Ҫ��
			// ʣ�µķ���Ҫ���Ӧ�ñ����һ������С��
			if ( tNumSet.size() >= nTargetCount )
			{
				TNumSet::iterator iterEnd = tNumSet.end();
				--iterEnd;
				nMaxNum = *iterEnd;
			}
			if ( nNewNum > nMaxNum )
			{
				break;
			}

			tNumSet.insert( nNewNum );	

			// �������ּ��ϵĴ�СΪ��ĿҪ��ĸ�����
			if ( tNumSet.size() > nTargetCount )
			{
				TNumSet::iterator iterEnd = tNumSet.end();
				--iterEnd;
				if ( iterEnd != iter )
				{
					tNumSet.erase( iterEnd );
				}
				
			}
			
		}
				
		++iter;
		nCount ++;
	}

	// ��Ϊ�����ڼ���nCount++֮ǰiter++�ˡ�����ָ�������һ�����֡�
	--iter;

	fout << *iter << endl;


#if 0 // ������㷨�ᳬʱ��
	// �Լ����Ƶ�һ���������
	const u32 MAX_NUM = 1000000000;
	
	TNumSet::iterator iter = tNumSet.begin();
	int nCount = 0;
	while ( nCount < nTargetCount )
	{
		for ( TNumSet::iterator subIter = tNumSet.begin(); *subIter <= *iter; ++subIter )
		{
			// ��ȡ�µĻ�������������С�
			u32 nNewNum = *subIter * *iter;
			if ( nNewNum > MAX_NUM )
			{
				break;	// �����̫���ˣ��϶�������Ŀ��Ҫ�ġ�
			}
			tNumSet.insert( nNewNum );	
		}
		++iter;
		nCount ++;
	}

	// ��Ϊ�����ڼ���nCount++֮ǰiter++�ˡ�����ָ�������һ�����֡�
	--iter;

	fout << *iter << endl;

#endif

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
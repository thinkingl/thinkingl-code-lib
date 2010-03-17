/*
ID: thinkin6
PROG: runround
LANG: C++
*/

/** USA CO 2.2.5 :

Runaround Numbers
Runaround numbers are integers with unique digits, none of which is zero (e.g., 81362) that also have an interesting property, exemplified by this demonstration:

If you start at the left digit (8 in our number) and count that number of digits to the right (wrapping back to the first digit when no digits on the right are available), you'll end up at a new digit (a number which does not end up at a new digit is not a Runaround Number). Consider: 8 1 3 6 2 which cycles through eight digits: 1 3 6 2 8 1 3 6 so the next digit is 6.
Repeat this cycle (this time for the six counts designed by the `6') and you should end on a new digit: 2 8 1 3 6 2, namely 2.
Repeat again (two digits this time): 8 1
Continue again (one digit this time): 3
One more time: 6 2 8 and you have ended up back where you started, after touching each digit once. If you don't end up back where you started after touching each digit once, your number is not a Runaround number.
Given a number M (that has anywhere from 1 through 9 digits), find and print the next runaround number higher than M, which will always fit into an unsigned long integer for the given test data.

PROGRAM NAME: runround

INPUT FORMAT

A single line with a single integer, M
SAMPLE INPUT (file runround.in)

81361
OUTPUT FORMAT

A single line containing the next runaround number higher than the input value, M.
SAMPLE OUTPUT (file runround.out)

81362

*/

/** 
˼·��

ûʲô��˵�ģ��Ӹ���������ʼһ�����ļ��ɡ�
������ο�ʼ��һλ������ô��ͻ���һ�����޵�ѭ������������λ��֮ǰһģһ����
Ҳ����˵�ڵڶ��ο�ʼ������һλ��֮ǰ�����������һλ���Ͽ�ʼ��2�Σ���ô������Ͳ���Runaround number��
��֮������ڶ��ο�ʼ������һλ������û��������һλ���Ͽ�ʼ��2�Σ���ô���������Runaround number��

��һЩ�Ļ�,һ������nλ����ô��תn��֮�������û����ת���������һλ������ô������Ͳ���Runaround number��
������ǡ�
��Ϊ������n-1��������תn�Σ��϶���һ�����ᱻ ��ת�� 2�Ρ�

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
	string strProblemName = "runround";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	unsigned int nNum;
	fin >> nNum;

	const int MaxNumDigit = 10;
	int arNumber[ MaxNumDigit ] = { 0 };
	int len = sizeof( arNumber );
	memset( arNumber, 0, len  );

	int nDiv = nNum;
	int i=0;
	while( nDiv && i<MaxNumDigit )
	{
		int nSubNum =nDiv % 10;
		if ( nSubNum == 0 )
		{
			nSubNum = 1;
		}
		arNumber[i] = nSubNum;
		nDiv /= 10;
		++i;
	}

	

	while( 1 )
	{
		for ( int i=0; i<MaxNumDigit; ++i )
		{
			arNumber[i] ++;
			if ( arNumber[i] > 9 )
			{
				arNumber[i] = 1; // �����´�ѭ������λ��������һλ����λ��������0�����Խ���������
			}
			else
			{
				break;	// ����λ���Ͳ��ü����ˡ�
			}
		}

		// ����λ�ǲ��� unique .
		bool bunique = true;
		bool arUnique[ 10 ] = { false };
		for ( int i=0; arNumber[i]; ++i )
		{
			int nNum = arNumber[i];
			if ( arUnique[ nNum ] )
			{				
				bunique = false;
				break;
			}
			else
			{
				arUnique[ nNum ] = true;
			}
		}
		if ( !bunique )
		{
			continue;
		}
		

		// ����ǲ��� unique runaround number.

		// ��λ���� ��λ���������0��
		int nDigitNum = 0;
		for ( int i=0; i<MaxNumDigit; ++i )
		{
			if ( arNumber[ i ] )
			{
				++ nDigitNum;
			}
			else
			{
				break;
			}
		}

		

		// ֻѭ��λ���Ρ�
		// �����λ��ʼ��
		bool arIsTouched[MaxNumDigit] = { false };
		bool bIsRunaroundNumber = false;
		int nCursor = nDigitNum - 1;
		for ( int i=0; i<nDigitNum; ++i )
		{
			int nStep = arNumber[nCursor];
			for ( int k=0; k<nStep; ++k )
			{
				-- nCursor;
				if ( nCursor < 0 )
				{
					nCursor += nDigitNum;
				}
			}
			if ( nCursor == nDigitNum - 1 )
			{
				bIsRunaroundNumber = ( i == ( nDigitNum - 1 ) );
				break;
			}
			else
			{
				if ( arIsTouched[ nCursor ] )
				{
					break;
				}
				arIsTouched[ nCursor ] = true;
			}
		}

		if ( bIsRunaroundNumber )
		{
			nNum = 0;
			for ( int i=0; i<MaxNumDigit; ++i )
			{
				nNum += arNumber[i] * pow( 10, (long double)i );
			}

			fout << nNum << endl;
			break;
		}

	}

//	fout << nSetNum << endl;

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
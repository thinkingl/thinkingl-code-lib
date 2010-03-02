// RowAcrossTheRiver.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"


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

#include <assert.h>

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

const int MAX_NUM = 4;

/** ��ǰ����Сʱ�䡣 */
unsigned int g_nMinTime = -1;

/** �ĸ��˵Ĺ���ʱ�䡣 */
int g_arTime[MAX_NUM] = { 10, 5, 2, 1 };

/** ������Ƿ�����ߣ����Թ��ӡ� */
#define IS_HERE( nState,nIndex ) 0 ==( ( nState >> nIndex ) & 0x01 )
/** ������Ƿ����Ǳߣ����Թ����� */
#define IS_THERE( nState,nIndex ) 0 !=( ( nState >> nIndex ) & 0x01 ) 

/** ��Ź��̡� */
typedef std::vector<int> TStateList;
typedef std::vector< TStateList > TRowSequnce;

TRowSequnce g_tBestRowSequence;

void Come( int nState, unsigned int nTotalTime, TStateList tsl );

/** ���ӡ� 
*	һ����2�˹��ӡ�
*/
void Go( int nState, unsigned int nTotalTime, TStateList tsl )
{
	// �ҳ���������
	for ( int j=0; j<MAX_NUM; ++j )
	{
		if ( IS_HERE( nState, j ) )
		{			

			for ( int k=j+1; k<MAX_NUM; ++k )
			{
				if ( IS_HERE( nState, k ) )
				{

					int nTime = max( g_arTime[j] , g_arTime[k] );
					int nNextState = nState | ( 1 << j ) | ( 1 << k );
					TStateList tNextSL = tsl;
					tNextSL.push_back( nNextState );
					Come( nNextState, nTotalTime + nTime, tNextSL );					

				}
			}
		}		
	}


}

/** �������ˡ� 
*	һ����һ���˻�����
*/
void Come( int nState, unsigned int nTotalTime, TStateList tsl )
{
	const int ALL_THERE = 0x0F;
	if ( nState == ALL_THERE )
	{
		if ( g_nMinTime > nTotalTime )
		{
			g_tBestRowSequence.clear();
		}
		if ( g_nMinTime >= nTotalTime )
		{
			g_nMinTime = nTotalTime;

			g_tBestRowSequence.push_back( tsl );
		}
		return;
	}

	for ( int i=0; i<MAX_NUM; ++i )
	{
		if ( IS_THERE( nState, i ) )
		{
			// ����˻�ȥ��
			int nTime = g_arTime[i];
			int nNextState = nState & ( ~( 1 << i ) );
			TStateList tNextSl = tsl;
			tNextSl.push_back( nNextState );
			
			Go( nNextState, nTotalTime + nTime, tNextSl );
		}
	}
	return ;
}

void PrintState( int nState )
{
	for( int i=0; i<MAX_NUM; ++i )
	{
		char cOut = 'A' + i;
		if ( IS_HERE( nState, i ) )
		{
			cout << cOut << "|";
		}
		else
		{
			cout << " |" << cOut;
		}
		cout << endl;
	}
}

int main( )
{
	// ʹ��1λ��ʾһ���˵�״̬��ʹ��32λint�ĵ�4λ��ʾ����4���˵�״̬��
	// ÿ���˵�״̬�����֣�0��ʾ��û���ӡ�1��ʾ�Ѿ����ӡ�
	// ��ʼ״̬����Ҷ�û���ӡ�
	int nState = 0;
	TStateList tsl;
	tsl.push_back( nState );
	Go( nState, 0 , tsl );

	cout << "Min time: " << g_nMinTime << endl;
	cout << "���� " << g_tBestRowSequence.size() << " �ַ�����" << endl;

	for ( size_t i=0; i<g_tBestRowSequence.size(); ++i )
	{
		TStateList bt = g_tBestRowSequence[i];

		cout << "���� " << i+1 << endl;
		for ( size_t m = 0; m<bt.size(); ++m )
		{
			PrintState( bt[m] );
			cout << "-->" << endl;
		}
		cout << "���!!!" << endl;
	}

	return 0;
}


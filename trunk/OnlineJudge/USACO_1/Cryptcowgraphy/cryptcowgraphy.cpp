/*
ID: thinkin6
PROG: cryptcow
LANG: C++
*/

/** USA CO 4.1.4 :
Cryptcowgraphy
Brian Dean
The cows of Farmer Brown and Farmer John are planning a coordinated escape from their respective farms and have devised a method of encryption to protect their written communications.

Specifically, if one cow has a message, say, "International Olympiad in Informatics", it is altered by inserting the letters C, O, and W, in random location in the message, such that C appears before O, which appears before W. Then the cows take the part of the message between C and O, and the part between O and W, and swap them. Here are two examples:

International Olympiad in Informatics
-> 
CnOIWternational Olympiad in Informatics

International Olympiad in Informatics
-> 
International Cin InformaticsOOlympiad W
To make matters more difficult, the cows can apply their encryption scheme several times, by again encrypting the string that results from the previous encryption. One night, Farmer John's cows receive such a multiply-encrypted message. Write a program to compute whether or not the non-encrypted original message could have been the string:

Begin the Escape execution at the Break of Dawn
PROGRAM NAME: cryptcow

INPUT FORMAT

A single line (with both upper and lower case) with no more than 75 characters that represents the encrypted message.
SAMPLE INPUT (file cryptcow.in)

Begin the EscCution at the BreOape execWak of Dawn
OUTPUT FORMAT

Two integers on a single line. The first integer is 1 if the message decodes as an escape message; 0 otherwise. The second integer specifies the number of encryptions that were applied (or 0 if the first integer was 0).
SAMPLE OUTPUT (file cryptcow.out)

1 1 
*/

/** 
*	�����������.
*	�ؼ��Ǽ�֦�Ż�,���˹�ϣ.
*	��ϣ�Ĵ�С��Ӱ��ʱ��ͽ����ȷ��.
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

const int MaxCharacterNum = 75;
string g_OriginalMessage = "Begin the Escape execution at the Break of Dawn";
const int PlanCharacterNum = g_OriginalMessage.length();

const int HASH_TABLE_SIZE = 802973;

bool *g_HashTable = 0;

bool CheckMessageValid( const char* pMessage )
{
	char szTmp[ MaxCharacterNum + 1 ] = {0};
	int nCursor = 0;
	for ( ; *pMessage; ++pMessage )
	{
		switch ( *pMessage )
		{
		case 'C':
		case 'O':
		case 'W':
			if ( nCursor > 0 )
			{
				if ( g_OriginalMessage.find( szTmp ) == -1 )
				{
					return false;
				}
				memset( szTmp, 0, sizeof( szTmp ) );
				nCursor = 0;
			}
			break;
		default:
			szTmp[ nCursor ] = *pMessage;
			++nCursor;
			break;
		}
	}
	return true;
}

int ELFHash( const char *pStr )
{
	unsigned long h=0,g;
	while(*pStr)
	{
		h=(h<<4) + *pStr++;
		g=h & 0xF0000000L;
		if(g) h^= g>>24;
		h &= ~g;
	}
	return h % HASH_TABLE_SIZE;
}

/** ����.
*	
*/
bool dfs( const char * pCrypt, const char* pOriginalBegin, const char *pOriginalEnd )
{
	int nHashValue = ELFHash( pCrypt );

	if ( !g_HashTable[ nHashValue ] )
	{
		return false;
	}

	bool bResult = true;

	// У��.
	if ( !CheckMessageValid( pCrypt ) )
	{
		g_HashTable[ nHashValue ] = false;
		return false;
	}

	const char* pC = pCrypt;
	const char *pFirstC = 0;
	const char *pLastW = 0;

	for ( ; *pC; ++pC )		// ���Բ�ͬ��C
	{
		if ( ( 0 == pFirstC ) )	// ��������һ��'C'֮ǰ.
		{
			if ( 'C' == *pC )
			{
				pFirstC = pC;
			}
			else 
			{
				if ( *pC != *pOriginalBegin )
				{
					g_HashTable[ nHashValue ] = false;

					return false;	// ��һ��C֮ǰ���ַ������ԭʼ�ַ����.
				}
				++ pOriginalBegin;
			}
			
		}
		if ( *pC == 'C' )			
		{	
			for ( const char* pW= pCrypt+ strlen( pCrypt ) -1; pW > pC; --pW )	// ����O���治ͬ��W.���ַ�����ĩβ��ʼ��'W'
			{
				if ( 0 == pLastW )	// �����һ��W.
				{
					if ( 'W' == *pW )
					{
						pLastW = pW;
					}
					else
					{
						if ( *pW != *pOriginalEnd ) // ���ҵ����һ��'W'֮ǰ,Ҫ��ԭʼ�ַ����.
						{
							g_HashTable[ nHashValue ] = false;

							return false;
						}
						-- pOriginalEnd;
					}
				}
				if ( 'W'== *pW )
				{				
					for ( const char* pO = pC+1; pO < pW; ++pO )	// ����C���治ͬ��O
					{
						if ( 'O' == *pO )	
						{
							// ���н���.
							char planMessage[MaxCharacterNum]={0};
							int nCursor = 0;
							const char* pOrM = g_OriginalMessage.c_str();

							// �ӵ�һ��C��ʼ,��һ��C֮ǰ�Ĳ��ֱ���ȥ.
							for ( const char* pMessage = pFirstC; pMessage<pC; ++pMessage )	// first C to C
							{								
								planMessage[ nCursor++ ] = ( *pMessage );
							}
							for ( const char* pM = pO+1; pM<pW; ++pM ) // O to W
							{
								planMessage[ nCursor++ ] = ( *pM );
							}
							for ( const char* pM = pC+1; pM<pO; ++pM )	// C to O
							{
								planMessage[ nCursor++ ] = ( *pM );
							}
							for ( const char* pM = pW+1; pM <= pLastW; ++pM )	// W to LastW.
							{
								planMessage[ nCursor++ ] = ( *pM );
							}
							
							// ������һ�ֵ�У��.
							bResult = dfs( planMessage, pOriginalBegin, pOriginalEnd );
							if ( bResult )
							{
								return true;
							}

						}
					}
							
				}
			}
						
		}
	}

	
	return bResult;	
}

/** �������±���,����,����. */
bool Recode( string& crypt, string& original )
{
	const char* pMessage = crypt.c_str();

	char nextCode = '0';
	typedef std::map< string, char > TCodeTable;
	TCodeTable codeTable;

	string strCodedCrypt;
	char szTmp[ MaxCharacterNum + 1 ] = {0};
	int nCursor = 0;
	for ( ; *pMessage; ++pMessage )
	{
		switch ( *pMessage )
		{
		case 'C':
		case 'O':
		case 'W':
			if ( nCursor > 0 )
			{
				if ( codeTable.find( szTmp ) == codeTable.end() )				
				{
					codeTable[ szTmp ] = nextCode;
					++nextCode;
				}
				
				strCodedCrypt.push_back( codeTable[ szTmp ] );

				memset( szTmp, 0, sizeof( szTmp ) );
				nCursor = 0;
			}

			strCodedCrypt.push_back( *pMessage );
			break;
		default:
			szTmp[ nCursor ] = *pMessage;
			++nCursor;
			break;
		}
	}

	string strCodedOriginal;
	const char* pPM = original.c_str();
	memset( szTmp, 0, sizeof( szTmp ) );
	nCursor = 0;
	for ( ; *pPM; ++pPM )
	{
		if ( codeTable.find( szTmp ) == codeTable.end() )
		{
			szTmp[ nCursor ] = *pPM;
			++nCursor;
		}
		else
		{
			strCodedOriginal.push_back( codeTable[ szTmp ] );
			memset( szTmp, 0, sizeof( szTmp ) );
			nCursor = 0;
		}
	}

	if ( *szTmp != NULL )	// ԭʼ�ִ���ʣ��û�б���.
	{
//		g_HashTable[ nHashValue ] = false;

		return false;
	}

	crypt = strCodedCrypt;
	original = strCodedOriginal;

	return true;
}

int main()
{
	string strProblemName = "cryptcow";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	char szCrypt[ MaxCharacterNum+1 ] = {0};
	string crypt;
	fin.getline( szCrypt, sizeof( szCrypt ) );
	crypt = szCrypt;

	bool bOk = true;

	// �жϳ���.
	bOk &= crypt.size() >= g_OriginalMessage.size();
	bOk &= ( ( ( crypt.size() - g_OriginalMessage.size() ) % 3 ) == 0 );

	if ( bOk )
	{
		// ���ַ������½��б���.
//		bOk = Recode( crypt, g_OriginalMessage );
		
		g_HashTable = new bool[ HASH_TABLE_SIZE + 1 ];
		memset( g_HashTable, true, sizeof( g_HashTable[0] ) * ( HASH_TABLE_SIZE+1 ) ); 

		if( bOk )
			bOk = dfs( crypt.c_str(), g_OriginalMessage.c_str(), g_OriginalMessage.c_str() + g_OriginalMessage.size() - 1 );
	
		delete g_HashTable;
		g_HashTable = 0;
	}

	
	
	if ( !bOk )
	{
		fout << 0 << " " << 0 << endl;
	}
	else
	{
		int nEncryptionNum = ( crypt.length() - PlanCharacterNum ) / 3;
		fout << 1 << " " << nEncryptionNum << endl;
	}

#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}
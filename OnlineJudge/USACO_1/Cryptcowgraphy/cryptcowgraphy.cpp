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




#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}
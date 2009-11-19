/*
ID: thinkin6
PROG: hamming
LANG: C++
*/

/** USA CO 2.1.7 :
Hamming Codes

Hamming Codes
Rob Kolstad
Given N, B, and D: Find a set of N codewords (1 <= N <= 64), each of length B bits (1 <= B <= 8), such that each of the codewords is at least Hamming distance of D (1 <= D <= 7) away from each of the other codewords. The Hamming distance between a pair of codewords is the number of binary bits that differ in their binary notation. Consider the two codewords 0x554 and 0x234 and their differences (0x554 means the hexadecimal number with hex digits 5, 5, and 4):

0x554 = 0101 0101 0100
0x234 = 0010 0011 0100
Bit differences: xxx  xx
Since five bits were different, the Hamming distance is 5.

PROGRAM NAME: hamming

INPUT FORMAT

N, B, D on a single line

SAMPLE INPUT (file hamming.in)

16 7 3
OUTPUT FORMAT

N codewords, sorted, in decimal, ten per line. In the case of multiple solutions, your program should output the solution which, if interpreted as a base 2^B integer, would have the least value.

SAMPLE OUTPUT (file hamming.out)

0 7 25 30 42 45 51 52 75 76
82 85 97 102 120 127

*/

/** 
思路：

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
    string strProblemName = "hamming";

    string strInFile = strProblemName + ".in";
    string strOutFile = strProblemName + ".out";


    ofstream fout ( strOutFile.c_str() );
    ifstream fin ( strInFile.c_str() );

    if( !fin )
    {
        cout << "open input file fail!" << endl;
        return 0;
    }

    int nNum, nBits, nDistance;
    fin >> nNum >> nBits >> nDistance;

    int nMaxNum = ( 1 << nBits ) - 1 ;

    typedef std::vector<int> TNumList;
    TNumList tNumList;
    for ( int i=0; i<=nMaxNum && tNumList.size() < nNum; ++i )
    {
        bool bOk = true;
        for ( int k=0; k<tNumList.size(); ++k )
        {
            // 求取distance。
            int nAnother = tNumList[k];
            int nMask = 1;
            int nDis = 0;
            for ( int j=0; j<nBits; (++j), (nMask <<= 1) )
            {
                if ( ( nAnother & nMask ) != ( i & nMask ) )
                {
                    nDis ++;
                }
            }

            if ( nDis < nDistance )
            {
                bOk = false;
                break;
            }
        }
        
        if ( bOk )
        {
            tNumList.push_back( i );
        }
    }

    for ( int i=0; i<tNumList.size(); ++i )
    {
        fout << tNumList[i];
        if ( ( i+1 ) % 10 == 0 || i == tNumList.size() - 1 )
        {
            fout << endl;
        }
        else
        {
            fout << " ";
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
/*
ID: thinkin6
PROG: preface
LANG: C++
*/

/** USA CO 2.2.3 :
Preface Numbering

Preface Numbering
A certain book's prefaces are numbered in upper case Roman numerals. Traditional Roman numeral values use a single letter to represent a certain subset of decimal numbers. Here is the standard set:

I   1     L   50    M  1000
V   5     C  100
X  10     D  500
As many as three of the same marks that represent 10n may be placed consecutively to form other numbers:

III is 3
CCC is 300
Marks that have the value 5x10n are never used consecutively.

Generally (with the exception of the next rule), marks are connected together and written in descending order to form even more numbers:

CCLXVIII = 100+100+50+10+5+1+1+1 = 268
Sometimes, a mark that represents 10^n is placed before a mark of one of the two next higher values (I before V or X; X before L or C; etc.). In this case, the value of the smaller mark is SUBTRACTED from the mark it precedes:

IV = 4
IX = 9
XL = 40
This compound mark forms a unit and may not be combined to make another compound mark (e.g., IXL is wrong for 39; XXXIX is correct).
Compound marks like XD, IC, and XM are not legal, since the smaller mark is too much smaller than the larger one. For XD (wrong for 490), one would use CDXC; for IC (wrong for 99), one would use XCIX; for XM (wrong for 990), one would use CMXC. 90 is expressed XC and not LXL, since L followed by X connotes that successive marks are X or smaller (probably, anyway).

Given N (1 <= N < 3,500), the number of pages in the preface of a book, calculate and print the number of I's, V's, etc. (in order from lowest to highest) required to typeset all the page numbers (in Roman numerals) from 1 through N. Do not print letters that do not appear in the page numbers specified.

If N = 5, then the page numbers are: I, II, III, IV, V. The total number of I's is 7 and the total number of V's is 2.

PROGRAM NAME: preface

INPUT FORMAT

A single line containing the integer N.
SAMPLE INPUT (file preface.in)

5
OUTPUT FORMAT

The output lines specify, in ascending order of Roman numeral letters, the letter, a single space, and the number of times that letter appears on preface page numbers. Stop printing letter totals after printing the highest value letter used to form preface numbers in the specified set.
SAMPLE OUTPUT (file preface.out)

I 7
V 2
*/

/** 
思路：
罗马数字看起来好复杂！！
但我已经发现规律了，哈哈，我果然是个天才！
。。。最近在温习“灌篮高手”呢，有点走火入魔。
罗马数字是基于加减法而不是进位乘法的，书写时可以从高往低一层层尝试去减，每层分别对应着罗马数字符号：
1000    900 500 400 100 90  50  40  10  9   5   4   1
M       CM  D   CD  C   XC  L   XL  X   IX  V   IV  I
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
    string strProblemName = "preface";

    string strInFile = strProblemName + ".in";
    string strOutFile = strProblemName + ".out";


    ofstream fout ( strOutFile.c_str() );
    ifstream fin ( strInFile.c_str() );

    if( !fin )
    {
        cout << "open input file fail!" << endl;
        return 0;
    }

    int nPagesNum;
    fin >> nPagesNum;

 /*     I   1     L   50    M  1000
        V   5     C  100
        X  10     D  500*/
    enum ERomanLetter
    {
        RL_I = 0,
        RL_V,
        RL_X,
        RL_L,
        RL_C,
        RL_D,
        RL_M,
        RL_Count,
    };

    char arRomanLetter[] = { 'I', 'V', 'X', 'L', 'C', 'D', 'M' };

    //    1000    900 500 400 100 90  50  40  10  9   5   4   1
    //    M       CM  D   CD  C   XC  L   XL  X   IX  V   IV  I

    int arRomanLetterNum[ RL_Count ] = {0};
    for ( int i=1; i<=nPagesNum; ++i )
    {
        int nNum = i;
        int nNumBk = nNum;
        char szNum[ 100 ] = {0};
        while( nNum > 0 )
        {
            if( nNum >= 1000 )
            {
                strcat( szNum, "M" );
                arRomanLetterNum[ RL_M ] ++;

                nNum -= 1000;
            }
            else if( nNum >= 900 )
            {
                strcat( szNum, "CM" );

                arRomanLetterNum[ RL_C ] ++;
                arRomanLetterNum[ RL_M ] ++;

                nNum -= 900;
            }
            else if( nNum >= 500 )
            {
                strcat( szNum, "D" );

                arRomanLetterNum[ RL_D ] ++;

                nNum -= 500;
            }
            else if( nNum >= 400 )
            {
                strcat( szNum, "CD" );

                arRomanLetterNum[ RL_C ] ++;
                arRomanLetterNum[ RL_D ] ++;

                nNum -= 400;
            }
            else if( nNum >= 100 )
            {
                strcat( szNum, "C" );

                arRomanLetterNum[ RL_C ] ++;

                nNum -= 100;
            }
            else if( nNum >= 90 )
            {
                strcat( szNum, "XC" );

                arRomanLetterNum[ RL_X ] ++;
                arRomanLetterNum[ RL_C ] ++;

                nNum -= 90;
            }
            else if( nNum >= 50 )
            {
                strcat( szNum, "L" );

                arRomanLetterNum[ RL_L ] ++;

                nNum -= 50;
            }
            else if( nNum >= 40 )
            {
                strcat( szNum, "XL" );

                arRomanLetterNum[ RL_X ] ++;
                arRomanLetterNum[ RL_L ] ++;

                nNum -= 40;
            }
            else if( nNum >= 10 )
            {
                strcat( szNum, "X" );

                arRomanLetterNum[ RL_X ] ++;

                nNum -= 10;
            }
            else if( nNum >= 9 )
            {
                strcat( szNum, "IX" );

                arRomanLetterNum[ RL_I ] ++;
                arRomanLetterNum[ RL_X ] ++;

                nNum -= 9;
            }
            else if( nNum >= 5 )
            {
                strcat( szNum, "V" );

                arRomanLetterNum[ RL_V ] ++;

                nNum -= 5;
            }
            else if( nNum >= 4 )
            {
                strcat( szNum, "IV" );

                arRomanLetterNum[ RL_I ] ++;
                arRomanLetterNum[ RL_V ] ++;

                nNum -= 4;
            }
            else if( nNum >= 1 )
            {
                strcat( szNum, "I" );

                arRomanLetterNum[ RL_I ] ++;

                nNum -= 1;
            }

        }  

        cout << "num: " << nNumBk << " : " << szNum << endl;
    }
    
    int nStopPos = 0;
    for ( int i=0; i<RL_Count; ++i )
    {
        nStopPos = RL_Count - i - 1;
        if ( arRomanLetterNum[ nStopPos ] != 0 )
        {            
            break;
        }
    }

    for ( int i=0; i<=nStopPos; ++i )
    {
        fout << arRomanLetter[i] << " " << arRomanLetterNum[i] << endl;
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
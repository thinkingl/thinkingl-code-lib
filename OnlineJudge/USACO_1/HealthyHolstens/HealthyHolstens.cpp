/*
ID: thinkin6
PROG: holstein
LANG: C++
*/

/** USA CO 2.1.6 :
Healthy Holsteins

Healthy Holsteins
Burch & Kolstad
Farmer John prides himself on having the healthiest dairy cows in the world. He knows the vitamin content for one scoop of each feed type and the minimum daily vitamin requirement for the cows. Help Farmer John feed his cows so they stay healthy while minimizing the number of scoops that a cow is fed.

Given the daily requirements of each kind of vitamin that a cow needs, identify the smallest combination of scoops of feed a cow can be fed in order to meet at least the minimum vitamin requirements.

Vitamins are measured in integer units. Cows can be fed at most one scoop of any feed type. It is guaranteed that a solution exists for all contest input data.

PROGRAM NAME: holstein

INPUT FORMAT

Line 1:	integer V (1 <= V <= 25), the number of types of vitamins
Line 2:	V integers (1 <= each one <= 1000), the minimum requirement for each of the V vitamins that a cow requires each day
Line 3:	integer G (1 <= G <= 15), the number of types of feeds available
Lines 4..G+3:	V integers (0 <= each one <= 1000), the amount of each vitamin that one scoop of this feed contains. The first line of these G lines describes feed #1; the second line describes feed #2; and so on.
SAMPLE INPUT (file holstein.in)

4
100 200 300 400
3
50   50  50  50
200 300 200 300
900 150 389 399
OUTPUT FORMAT

The output is a single line of output that contains:

the minimum number of scoops a cow must eat, followed by:
a SORTED list (from smallest to largest) of the feed types the cow is given
If more than one set of feedtypes yield a minimum of scoops, choose the set with the smallest feedtype numbers.
SAMPLE OUTPUT (file holstein.out)

2 1 3

*/

/** 
    思路：
    1. 穷举。题目规模是25种vitamin，15种饲料，不多，穷举规模是C15 1 + C15 2 +... + C15 N，C15 N 表示在15种饲料中挑选出N种的组合。
    N是最小的饲料种类。
    此算法最差情况下的运算规模是。C15 7才6435，这个规模肯定远小于15×C15 7 = 96525。精确的需要写程序算一下，反正对现在的计算机来说小菜。

    2. （错误！）可以处理大规模数据的算法。我的直觉告诉我可以，但没想好怎么证明正确性。
        1）首先用贪婪法，可以确定最小的饲料种类。具体做法是循环所有饲料，分别在需要的vitamin里减去饲料里含有的vitamin，负数的差认做0，
        取最大vitamin差额最小的饲料。如果有多种饲料差额一样，取序号最小的。记录这种饲料，并将这种饲料在饲料中排除。
        如果最大差额不是0，继续循环做以上处理，直到最大差额是0.
        这时记录下来的饲料个数就是最小的饲料个数。此步运算规模是15×N，最大规模是15×15.
        2）优化得到的饲料序列，使它满足题意是最小的。
        (If more than one set of feedtypes yield a minimum of scoops, choose the set with the smallest feedtype numbers.)
        a）可以从最小序号开始组合N种饲料，直到满足条件。运算规模C15 N,最差 C15 7
        b）替换法。用小的替换大的，具体步骤和正确性保证，我的直觉不太确定。

        反例：viatmin 2种， 分别需要 3， 1000.饲料3种，分别含有：
        （1） 0 999
        （2） 1 500
        （3） 2 500
        最小的组合应该是两种饲料，2和3.
        但用上面的算法，第一个会选中1，造成需要3种饲料才可以满足。
        。
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

const int MAX_FEEDTYPE = 15;
const int MAX_VITAMIN_TYPE = 25;

class CCowFarm
{
public:
    CCowFarm();

    void InputData( ifstream& infile );
 //   void OutputData( ofstream& outfile );
    void OutputData( basic_ostream<char, char_traits<char> >& outStream );
    bool FeedCow();
private:

    int m_nVitaminNum;
    int m_nFeedtypeNum;

    int m_arVitaminNeed[MAX_VITAMIN_TYPE];
    int m_arFeedVitamin[MAX_FEEDTYPE][MAX_VITAMIN_TYPE];
};

CCowFarm::CCowFarm()
{
    m_nVitaminNum = 0;
    m_nFeedtypeNum = 0;
    memset( m_arVitaminNeed, 0, sizeof( m_arVitaminNeed[0] ) * MAX_VITAMIN_TYPE );
    memset( m_arFeedVitamin, 0, sizeof( m_arFeedVitamin[0][0] * MAX_FEEDTYPE * MAX_VITAMIN_TYPE ) );
}

void CCowFarm::InputData( ifstream& infile )
{
    // Line one : The number of types of vitamin.
    infile >> m_nVitaminNum;

    // Line 2: The minimum requirement for each of the vitamins that a cow requires each day.
    for ( int i=0; i<m_nVitaminNum; ++i )
    {
        infile >> m_arVitaminNeed[ i ] ;
    }

    // Line 3: The number of types of feeds available.
    infile >> m_nFeedtypeNum;

    // Lines: The amount of each vitamin that one scoop of this feed contains.
    for ( int nFeedtype =0; nFeedtype < m_nFeedtypeNum; ++nFeedtype )
    {
        for ( int nVitamin=0; nVitamin<m_nVitaminNum; ++nVitamin )
        {
            infile >> m_arFeedVitamin[ nFeedtype ][ nVitamin ] ;
        }
    }
}

void CCowFarm::OutputData( basic_ostream<char, char_traits<char> >& outStream )
{

}

bool CCowFarm::FeedCow()
{
    return false;
}

int main()
{
    string strProblemName = "holstein";

    string strInFile = strProblemName + ".in";
    string strOutFile = strProblemName + ".out";


    ofstream fout ( strOutFile.c_str() );
    ifstream fin ( strInFile.c_str() );

    if( !fin )
    {
        cout << "open input file fail!" << endl;
        return 0;
    }

    CCowFarm farm;
    farm.InputData( fin );
    farm.OutputData( fout );
    farm.OutputData( cout );
    


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
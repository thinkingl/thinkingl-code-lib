/*
ID: thinkin6
PROG: lamps
LANG: C++
*/

/** USA CO 2.2.6 :

Party Lamps
IOI 98
To brighten up the gala dinner of the IOI'98 we have a set of N (10 <= N <= 100) colored lamps numbered from 1 to N.

The lamps are connected to four buttons:

Button 1: When this button is pressed, all the lamps change their state: those that are ON are turned OFF and those that are OFF are turned ON.
Button 2: Changes the state of all the odd numbered lamps.
Button 3: Changes the state of all the even numbered lamps.
Button 4: Changes the state of the lamps whose number is of the form 3xK+1 (with K>=0), i.e., 1,4,7,...
A counter C records the total number of button presses.

When the party starts, all the lamps are ON and the counter C is set to zero.

You are given the value of counter C (0 <= C <= 10000) and the final state of some of the lamps after some operations have been executed. Write a program to determine all the possible final configurations of the N lamps that are consistent with the given information, without repetitions.

PROGRAM NAME: lamps

INPUT FORMAT

No lamp will be listed twice in the input.

Line 1:	N
Line 2:	Final value of C
Line 3:	 Some lamp numbers ON in the final configuration, separated by one space and terminated by the integer -1.
Line 4:	 Some lamp numbers OFF in the final configuration, separated by one space and terminated by the integer -1.
SAMPLE INPUT (file lamps.in)

10
1
-1
7 -1
In this case, there are 10 lamps and only one button has been pressed. Lamp 7 is OFF in the final configuration.

OUTPUT FORMAT

Lines with all the possible final configurations (without repetitions) of all the lamps. Each line has N characters, where the first character represents the state of lamp 1 and the last character represents the state of lamp N. A 0 (zero) stands for a lamp that is OFF, and a 1 (one) stands for a lamp that is ON. The lines must be ordered from least to largest (as binary numbers).

If there are no possible configurations, output a single line with the single word `IMPOSSIBLE'

SAMPLE OUTPUT (file lamps.out)

0000000000
0101010101
0110110110
In this case, there are three possible final configurations:
All lamps are OFF
Lamps 1, 4, 7, 10 are OFF and lamps 2, 3, 5, 6, 8, 9 are ON.
Lamps 1, 3, 5, 7, 9 are OFF and lamps 2, 4, 6, 8, 10 are ON.

*/

/** 
思路：
个按钮都有一个规律，那就是按一个按钮两次（不管是连续按还是非连续按），都相当于没有按过。
因此，可能的组合是2的4次幂，16种。在里面滤出符合条件的就行了。
条件1是灯的状态。
条件二是按的次数。达到某种组合需要按的次数假如是i（i一定小于4），那么可能的次数就是i+2n（n=0，1，2 ... ）。

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

class CLampLine
{
public:
	enum ELampState
	{
		Lamp_Off = 0,
		Lamp_On = 1,		
	};
public:
	CLampLine( int nLampNum );
	CLampLine( const CLampLine& another );
	CLampLine& operator = ( const CLampLine& another );
//	void SetLampNum( int nNum );
	bool operator < ( const CLampLine& anoter ) const;

	void PushButton( int nButton );
	void PushButton1();
	void PushButton2();
	void PushButton3();
	void PushButton4();

	int GetLampState( int nLamp ) const;

	int GetPushButtonCounter() const;

	void Print( ofstream& fo ) const;

	bool IsSameStat( const CLampLine& another ) const;

protected:
private:

	typedef vector<int> TLampLineState;
	TLampLineState m_tLampLineState;

	int m_nPushButtonCounter;
};

CLampLine::CLampLine( int nLampNum )
{
	this->m_tLampLineState = TLampLineState( nLampNum, Lamp_On );
	this->m_nPushButtonCounter = 0;
}

CLampLine::CLampLine( const CLampLine& another )
{
	*this = another;
}

CLampLine& CLampLine::operator =( const CLampLine& another )
{
	if ( this == &another )
	{
		return *this;
	}
	this->m_tLampLineState = another.m_tLampLineState;
	this->m_nPushButtonCounter = another.m_nPushButtonCounter;
	return *this;
}

void CLampLine::PushButton( int nButton )
{
	switch( nButton )
	{
	case 0:
		PushButton1();
		break;
	case 1:
		PushButton2();
		break;
	case 2:
		PushButton3();
	    break;
	case 3:
		PushButton4();
	    break;
	default:
	    break;
	}
}

void CLampLine::PushButton1()
{
	++m_nPushButtonCounter;

	for ( size_t i=0; i<m_tLampLineState.size(); ++i )
	{
		m_tLampLineState[i] = !m_tLampLineState[i];
	}
}

void CLampLine::PushButton2()
{
	++m_nPushButtonCounter;
	// odd lamps
	for ( size_t i=0; i<m_tLampLineState.size(); i+=2 )
	{
		m_tLampLineState[i] = !m_tLampLineState[i];
	}
}

void CLampLine::PushButton3()
{
	++m_nPushButtonCounter;
	// even lamps. 
	for ( size_t i=1; i<m_tLampLineState.size(); i+=2 )
	{
		m_tLampLineState[i] = !m_tLampLineState[i];
	}
}

void CLampLine::PushButton4()
{
	++m_nPushButtonCounter;
	// 3k + 1.
	for ( size_t i=0; i<m_tLampLineState.size(); i+=3 )
	{
		m_tLampLineState[i] = !m_tLampLineState[i];
	}
}

int CLampLine::GetLampState( int nLamp ) const
{
	return m_tLampLineState[ nLamp ];
}

int CLampLine::GetPushButtonCounter() const
{
	return m_nPushButtonCounter;
}

bool CLampLine::operator <( const CLampLine& anoter ) const
{
	bool bRet = this->m_tLampLineState < anoter.m_tLampLineState;
	if ( bRet )
	{
		return bRet;
	}
	else // not less than. maybe bigger, maybe eque .
	{
		bRet = !(  anoter.m_tLampLineState < this->m_tLampLineState );
		if ( !bRet )
		{
			return bRet;
		}
		else
		{
			return this->m_nPushButtonCounter < anoter.m_nPushButtonCounter;
		}
	}
	
}

bool CLampLine::IsSameStat( const CLampLine& another ) const
{
	return m_tLampLineState == another.m_tLampLineState;
}

void CLampLine::Print( ofstream& fo ) const
{
	for ( size_t i = 0; i<m_tLampLineState.size(); ++i )
	{
		fo << m_tLampLineState[i];
// 		if ( i != m_tLampLineState.size() - 1 )
// 		{
// 			fo << " ";
// 		}
	}
	fo << endl;
}

typedef set< CLampLine > TLampLineSet;

void GetAllPosibleLampLine( const CLampLine& curLampLine, int nPushStep, TLampLineSet& allPossibleLampLineSet )
{
	if ( nPushStep < 4 )
	{
		GetAllPosibleLampLine( curLampLine, nPushStep + 1, allPossibleLampLineSet );

		CLampLine afterPush = curLampLine;
		afterPush.PushButton( nPushStep );
		GetAllPosibleLampLine( afterPush, nPushStep + 1, allPossibleLampLineSet );
	}	
	else
	{
		allPossibleLampLineSet.insert( curLampLine );
	}

}

int main()
{
	string strProblemName = "lamps";

	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";


	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	unsigned int nLampNum;
	fin >> nLampNum;

	CLampLine initLampLine( nLampNum );
	TLampLineSet allPossibleLampLineSet;
	GetAllPosibleLampLine( initLampLine, 0, allPossibleLampLineSet );

	int nPushCounter;
	fin >> nPushCounter;

	typedef vector<int> TLampLineStat;
	TLampLineStat lampsShouldOn, lampsShouldOff;

	int nLampState;
	fin >> nLampState;
	while( nLampState != -1 )
	{
		lampsShouldOn.push_back( nLampState );
		fin >> nLampState;
	}

	fin >> nLampState;
	while( nLampState != -1 )
	{
		lampsShouldOff.push_back( nLampState );
		fin >> nLampState;
	}



	// check .
	bool bHaveRet = false;
	TLampLineSet::const_iterator citerPreOkOrBadStat = allPossibleLampLineSet.end();
	for ( TLampLineSet::const_iterator citer = allPossibleLampLineSet.begin();
		citer != allPossibleLampLineSet.end(); ++citer )
	{
		if ( citerPreOkOrBadStat != allPossibleLampLineSet.end() && citerPreOkOrBadStat->IsSameStat( *citer )  )
		{
			continue;
		}

		int nStepDiff = nPushCounter - citer->GetPushButtonCounter();
		if ( nStepDiff < 0 ||  0 != ( nStepDiff & 0x1 ) ) // odd is not good, even is ok.
		{
			continue;			
		}

		bool bStatOk = true;
		// check lamps should on.
		for ( size_t i=0; i<lampsShouldOn.size(); ++i )
		{
			int nSt = citer->GetLampState( lampsShouldOn[i] - 1 );
			if ( nSt != CLampLine::Lamp_On )
			{
				bStatOk = false;
				continue;
			}
		}

		if ( !bStatOk )
		{
			citerPreOkOrBadStat = citer;
			continue;
		}

		// check lamps should off.
		for ( size_t i=0; i<lampsShouldOff.size(); ++i )
		{
			int nSt = citer->GetLampState( lampsShouldOff[i] - 1 );
			if ( nSt != CLampLine::Lamp_Off )
			{
				bStatOk = false;
				continue;
			}
		}

		if ( !bStatOk )
		{
			citerPreOkOrBadStat = citer;
			continue;
		}
		citerPreOkOrBadStat = citer;
		citer->Print( fout );
		bHaveRet = true;
	}

	if (  !bHaveRet )
	{
		fout << "IMPOSSIBLE" << endl;
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
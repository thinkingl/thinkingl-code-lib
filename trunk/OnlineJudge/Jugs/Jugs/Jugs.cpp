// Jugs.cpp : �������̨Ӧ�ó������ڵ㡣
//
// ˮ������.
// �� Anagrams By Stack �еķ���.
// ���������п��ܳ��ֵ�ֵ..���ҵ�Ŀ��ֵ�����..

// ��ϸ:
// �ø���Ԫ������Ѿ������Ľ��,���ڼ�ʱ��֦.��Ϊͬ���������δ���ķ�չ��û���κεĲ��.
// Ū������Ԫ����.
// 1.���� A.	Fill A
// 2.���� B.	Fill B    
// 3. A-> B.	Pour A to B
// 4. B-> A.	Pour B to A
// 5  ��� A.	Empty A
// 6. ��� B.	Empty B

// ���õ���һֱ���Լ���������. :-)

// ϲ����class. ϰ��,����ʡ����������..

// #include "stdafx.h"
#include <iostream>
#include <string>


using namespace std;


class FoolBaby
{
public:
	FoolBaby( int nJugA, int nJugB, int nGoal );
	~FoolBaby();

	bool Work(  int curCapacityA, int curCapacityB , string outputWord );

private:
	// �洢A,B״̬������.
	bool ** m_arrayCapacity;

	int m_nJugACapacity, m_nJugBCapacity, m_nGoalCapacity;

	// ����A.
	bool FillA( int curCapacityA, int curCapacityB, string outputWord );

	bool FillB(  int curCapacityA, int curCapacityB, string outputWord  );

	bool PourAtoB(  int curCapacityA, int curCapacityB, string outputWord  );

	bool PourBtoA(  int curCapacityA, int curCapacityB, string outputWord  );

	bool EmptyA(  int curCapacityA, int curCapacityB, string outputWord  );

	bool EmptyB(  int curCapacityA, int curCapacityB, string outputWord  );

	// ��֦,���,ѭ��.
	bool PruningCheckLoop(  int curCapacityA, int curCapacityB, string outputWord );
};

bool FoolBaby::PruningCheckLoop(  int curCapacityA, int curCapacityB, string outputWord )
{
	// ��֦.
	if( this->m_arrayCapacity[curCapacityA][curCapacityB] )
	{
		return false;
	}
	else
	{
		m_arrayCapacity[curCapacityA][curCapacityB] = true;
	}

	// Check.
	if( curCapacityA == m_nGoalCapacity || curCapacityB == m_nGoalCapacity )
	{
		cout << outputWord;
		return true;
	}

	// Do Next work.
	return this->Work( curCapacityA, curCapacityB,  outputWord );
}

bool FoolBaby::PourAtoB(  int curCapacityA, int curCapacityB, string outputWord  )
{
	if( curCapacityA == 0 || curCapacityB == this->m_nJugBCapacity )
	{
		return false;
	}

	// Pour.
	int tempA = max( 0, curCapacityA - this->m_nJugBCapacity + curCapacityB );
	curCapacityB += curCapacityA - tempA;
	curCapacityA = tempA;
	outputWord += "pour A B\n";

	return this->PruningCheckLoop( curCapacityA, curCapacityB, outputWord );
}

bool FoolBaby::PourBtoA(int curCapacityA, int curCapacityB, std::string outputWord)
{
	if( curCapacityA == m_nJugACapacity || curCapacityB == 0 )
	{
		return false;
	}

	// Pour.
	int tempA = min( m_nJugACapacity, curCapacityA+curCapacityB );
	curCapacityB -= tempA - curCapacityA;
	curCapacityA = tempA;
	outputWord += "pour B A\n";

	return this->PruningCheckLoop( curCapacityA, curCapacityB, outputWord );

}

bool FoolBaby::FillA(  int curCapacityA, int curCapacityB, string outputWord  )
{
	if( curCapacityA == this->m_nJugACapacity )
	{
		return false;
	}

	// Fill.
	curCapacityA = m_nJugACapacity;
	outputWord += "fill A\n" ;

	return this->PruningCheckLoop( curCapacityA, curCapacityB, outputWord );

}

bool FoolBaby::FillB(  int curCapacityA, int curCapacityB, string outputWord  )
{
	if( curCapacityB == this->m_nJugBCapacity )
	{
		return false;
	}

	// Fill.
	curCapacityB = m_nJugBCapacity;
	outputWord += "fill B\n" ;

	return this->PruningCheckLoop( curCapacityA, curCapacityB, outputWord );
}

bool FoolBaby::EmptyA(int curCapacityA, int curCapacityB, std::string outputWord)
{
	if( curCapacityA == 0 )
	{
		return false;
	}

	// empty.
	curCapacityA = 0;
	outputWord += "empty A\n";

	return this->PruningCheckLoop( curCapacityA, curCapacityB, outputWord );
}

bool FoolBaby::EmptyB(int curCapacityA, int curCapacityB, std::string outputWord)
{
	if( curCapacityB == 0 )
	{
		return false;
	}

	// empty.
	curCapacityB = 0;
	outputWord += "empty B\n";

	return this->PruningCheckLoop( curCapacityA, curCapacityB, outputWord );
}

FoolBaby::FoolBaby(int nJugA, int nJugB, int nGoal)
{	
	m_arrayCapacity = new bool*[nJugA + 1] ;
	for( int i=0; i<nJugA+1; i++ )
	{
		m_arrayCapacity[i] = new bool[nJugB + 1];

		for( int k=0; k< nJugB + 1; k++ )
		{
			m_arrayCapacity[i][k] = false;
		}
	}

	this->m_nJugACapacity = nJugA;
	this->m_nJugBCapacity = nJugB;
	this->m_nGoalCapacity = nGoal;

	this->m_arrayCapacity[0][0] = true;
}

FoolBaby::~FoolBaby()
{
}

bool FoolBaby::Work( int curCapacityA, int curCapacityB, string outputWord )
{
	if( this->FillA( curCapacityA, curCapacityB, outputWord ) )
	{
		return true;
	}

	if( this->FillB( curCapacityA, curCapacityB, outputWord ) )
	{
		return true;
	}

	if( this->PourAtoB( curCapacityA, curCapacityB, outputWord ) )
	{
		return true;
	}
	if( this->PourBtoA( curCapacityA, curCapacityB, outputWord ) )
	{
		return true;
	}
	if( this->EmptyA( curCapacityA, curCapacityB, outputWord ) )
	{
		return true;
	}
	if( this->EmptyB( curCapacityA, curCapacityB, outputWord ) )
	{
		return true;
	}

	return false;
}


int main()
{

	int nCapacityA, nCapacityB, nGoalCapacity;
	while( cin >> nCapacityA >> nCapacityB >> nGoalCapacity )
	{
		FoolBaby worker( nCapacityA, nCapacityB, nGoalCapacity );
		string temp;
		if( worker.Work( 0, 0, temp ) )		
			cout << "success" << endl;
	}
}

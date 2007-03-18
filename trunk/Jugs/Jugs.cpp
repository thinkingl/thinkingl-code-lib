// Jugs.cpp : 定义控制台应用程序的入口点。
//
// 水壶问题.
// 用 Anagrams By Stack 中的方法.
// 搜索出所有可能出现的值..在找到目标值后结束..

// 详细:
// 用个二元数组存已经遍历的结果,用于及时剪枝.因为同样的组合在未来的发展上没有任何的差别.
// 弄出几个元操作.
// 1.灌满 A.	Fill A
// 2.灌满 B.	Fill B
// 3. A-> B.	Pour A to B
// 4. B-> A.	Pour B to A
// 5  清空 A.	Empty A
// 6. 清空 B.	Empty B

// 就让电脑一直尝试几个操作吧. :-)

// 喜欢用class. 习惯,并且省了声明函数..

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
	// 存储A,B状态的数组.
	bool ** m_arrayCapacity;

	int m_nJugACapacity, m_nJugBCapacity, m_nGoalCapacity;

	// 灌满A.
	bool FillA( int curCapacityA, int curCapacityB, string outputWord );

	bool FillB(  int curCapacityA, int curCapacityB, string outputWord  );

	bool PourAtoB(  int curCapacityA, int curCapacityB, string outputWord  );

	bool PourBtoA(  int curCapacityA, int curCapacityB, string outputWord  );

	bool EmptyA(  int curCapacityA, int curCapacityB, string outputWord  );

	bool EmptyB(  int curCapacityA, int curCapacityB, string outputWord  );

	// 剪枝,检查,循环.
	bool PruningCheckLoop(  int curCapacityA, int curCapacityB, string outputWord );
};

bool FoolBaby::PruningCheckLoop(  int curCapacityA, int curCapacityB, string outputWord )
{
	// 剪枝.
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

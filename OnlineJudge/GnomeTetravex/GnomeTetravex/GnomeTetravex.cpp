// GnomeTetravex.cpp : 定义控制台应用程序的入口点。
//

// 无优化的版本超时无法通过..
// 优化策略:
// 这个方框是中心对称的.
// 如果其中的一个方块在某点上无法找到一个满足条件的组合,那么,这个方块在这个点的旋转对应点上也不会找到满足条件的组合.
// 证明: 反证.如果可以找到一个组合,那么通过旋转整个方框,就能找到 无法找到满足条件组合的那个点的一个满足条件的组合..
// 这个优化只是针对空方块的对称性质.可以节约大概一半的时间..

// CentrosymmetricPlan就是用于处理对称关系的类.
// 指定Size生成一个矩阵.
// SetPoint的时候,就将那一点和那一点的旋转对称点都置位.
// 对称点求法: 一个点 (x,y), 矩阵Size = s. 那么四个点是: (x,y), (y,-x+s) (-x+s, -y+s) (-y+s, x)

// 2007.1.3
// 尝试着网上所说的限时搜索(属于Cheat)不能通过.看来jol的数据的确很强.
// 正面的优化策略: 先分析输入的方块,按方块中拥有有重复的数字的数目和位置确定方块能放置的位置.
// 例如: 在四个角上的方块必须满足有两个相邻的边是有重复数字的.
// 能放在在边上的方块必须有三个边都是重复数字,而能放在内部的方块四个数字必须都是重复的.
// 这样分析后每个方块可能的位置就能确定了.而且如果有方块没有相邻的重复数字,就可以直接返回 Impossible 了.
// 在这基础上可以实现步骤策略的优化:例如,如果有4个只有两条相邻边是重复数的方块,那么角上就不可能放其他的方块了.
// 优化的步骤应该是从中间开始放起.然后是边上,最后是角落.
// 但是,实现起来比较复杂,先不在步骤上做优化了.

// #include "stdafx.h"

#include <iostream>
using namespace std;

class Square;
class PlanarPosition;
void OutPutTheResult(Square *** curSquare, int gameSize , PlanarPosition curPos );

// 优化的游戏矩阵
class OptimizeGameSquare
{
public:
protected:
private:
};


// class for Position on a plan.
class PlanarPosition
{
public:
 //   PlanarPosition( const Position & pos, int planSize );
    PlanarPosition( const int& plainSize );

    int m_x;
    int m_y;

    PlanarPosition &operator ++( int );

    // Check if it is over the end.
    bool IsOverEnd() const ;

	PlanarPosition LeftStep( int nStep );
	PlanarPosition RightStep( int nStep );
	PlanarPosition UpStep( int nStep );
	PlanarPosition DownStep( int nStep );

private:   

    // the size of the square.
    int m_planSize;
};

PlanarPosition PlanarPosition::LeftStep(int nStep )
{
	PlanarPosition newPos = *this;
	newPos.m_x -= nStep;
	return newPos;
}

PlanarPosition PlanarPosition::RightStep(int nStep )
{
	PlanarPosition newPos = *this;
	newPos.m_x += nStep;
	return newPos;
}

PlanarPosition PlanarPosition::UpStep(int nStep )
{
	PlanarPosition newPos = *this;
	newPos.m_y -= nStep;
	return newPos;
}

PlanarPosition PlanarPosition::DownStep(int nStep )
{
	PlanarPosition newPos = *this;
	newPos.m_y += nStep;
	return newPos;
}

PlanarPosition::PlanarPosition(const int& plainSize )
{
    this->m_planSize = plainSize;
    m_x = 0;
    m_y = 0;
}

PlanarPosition &PlanarPosition::operator ++( int )
{
    m_x ++;
    if( this->m_x == m_planSize )
    {
        m_y ++;
        m_x = 0;
    }
    return *this;
}

bool PlanarPosition::IsOverEnd() const
{
    if( m_x < m_planSize && m_y < m_planSize && m_x >= 0 && m_y >= 0)
    {
        return false;
    }
    return true;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 用于优化的中心对称正方形.
class CentrosymmetricPlan 
{
public:
	CentrosymmetricPlan( const int& plainSize );
	virtual ~CentrosymmetricPlan();

	// 设置一个点以及它的旋转后的点..
	void SetPoint( int index, bool bValue, int x, int y );
	void SetPoint( int index, bool bValue, PlanarPosition pos );
	// 获取一个点的状态.
	bool GetPoint( int index, int x, int y );
	bool GetPoint( int index, PlanarPosition pos );

protected:
	// 真实的矩阵.
	bool *** m_square;

	// 大小.
	int m_size;
private:
};

CentrosymmetricPlan::CentrosymmetricPlan(const int& plainSize )
{
	m_size = plainSize;

	this->m_square = new bool**[ plainSize*plainSize ];
	
	for ( int squareCount = 0 ; squareCount< plainSize*plainSize ; squareCount ++ )
	{
		this->m_square[squareCount] = new bool*[plainSize];
		for ( int i=0; i<plainSize; i++ )
		{
			m_square[squareCount][i] = new bool[plainSize];
			for( int n=0; n<plainSize; n++ )
			{
				m_square[squareCount][i][n] = true;
			}
		}
	}

	
}

CentrosymmetricPlan::~CentrosymmetricPlan()
{
	for ( int squareNum = 0; squareNum <m_size*m_size; squareNum ++ )
	{
		for ( int i=0; i<m_size; i++ )
		{
			delete[] m_square[squareNum][i];
		}
		delete[] m_square[squareNum];
	}
	delete[] m_square;
	
}

void CentrosymmetricPlan::SetPoint(int index, bool bValue, int x, int y )
{
	int zeroSize = m_size - 1;
	this->m_square[index][y][x] = bValue;
	this->m_square[index][y][-x+zeroSize] = bValue;
	m_square[index][-x+zeroSize][-y+zeroSize] = bValue;
	m_square[index][-y+zeroSize][x] = bValue;
}

void CentrosymmetricPlan::SetPoint( int index, bool bValue, PlanarPosition pos)
{
	return this->SetPoint( index, bValue, pos.m_x, pos.m_y );
}

bool CentrosymmetricPlan::GetPoint(int index, int x, int y )
{
	return this->m_square[index][y][x];
}

bool CentrosymmetricPlan::GetPoint( int index, PlanarPosition pos)
{
	return this->GetPoint( index, pos.m_x, pos.m_y );
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Square
{
public:
	Square(  int top, int right , int bottom, int left);
	~Square();

	// 顺时针旋转1下.
	Square &operator ++(int);

	// 是否已经使用.
	bool m_bUsed;

public:
	int m_left;
	int m_top;
	int m_right;
	int m_bottom;


};

Square::Square( int top, int right , int bottom, int left )
{
	m_left = left;
	m_right = right;
	m_top = top;
	m_bottom = bottom;

	m_bUsed = false;
}

Square::~Square()
{
}

Square &Square::operator ++(int)
{
	int temp = m_left;
	m_left = m_bottom;
	m_bottom = m_right;
	m_right = m_top;
	m_top = temp;
	return *this;
}

// 初始化.
// And loop to load all the squares.
Square ** Init( int size )
{
	Square **squaresArray = new Square*[size*size];
	for( int i=0; i<size*size; i++ )
	{
		int top, right, bottom, left;
		cin >> top >>  right  >>  bottom >> left ;
		squaresArray[i] = new Square(  top, right, bottom, left );	
	}
	return squaresArray;
}

Square ***InitSquares( int size )
{
	Square ***squareArray = new Square**[size];
	for( int i=0; i<size; i++ )
	{
		squareArray[i] = new Square *[size];
	}
	return squareArray;
}

// release the memory.
void Destroy( Square *** squaresArray, int gameSize )
{
	for( int i=0; i<gameSize; i++ )
	{
		delete[] squaresArray[i];
	}

	delete[] squaresArray;
}

// 检查合法性.
// 只检查当前位置的方块在所有方块中是否合法.
// 限制: 放方块的顺序总是从左上到左下的顺序,所以,默认方块的右边和下边一定没有其他的方块.
bool CheckValid( Square *** curSquares, PlanarPosition curPos, int gameSize )
{

//	OutPutTheResult( curSquares, gameSize, curPos );
//	counter ++;

	PlanarPosition leftPos = curPos.LeftStep(1);
	if ( !leftPos.IsOverEnd() )
	{
		if ( curSquares[leftPos.m_y][leftPos.m_x]->m_right != curSquares[curPos.m_y][curPos.m_x]->m_left )
		{
			return false;
		}
	}

	PlanarPosition upPos = curPos.UpStep(1);
	if ( !upPos.IsOverEnd() )
	{
		if ( curSquares[upPos.m_y][upPos.m_x]->m_bottom != curSquares[curPos.m_y][curPos.m_x]->m_top )
		{
			return false;
		}
	}
	return true;
}


// 搜索可能的组合.
bool Search( Square *** curSquares,int gameSize, PlanarPosition curPos, Square ** allSquares, CentrosymmetricPlan& optimizeSquare )
{
	// 如果当前要搜索的位置已经超出了范围,说明这个解已经找到了.
	if ( curPos.IsOverEnd() )
	{
		return true;
	}

	//if ( counter > maxNum )
	//{
	//	return false;
	//}

	for ( int i=0; i<gameSize*gameSize; i++ )
	{
		if ( !allSquares[i]->m_bUsed && optimizeSquare.GetPoint(i, curPos) )
		{
			// 尝试在当前位置放置当前块.(循环4次把这个块都转一遍.
			allSquares[i]->m_bUsed = true;


			curSquares[curPos.m_y][curPos.m_x] = allSquares[i];
			for ( int round =0; round<4; round++ )
			{
				bool bResult = CheckValid( curSquares, curPos, gameSize );
				if ( bResult )
				{
					// 尝试下一个位置.
					PlanarPosition nextPos = curPos;
					nextPos ++;

	//				OutPutTheResult( curSquares, gameSize, curPos );
	//				cout << "once" << endl;
					bResult = Search( curSquares, gameSize, nextPos, allSquares, optimizeSquare );
					if (  bResult )
					{
						return true;
					}
				}
				(*allSquares[i]) ++;
			}

			// 这个块试过了不行.将这个块已经使用的标志清除.
			allSquares[i]->m_bUsed = false;

			// 剪枝..
			if ( curPos.m_y == 0 && curPos.m_x == 0 )
			{
				optimizeSquare.SetPoint( i, false, curPos);
			}

		}
	}
	

	return false;
}

void OutPutTheResult(Square *** curSquare, int gameSize , PlanarPosition curPos )
{
	for ( int y=0; y<curPos.m_y; y++ )
	{
		for ( int x=0; x<gameSize; x++ )
		{
			cout << " " << curSquare[y][x]->m_top << " | ";
		}

		cout << endl;

		for ( int x=0; x<gameSize; x++ )
		{
			cout << curSquare[y][x]->m_left << " " << curSquare[y][x]->m_right << "| ";
		}

		cout << endl;

		for ( int x=0; x<gameSize; x++ )
		{
			cout << " " << curSquare[y][x]->m_bottom << " | ";
		}

		cout << endl;
		cout << " --   "<< endl;
	}

	for ( int x=0; x<=curPos.m_x; x++ )
	{
		cout << " " << curSquare[curPos.m_y][x]->m_top << " | ";
	}

	cout << endl;

	for ( int x=0; x<=curPos.m_x; x++ )
	{
		cout << curSquare[curPos.m_y][x]->m_left << " " << curSquare[curPos.m_y][x]->m_right << "| ";
	}

	cout << endl;

	for ( int x=0; x<=curPos.m_x; x++ )
	{
		cout << " " << curSquare[curPos.m_y][x]->m_bottom << " | ";
	}

	cout << endl;
	cout << " --   "<< endl;
	cout << "------------------------------------------------" << endl;
}

void OutPutTheResult( Square *** curSquare, int gameSize )
{
	for ( int y=0; y<gameSize; y++ )
	{
		for ( int x=0; x<gameSize; x++ )
		{
			cout << " " << curSquare[y][x]->m_top << " | ";
		}

		cout << endl;

		for ( int x=0; x<gameSize; x++ )
		{
			cout << curSquare[y][x]->m_left << " " << curSquare[y][x]->m_right << "|";
		}

		cout << endl;

		for ( int x=0; x<gameSize; x++ )
		{
			cout << " " << curSquare[y][x]->m_bottom << " | ";
		}

		cout << endl;
		cout << " --   "<< endl;
	}
}

int main()
{

	int gameSize;
	
	Square ***squaresArray;

	Square **allSquares;

	int gameIndex = 0;

	while( cin >> gameSize )
	{
		gameIndex ++;

		if( gameSize == 0 )
		{
			return 0;
		}

		allSquares = Init( gameSize );

			//5 9 1 4
			//4 4 5 6
			//6 8 5 4
			//0 4 4 3
		//allSquares = new Square *[4];
		//allSquares[0] = new Square( 5, 9, 1 ,4 );
		//allSquares[1] = new Square( 4, 4, 5 ,6 );
		//allSquares[2] = new Square( 6, 8, 5 ,4 );
		//allSquares[3] = new Square( 0, 4, 4 ,3 );

		//allSquares = new Square*[25];
		//for ( int i=0; i<25; i++ )
		//{
		//	allSquares[i] = new Square( i, i+1 , i+2 , i+3 );
		//}

		squaresArray = InitSquares( gameSize );
		
		
		cout << "Game " << gameIndex << ": ";

		PlanarPosition curPos( gameSize );

		CentrosymmetricPlan optimizeSquare( gameSize );

		bool bResult = Search( squaresArray,gameSize, curPos, allSquares, optimizeSquare );
		
		/*if ( counter > maxNum - 10 )
		{
			cout << "Possible" << endl;			
		}
		else*/
		{
			if( bResult )
			{
				cout << "Possible" << endl;


				//OutPutTheResult( squaresArray, gameSize );
			}
			else
			{
				cout << "Impossible" <<endl;
			}
		}
		
		
	
//		cout << counter << endl;
//		counter = 0;

		Destroy( squaresArray, gameSize );

		for( int i=0; i<gameSize; i++ )
		{
			delete allSquares[i];
		}
		delete[] allSquares;

	}



	return 0;
}


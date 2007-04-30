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

//////////////////////////////////////////////////////////////////////////
// 更新日志.

// 2007.4.30
//	(1). 查找有两个目标: 
//	1	2
//	3	x
// 如上图,查找x的时候,只会查找x的左边是否和3的右边相等. x的上边是否和2的下边相等.
// 籍此,将类哈希的查询结构展成两个,进一步提升性能.



// 2007.4.29..
// 有很长时间没有弄这些东西了.隔了几个月,觉得想法成熟了些.
// 利用哈希的原理优化查找过程.这样应该会有数量级上的性能提升.
// 好像这些题目普遍的一个特点就是"小问题".按"正常"方式解决的话耗费的内存很小.
// 而它给出的内存使用量限制却很大.一般的是32M..但实际程序使用的内存只在K的数量级上.
// 以空间换时间,貌似是做这种题目的王道.
// 详细:
// 最快的数据结构,当然是普通的数组了.用数组套数组再套数组..
// 层次: 类哈希数组--(方块序号,方块边序号)的数组.
// 这个数组的长度,放在另一个数组里.
// 

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
#include <math.h>

#include <windows.h>

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

	PlanarPosition getNextPos()
	{
		PlanarPosition nextPos(m_planSize);
		nextPos = *this;
		nextPos ++;
		return nextPos;
	}

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
// 优化
class Square
{
public:
	
	Square( const Square& another )
	{
		*this = another;
	}
	
	Square(  int top, int right , int bottom, int left);
	~Square();

	// 顺时针旋转1下.
	Square &operator ++(int);

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

//////////////////////////////////////////////////////////////////////////////////////////////////

// 用空间换时间的快速查找类..
class SearchQuickly
{
public:
	SearchQuickly( int size )
		:m_optimizeSquare( size )
	{
		m_gameSize = size;
		m_squareCount = m_gameSize * m_gameSize;
		this->Init();
		this->InitSquares();
		this->InitHashTable();

		// 把初始的状态打出来..
		for ( int i=0; i<m_gameSize; i++ )
		{
			for ( int k=0; k<m_gameSize; k++ )
			{
				this->m_gameModel[i][k] = this->m_squareNodeArray[i*m_gameSize + k].m_square[3];
			}
		}
		this->OutPutTheResult();

	}
	~SearchQuickly()
	{
		for( int i=0; i<m_gameSize; i++ )
		{
			delete m_gameModel[i];
		}

		delete[] this->m_squareNodeArray;
		delete[] m_gameModel;		
	}

	// 递归的开始..
	bool Search( )
	{
		PlanarPosition beginPos( m_gameSize );
		PlanarPosition nextPos = beginPos.getNextPos();
		for ( int i=0; i<this->m_squareCount; i++ )
		{
			if ( this->m_optimizeSquare.GetPoint( i, beginPos ) )
			{
				this->m_optimizeSquare.SetPoint( i, false, beginPos );
				for ( int k=0; k<4; k++ )
				{
					this->m_gameModel[beginPos.m_y][beginPos.m_x] = this->m_squareNodeArray[i].m_square[k];
					this->m_squareNodeArray[i].m_isUsed = true;
					if ( this->Search( nextPos ) )
					{
						return true;
					}
					else
					{
						this->m_squareNodeArray[i].m_isUsed = false;
					}
				}
			}
		}
		return false;
	}

	void OutPutTheResult()
	{
		for ( int y=0; y<m_gameSize; y++ )
		{
			for ( int x=0; x<m_gameSize; x++ )
			{
				cout << " " << m_gameModel[y][x]->m_top << " | ";
			}

			cout << endl;

			for ( int x=0; x<m_gameSize; x++ )
			{
				cout << m_gameModel[y][x]->m_left << " " << m_gameModel[y][x]->m_right << "|";
			}

			cout << endl;

			for ( int x=0; x<m_gameSize; x++ )
			{
				cout << " " << m_gameModel[y][x]->m_bottom << " | ";
			}

			cout << endl;
			cout << " --   "<< endl;
		}
	}
protected:
	// 搜索可能的组合.
	bool Search( PlanarPosition curPos )
	{		
		// 如果当前要搜索的位置已经超出了范围,说明这个解已经找到了.
		if ( curPos.IsOverEnd() )
		{
			return true;
		}

		PlanarPosition leftPos = curPos.LeftStep(1);
		PlanarPosition upPos = curPos.UpStep(1);
		PlanarPosition nextPos = curPos.getNextPos();

		Square *leftSquare = 0;
		Square *upSquare  = 0;
		
		if ( !leftPos.IsOverEnd() )
		{
			leftSquare = this->m_gameModel[leftPos.m_y][leftPos.m_x];
		}

		if ( !upPos.IsOverEnd() )
		{
			upSquare = this->m_gameModel[upPos.m_y][upPos.m_x];
		}
		
		if ( !leftPos.IsOverEnd() )
		{
			int numberCount = this->m_hashTableLeft[leftSquare->m_right].m_Count;
			for ( int i=0; i<numberCount; i++ )
			{
				SquareNode *curNode = &this->m_squareNodeArray[ this->m_hashTableLeft[leftSquare->m_right].m_hashTable[i][0] ];
				Square *curSquare = curNode->m_square[ this->m_hashTableLeft[leftSquare->m_right].m_hashTable[i][1] ];
				// 看看上边是否合适..
				if ( ( !curNode->m_isUsed ) 					
					&& this->m_optimizeSquare.GetPoint( this->m_hashTableLeft[leftSquare->m_right].m_hashTable[i][0], curPos ) 
					&& ( ( upPos.IsOverEnd() || upSquare->m_bottom == curSquare->m_top ) )
					)
				{
					// 这个可以..
					this->m_gameModel[curPos.m_y][curPos.m_x] = curSquare;
					curNode->m_isUsed = true;			


					//this->/*OutPutTheResult*/( curPos );


					// 搜索下一个..						
					if ( this->Search( nextPos ) )
					{
						return true;
					}
					else
					{
						// 没有找到,释放,找下一个.
						curNode->m_isUsed = false;
			//			this->m_optimizeSquare.SetPoint( this->m_hashTableLeft[leftSquare->m_right].m_hashTable[i][0], false, curPos);
					}
				}
				
			}
			
		}
		else
		{			
			if ( !upPos.IsOverEnd() )
			{
				int numberCount = this->m_hashTableTop[upSquare->m_bottom].m_Count;
				for ( int i=0; i<numberCount; i++ )
				{
					SquareNode *curNode = &this->m_squareNodeArray[ this->m_hashTableTop[upSquare->m_bottom].m_hashTable[i][0] ];
					Square *curSquare = curNode->m_square[ this->m_hashTableTop[upSquare->m_bottom].m_hashTable[i][1] ];
					// 看看是否合适..
					if ( !curNode->m_isUsed 
						&& this->m_optimizeSquare.GetPoint( this->m_hashTableTop[upSquare->m_bottom].m_hashTable[i][0], curPos ))
					{
						// 这个可以..
						this->m_gameModel[curPos.m_y][curPos.m_x] = curSquare;
						curNode->m_isUsed = true;

						//this->OutPutTheResult( curPos );


						// 搜索下一个..						
						if ( this->Search( nextPos ) )
						{
							return true;
						}
						else
						{
							// 没有找到,释放,找下一个.
							curNode->m_isUsed = false;
						//	this->m_optimizeSquare.SetPoint( this->m_hashTableTop[upSquare->m_bottom].m_hashTable[i][0], false, curPos);
						}
					}
				}
			}
			else
			{
				std::cout << "非常的不对劲!!!" << std::endl;
			}
		}

		return false;
	}

	void InitSquares( )
	{
		m_gameModel = new Square**[m_gameSize];
		for( int i=0; i<m_gameSize; i++ )
		{
			m_gameModel[i] = new Square *[m_gameSize];
		}
	}
	// 初始化.
	// And loop to load all the squares.
	void Init( )
	{
		this->m_squareNodeArray = new SquareNode[ this->m_squareCount ];
	}

	// 初始化类哈希数据结构.
	void InitHashTable()
	{
		for ( int squareIndex=0; squareIndex<this->m_squareCount; squareIndex ++ )
		{
			for ( int i=0; i<4; i++ )
			{
				int topValue = this->m_squareNodeArray[squareIndex].m_square[i]->m_top;
				int temp[2] = { squareIndex, i };
				this->m_hashTableTop[topValue].AddNode( temp );

				int leftValue = this->m_squareNodeArray[squareIndex].m_square[i]->m_left;
				this->m_hashTableLeft[leftValue].AddNode( temp );
			}
		}
	}
	void OutPutTheResult( PlanarPosition curPos )
	{
		for ( int y=0; y<curPos.m_y; y++ )
		{
			for ( int x=0; x<this->m_gameSize; x++ )
			{
				cout << " " << m_gameModel[y][x]->m_top << " | ";
			}

			cout << endl;

			for ( int x=0; x<m_gameSize; x++ )
			{
				cout << this->m_gameModel[y][x]->m_left << " " << m_gameModel[y][x]->m_right << "| ";
			}

			cout << endl;

			for ( int x=0; x<m_gameSize; x++ )
			{
				cout << " " << m_gameModel[y][x]->m_bottom << " | ";
			}

			cout << endl;
			cout << " --   "<< endl;
		}

		for ( int x=0; x<=curPos.m_x; x++ )
		{
			cout << " " << m_gameModel[curPos.m_y][x]->m_top << " | ";
		}

		cout << endl;

		for ( int x=0; x<=curPos.m_x; x++ )
		{
			cout << m_gameModel[curPos.m_y][x]->m_left << " " << m_gameModel[curPos.m_y][x]->m_right << "| ";
		}

		cout << endl;

		for ( int x=0; x<=curPos.m_x; x++ )
		{
			cout << " " << m_gameModel[curPos.m_y][x]->m_bottom << " | ";
		}

		cout << endl;
		cout << " --   "<< endl;
		cout << "------------------------------------------------" << endl;
	}
	

private:
	enum { MaxNumber = 10 };
	enum { MaxSquare = 100 };

	// 游戏内存模型.
	Square *** m_gameModel;

	// 一个方块.
	class SquareNode
	{
	public:
		
		SquareNode()
		{
			m_isUsed = false;

			/*int top, right, bottom, left;
			cin >> top >>  right  >>  bottom >> left ;	
			Square temp(  top, right, bottom, left );	*/

			/*static int kkk = 0;
			kkk ++;*/
		//	Square temp(  rand()%9,rand()%9,rand()%9,rand()%9 );

			int limit = 9;
			Square temp(  rand()%limit,rand()%limit,rand()%limit,rand()%limit );	

			for ( int k=0; k<4; k++ )
			{
				m_square[k] = new Square( temp++ );
			}			
		}

		~SquareNode()
		{
			for ( int i=0; i<4; i++ )
			{
				delete m_square[i];
			}
		}
		// 四种形态的方块..
		Square * m_square[4];
		bool m_isUsed;
	protected:
	private:
	};
	// 所有的方块..
	SquareNode *m_squareNodeArray;

	// 用于优化性能的一个中心对称矩阵...可以防止一个方块在中心对称的位置上重复试.
	CentrosymmetricPlan m_optimizeSquare;
	
	class HashTableNode
	{
	public:
		HashTableNode()
		{
			m_Count = 0;
		}

		// 添加一项..
		void AddNode( int* oneSquare )
		{
			m_hashTable[m_Count][0] = oneSquare[0];
			m_hashTable[m_Count][1] = oneSquare[1];
			m_Count ++;
		}
		// 这个数的数量..
		int m_Count;
		// 数据.
		int m_hashTable[MaxSquare][2];
	protected:
	private:
	};
	// 用于优化查找性能的类哈希结构.
	HashTableNode m_hashTableTop[MaxNumber];
	HashTableNode m_hashTableLeft[MaxNumber];



	// 大小..
	int m_gameSize;
	// 数目.
	int m_squareCount;
};


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







int main()
{

	int gameSize;
	int gameIndex = 0;
	while( cin >> gameSize )
	{
		int beginTime = ::GetTickCount();
		gameIndex ++;
		if( gameSize == 0 )
		{
			return 0;
		}
		SearchQuickly finder( gameSize );

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

		cout << "Game " << gameIndex << ": ";
		bool bResult = finder.Search();
		
		int endTime = ::GetTickCount();

		cout << "time" << (endTime-beginTime) << std::endl;

		if( bResult )
		{
			cout << "Possible" << endl;


			finder.OutPutTheResult();
		}
		else
		{
			cout << "Impossible" <<endl;
		}
	}



	return 0;
}


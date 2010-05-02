// GnomeTetravex.cpp : �������̨Ӧ�ó������ڵ㡣
//

// ���Ż��İ汾��ʱ�޷�ͨ��..
// �Ż�����:
// ������������ĶԳƵ�.
// ������е�һ��������ĳ�����޷��ҵ�һ���������������,��ô,�����������������ת��Ӧ����Ҳ�����ҵ��������������.
// ֤��: ��֤.��������ҵ�һ�����,��ôͨ����ת��������,�����ҵ� �޷��ҵ�����������ϵ��Ǹ����һ���������������..
// ����Ż�ֻ����Կշ���ĶԳ�����.���Խ�Լ���һ���ʱ��..

// CentrosymmetricPlan�������ڴ����Գƹ�ϵ����.
// ָ��Size����һ������.
// SetPoint��ʱ��,�ͽ���һ�����һ�����ת�ԳƵ㶼��λ.
// �ԳƵ���: һ���� (x,y), ����Size = s. ��ô�ĸ�����: (x,y), (y,-x+s) (-x+s, -y+s) (-y+s, x)

//////////////////////////////////////////////////////////////////////////
// ������־.

// 2007.5.9.
// ��ʶ��,����һ��û���������Ŀ.
// ��Ҫ�ϲ���ͬ�Ŀ�....������ͬ�Ŀ���ͬһ���ط���������..

// 2007.5.1. 0:22.
// ��Ȼ��ʱ..
// ����һ����Ŭ��:
// ���������,ȷ�������ܹ����ڵ�λ��..
// λ����3��: �ڲ� ���� ����
// �ֱ�Ϊ������ĸ�����������ķ����ϵ���ͬ����.���鲻���ظ�.����Ŀ�ٵķ��鿪ʼ������.
// ���ҵ�4�������ڲ�����,�п��ܷ��ڷ�����κ�λ��.
// ���ҵ�3�����Ǳ��Ϸ���.�п��ܷ��ڱ��ϻ����.
// ���ڵ�2���������ҵ����� ���Ϸ���,ֻ�ܷ���4������.


// 2007.4.30
//	(1). ����������Ŀ��: 
//	1	2
//	3	x
// ����ͼ,����x��ʱ��,ֻ�����x������Ƿ��3���ұ����. x���ϱ��Ƿ��2���±����.
// ����,�����ϣ�Ĳ�ѯ�ṹչ������,��һ����������.



// 2007.4.29..
// �кܳ�ʱ��û��Ū��Щ������.���˼�����,�����뷨������Щ.
// ���ù�ϣ��ԭ���Ż����ҹ���.����Ӧ�û����������ϵ���������.
// ������Щ��Ŀ�ձ��һ���ص����"С����".��"����"��ʽ����Ļ��ķѵ��ڴ��С.
// �����������ڴ�ʹ��������ȴ�ܴ�.һ�����32M..��ʵ�ʳ���ʹ�õ��ڴ�ֻ��K����������.
// �Կռ任ʱ��,ò������������Ŀ������.
// ��ϸ:
// �������ݽṹ,��Ȼ����ͨ��������.��������������������..
// ���: ���ϣ����--(�������,��������)������.
// �������ĳ���,������һ��������.
// 

// 2007.1.3
// ������������˵����ʱ����(����Cheat)����ͨ��.����jol�����ݵ�ȷ��ǿ.
// ������Ż�����: �ȷ�������ķ���,��������ӵ�����ظ������ֵ���Ŀ��λ��ȷ�������ܷ��õ�λ��.
// ����: ���ĸ����ϵķ�������������������ڵı������ظ����ֵ�.
// �ܷ����ڱ��ϵķ�������������߶����ظ�����,���ܷ����ڲ��ķ����ĸ����ֱ��붼���ظ���.
// ����������ÿ��������ܵ�λ�þ���ȷ����.��������з���û�����ڵ��ظ�����,�Ϳ���ֱ�ӷ��� Impossible ��.
// ��������Ͽ���ʵ�ֲ�����Ե��Ż�:����,�����4��ֻ���������ڱ����ظ����ķ���,��ô���ϾͲ����ܷ������ķ�����.
// �Ż��Ĳ���Ӧ���Ǵ��м俪ʼ����.Ȼ���Ǳ���,����ǽ���.
// ����,ʵ�������Ƚϸ���,�Ȳ��ڲ��������Ż���.

// #include "stdafx.h"

#include <iostream>
#include <math.h>

#ifdef _DEBUG

#include <windows.h>

#endif
#include <map>
#include <vector>

using namespace std;

class Square;
class PlanarPosition;
void OutPutTheResult(Square *** curSquare, int gameSize , PlanarPosition curPos );

// �Ż�����Ϸ����
//class OptimizeGameSquare
//{
//public:
//protected:
//private:
//};


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
// �����Ż������ĶԳ�������.
class CentrosymmetricPlan 
{
public:
	CentrosymmetricPlan( const int& plainSize );
	virtual ~CentrosymmetricPlan();

	void Reset();

	// ����һ�����Լ�������ת��ĵ�..
	// ��������ͬ�ĵ�Ҳ����..
	void SetPoint( int index, bool bValue, int x, int y );
	void SetPoint( int index, bool bValue, PlanarPosition pos );

	// ����������һ����..
	void SetOnePoint( int index, bool bValue, int x, int y );


	// ��ȡһ�����״̬.
	bool GetPoint( int index, int x, int y );
	bool GetPoint( int index, PlanarPosition pos );

	// ���÷���ϲ�map.
	void SetSameSquareMap( std::multimap<int,int> sameSquareMap )
	{
		this->m_sameSquareMap = sameSquareMap;
	}

protected:
	// ��ʵ�ľ���.
	bool *** m_square;

	// ��С.
	int m_size;

	// ��ͬ�������..
	// ӳ���ϵ: ������� -> ��ͬ�������.
	std::multimap<int, int> m_sameSquareMap;
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

void CentrosymmetricPlan::Reset()
{
	for ( int squareCount = 0 ; squareCount< m_size*m_size ; squareCount ++ )
	{
		for ( int i=0; i<m_size; i++ )
		{
			for( int n=0; n<m_size; n++ )
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

void CentrosymmetricPlan::SetOnePoint(int index, bool bValue, int x, int y )
{
	int zeroSize = m_size - 1;
	this->m_square[index][y][x] = bValue;
	this->m_square[index][y][-x+zeroSize] = bValue;
	m_square[index][-x+zeroSize][-y+zeroSize] = bValue;
	m_square[index][-y+zeroSize][x] = bValue;
}

void CentrosymmetricPlan::SetPoint(int index, bool bValue, int x, int y )
{	
	this->SetOnePoint( index, bValue, x, y );
	// ��������ͬ�ĵ��״̬Ҳ������...
	std::pair< std::multimap<int,int>::iterator, std::multimap<int,int>::iterator> targetIterPair = this->m_sameSquareMap.equal_range( index );
	std::multimap<int,int>::iterator iter = targetIterPair.first;
	while ( iter != targetIterPair.second )
	{
		int sameIndex = iter->second;
		this->SetOnePoint( sameIndex, bValue, x, y );
		iter ++;
	}

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
// �Ż�
class Square
{
public:
	
	Square( const Square& another )
	{
		*this = another;
	}
	
	Square(  int top, int right , int bottom, int left);
	~Square();

	// ˳ʱ����ת1��.
	Square &operator ++(int);

	bool CompleteEqual(  const Square& another )
	{
		return( this->m_left==another.m_left && this->m_bottom == another.m_bottom && this->m_right == another.m_right && this->m_top == another.m_top );
	}

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

// �ÿռ任ʱ��Ŀ��ٲ�����..
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

		//Square kkkk( 0,0,0,0 );
		//Square wwww( 0,0,0,0 );

		//bool result = ( kkkk.CompleteEqual( wwww ) );
 
		// ��ʼ������
		for ( int i=0; i<this->m_squareCount; i++ )
		{
			for ( int k=0; k<this->m_squareCount; k++ )
			{
				if ( i!=k )
				{
					// �Ƚϵ�i���͵�k���Ƿ�һ��..
					for ( int j=0; j<4; j++ )
					{
						if ( this->m_squareNodeArray[i].m_square[0]->CompleteEqual( *( this->m_squareNodeArray[k].m_square[j] ) )  )
						{
							// �ҵ���һ����ͬ��..
							this->m_sameSquareMap.insert( std::pair<int,int>( i, k ) );
						}
					}
				}
			}
		}

		this->m_optimizeSquare.SetSameSquareMap( this->m_sameSquareMap );

		// �ѳ�ʼ��״̬�����..
		for ( int i=0; i<m_gameSize; i++ )
		{
			for ( int k=0; k<m_gameSize; k++ )
			{
				this->m_gameModel[i][k] = this->m_squareNodeArray[i*m_gameSize + k].m_square[3];
			}
		}

#ifdef _DEBUG
		this->OutPutTheResult();
#endif
		

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

	// �ݹ�Ŀ�ʼ..
	bool Search( )
	{
		PlanarPosition beginPos( m_gameSize );
		PlanarPosition nextPos = beginPos.getNextPos();
		for ( int i=0; i<this->m_squareCount; i++ )
		{
			if ( this->m_optimizeSquare.GetPoint( i, beginPos ) )
			{
//				this->m_optimizeSquare.SetPoint( i, false, beginPos );
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

						this->m_optimizeSquare.SetPoint( i, false, 0, 0 );
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
	// �������ܵ����.
	bool Search( PlanarPosition curPos )
	{		
		// �����ǰҪ������λ���Ѿ������˷�Χ,˵��������Ѿ��ҵ���.
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
			std::vector<int> clickOutNum;

			int numberCount = this->m_hashTableLeft[leftSquare->m_right].m_Count;
			for ( int i=0; i<numberCount; i++ )
			{
				int curSquareIndex = this->m_hashTableLeft[leftSquare->m_right].m_squareIndex[i];
				SquareNode *curNode = &this->m_squareNodeArray[curSquareIndex];

				if ( !this->m_optimizeSquare.GetPoint( curSquareIndex , curPos ) )
				{
					continue;
				}

				for ( int k=0; k<4; k++ )
				{
					Square *curSquare = this->m_hashTableLeft[leftSquare->m_right].m_hashTable[curSquareIndex][k];
					if( curSquare == 0 )
					{
						break;
					}
					else
					{
						// �����ϱ��Ƿ����..
						
						if ( ( !curNode->m_isUsed ) 					
							&& ( ( upPos.IsOverEnd() || upSquare->m_bottom == curSquare->m_top ) )
							)
						{
							// �������..
							this->m_gameModel[curPos.m_y][curPos.m_x] = curSquare;
							curNode->m_isUsed = true;			

#ifdef _DEBUG
this->OutPutTheResult( curPos );
#endif

							


							// ������һ��..						
							if ( this->Search( nextPos ) )
							{
								return true;
							}
							else
							{
								// û���ҵ�,�ͷ�,����һ��.
								curNode->m_isUsed = false;
								//			this->m_optimizeSquare.SetPoint( this->m_hashTableLeft[leftSquare->m_right].m_hashTable[i][0], false, curPos);
							}
						}
					}
				}

				this->m_optimizeSquare.SetPoint( curSquareIndex, false, curPos );
				clickOutNum.push_back( curSquareIndex );				
			}

			for( int i=0; i<clickOutNum.size(); i++ )
			{
				this->m_optimizeSquare.SetPoint( clickOutNum[i], true, curPos );
			}
			
		}
		else
		{			
			if ( !upPos.IsOverEnd() )
			{
				int numberCount = this->m_hashTableTop[upSquare->m_bottom].m_Count;

				std::vector<int> clickOutNum;

				for ( int i=0; i<numberCount; i++ )
				{
					int curSquareIndex = this->m_hashTableTop[upSquare->m_bottom].m_squareIndex[i];
					SquareNode *curNode = &this->m_squareNodeArray[curSquareIndex];

					if ( this->m_optimizeSquare.GetPoint(curSquareIndex, curPos ) )
					{
						for ( int k=0;k<4; k++ )
						{
							Square *curSquare = this->m_hashTableTop[upSquare->m_bottom].m_hashTable[curSquareIndex][k];
							if ( curSquare == 0 )
							{
								break;
							}
							else
							{
								// �����Ƿ����..
								if ( !curNode->m_isUsed )
								{
									// �������..
									this->m_gameModel[curPos.m_y][curPos.m_x] = curSquare;
									curNode->m_isUsed = true;

#ifdef _DEBUG
this->OutPutTheResult( curPos );
#endif
									


									// ������һ��..						
									if ( this->Search( nextPos ) )
									{
										return true;
									}
									else
									{
										// û���ҵ�,�ͷ�,����һ��.
										curNode->m_isUsed = false;
										//	this->m_optimizeSquare.SetPoint( this->m_hashTableTop[upSquare->m_bottom].m_hashTable[i][0], false, curPos);
									}
								}
							}
						}

						this->m_optimizeSquare.SetPoint( curSquareIndex, false, curPos );
						clickOutNum.push_back( curSquareIndex );
					}					
				}

				for( int i=0; i<clickOutNum.size(); i++ )
				{
					this->m_optimizeSquare.SetPoint( clickOutNum[i], true, curPos );
				}

			}
			else
			{
				std::cout << "�ǳ��Ĳ��Ծ�!!!" << std::endl;
			}
		}

		// ��ǰ���������
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
	// ��ʼ��.
	// And loop to load all the squares.
	void Init( )
	{
		this->m_squareNodeArray = new SquareNode[ this->m_squareCount ];
	}

	// ��ʼ�����ϣ���ݽṹ.
	void InitHashTable()
	{
		for ( int squareIndex=0; squareIndex<this->m_squareCount; squareIndex ++ )
		{
			for ( int i=0; i<4; i++ )
			{
				int topValue = this->m_squareNodeArray[squareIndex].m_square[i]->m_top;
				this->m_hashTableTop[topValue].AddSquare( squareIndex, m_squareNodeArray[squareIndex].m_square[i] );

				int leftValue = this->m_squareNodeArray[squareIndex].m_square[i]->m_left;
				this->m_hashTableLeft[leftValue].AddSquare( squareIndex, this->m_squareNodeArray[squareIndex].m_square[i] );
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

	// ��Ϸ�ڴ�ģ��.
	Square *** m_gameModel;

	// ��ͬ�������..
	// ӳ���ϵ: ������� -> ��ͬ�������.
	std::multimap<int, int> m_sameSquareMap;

	// һ������.
	class SquareNode
	{
	public:
		
		SquareNode()
		{
			m_isUsed = false;


			

			/*static int kkk = 0;
			kkk ++;*/
		//	Square temp(  rand()%9,rand()%9,rand()%9,rand()%9 );

#ifdef _DEBUG
			int limit = 1;
			Square temp(  rand()%limit,rand()%limit,rand()%limit,rand()%limit );	

			static int kkk = 0;
			kkk ++;
			if ( kkk == 25 )
			{
				temp.m_bottom = temp.m_left = temp.m_right = temp.m_top = 2;
			}
#else
			int top, right, bottom, left;
			cin >> top >>  right  >>  bottom >> left ;	
			Square temp(  top, right, bottom, left );	
#endif
			

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
		// ������̬�ķ���..
		Square * m_square[4];
		bool m_isUsed;
	protected:
	private:
	};
	// ���еķ���..
	SquareNode *m_squareNodeArray;

	// �����Ż����ܵ�һ�����ĶԳƾ���...���Է�ֹһ�����������ĶԳƵ�λ�����ظ���.
	CentrosymmetricPlan m_optimizeSquare;

	
	class HashTableNode
	{
	public:
		HashTableNode()
		{
			m_Count = 0;
			//memset( m_hashTable, 0, M)
			for ( int i=0; i<MaxSquare; i++ )
			{
				for( int k=0; k<4; k++ )
				{
					this->m_hashTable[i][k] = 0;
				}
			}
		}

		// ����һ��..
		void AddSquare( int squareIndex, Square* oneSquare )
		{
			bool bHas = false;
			for ( int i=0; i<m_Count; i++ )
			{
				if ( m_squareIndex[i] == squareIndex )
				{
					bHas = true;
					break;
				}
			}
			this->m_squareIndex[m_Count] = squareIndex;
			for ( int i=0; i<4; i++ )
			{
				if ( this->m_hashTable[squareIndex][i] == 0 )
				{
					this->m_hashTable[squareIndex][i] = oneSquare;
					break;
				}
			}
			
			if ( !bHas )
			{
				m_Count ++;
			}			
		}
		//// �����������..
		int m_Count;
		// ����.
		Square * m_hashTable[MaxSquare][4];
		int m_squareIndex[MaxSquare];
	protected:
	private:
	};

	// �����Ż��������ܵ����ϣ�ṹ.
	HashTableNode m_hashTableTop[MaxNumber];
	HashTableNode m_hashTableLeft[MaxNumber];



	// ��С..
	int m_gameSize;
	// ��Ŀ.
	int m_squareCount;
};


// ���Ϸ���.
// ֻ��鵱ǰλ�õķ��������з������Ƿ�Ϸ�.
// ����: �ŷ����˳�����Ǵ����ϵ����µ�˳��,����,Ĭ�Ϸ�����ұߺ��±�һ��û�������ķ���.
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
#ifdef _DEBUG
int beginTime = ::GetTickCount();
#endif
		
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
		
#ifdef _DEBUG
		int endTime = ::GetTickCount();

		cout << "time" << (endTime-beginTime) << std::endl;
#endif
		

		if( bResult )
		{
			cout << "Possible" << endl;

#ifdef _DEBUG
		finder.OutPutTheResult();
#endif
			
		}
		else
		{
			cout << "Impossible" <<endl;
		}
	}



	return 0;
}

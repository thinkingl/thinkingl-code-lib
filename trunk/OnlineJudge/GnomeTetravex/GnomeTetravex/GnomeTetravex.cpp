// GnomeTetravex.cpp : �������̨Ӧ�ó������ڵ㡣
//

// ���Ż��İ汾��ʱ�޷�ͨ��..
// �Ż�����:
// ������������ĶԳƵ�.
// ������е�һ��������ĳ�����޷��ҵ�һ���������������,��ô,�����������������ת��Ӧ����Ҳ�����ҵ��������������.
// ֤��: ��֤.��������ҵ�һ�����,��ôͨ����ת��������,�����ҵ� �޷��ҵ�����������ϵ��Ǹ����һ���������������..
// ����Ż�ֻ����Կշ���ĶԳ�����.���Խ�Լ���һ���ʱ��..

// CentrosymmetricPlan�������ڴ���Գƹ�ϵ����.
// ָ��Size����һ������.
// SetPoint��ʱ��,�ͽ���һ�����һ�����ת�ԳƵ㶼��λ.
// �ԳƵ���: һ���� (x,y), ����Size = s. ��ô�ĸ�����: (x,y), (y,-x+s) (-x+s, -y+s) (-y+s, x)

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
using namespace std;

class Square;
class PlanarPosition;
void OutPutTheResult(Square *** curSquare, int gameSize , PlanarPosition curPos );

// �Ż�����Ϸ����
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
// �����Ż������ĶԳ�������.
class CentrosymmetricPlan 
{
public:
	CentrosymmetricPlan( const int& plainSize );
	virtual ~CentrosymmetricPlan();

	// ����һ�����Լ�������ת��ĵ�..
	void SetPoint( int index, bool bValue, int x, int y );
	void SetPoint( int index, bool bValue, PlanarPosition pos );
	// ��ȡһ�����״̬.
	bool GetPoint( int index, int x, int y );
	bool GetPoint( int index, PlanarPosition pos );

protected:
	// ��ʵ�ľ���.
	bool *** m_square;

	// ��С.
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

	// ˳ʱ����ת1��.
	Square &operator ++(int);

	// �Ƿ��Ѿ�ʹ��.
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

// ��ʼ��.
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


// �������ܵ����.
bool Search( Square *** curSquares,int gameSize, PlanarPosition curPos, Square ** allSquares, CentrosymmetricPlan& optimizeSquare )
{
	// �����ǰҪ������λ���Ѿ������˷�Χ,˵��������Ѿ��ҵ���.
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
			// �����ڵ�ǰλ�÷��õ�ǰ��.(ѭ��4�ΰ�����鶼תһ��.
			allSquares[i]->m_bUsed = true;


			curSquares[curPos.m_y][curPos.m_x] = allSquares[i];
			for ( int round =0; round<4; round++ )
			{
				bool bResult = CheckValid( curSquares, curPos, gameSize );
				if ( bResult )
				{
					// ������һ��λ��.
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

			// ������Թ��˲���.��������Ѿ�ʹ�õı�־���.
			allSquares[i]->m_bUsed = false;

			// ��֦..
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


#include "Sudoku.h"
#include <assert.h>
#include <iostream>

using namespace std;

CSudoku::CSudoku( int nSize )
{
	assert( nSize == 9 );
	m_nSudokuSize = nSize;
	m_arSudokuData = new int[ nSize * nSize ];
}


CSudoku::~CSudoku(void)
{
	delete[] m_arSudokuData;
	m_arSudokuData = 0;
}

int CSudoku::GetNum( int row, int col ) const
{
	int nIndex = this->GetIndex( row, col );
	return m_arSudokuData[ nIndex ];
}

int CSudoku::GetIndex( int row, int col ) const
{
	int nIndex = row * m_nSudokuSize  + col;
	return nIndex;
}

bool CSudoku::SetNumAndCheck( int row, int col, int value )
{
	bool bOk = this->Check( value, row, col );
	if ( bOk )
	{
		int nIndex = this->GetIndex( row, col );
		this->m_arSudokuData[ nIndex ] = value;
	}
	return bOk;
}

bool CSudoku::Check( int value, int row, int col )const
{
	// 是空格,那肯定没问题..
	if ( value == 0 )
	{
		return true;
	}

	// 所在行和列不能重复.
	for ( int i=0; i<m_nSudokuSize; ++i )
	{
		int numR = 0;
		int numC = 0;
		if ( i != col )
		{
			numR = this->GetNum( row, i );
		}
		
		if ( i!= row )
		{
			numC = this->GetNum( i, col );
		}
		if ( value == numR || value == numC )
		{
			return false;
		}
	}
	
	// 所在的9宮格.
	int rowBegin = row / 3 * 3;
	int colBegin = col / 3 * 3;
	for ( int i=0; i<3; ++i )
	{
		for ( int k=0; k<3; ++k )
		{
			int num = this->GetNum( rowBegin + i, colBegin + k );
			if ( value == num )
			{
				return false;
			}
		}
	}

	return true;
}

bool CSudoku::Next( int& row, int& col ) const
{
	col++;
	if ( col == m_nSudokuSize )
	{
		col = 0;
		row ++;
	}
	if ( row == m_nSudokuSize )
	{
		return false;
	}
	return true;
}

void CSudoku::DumpToCout() const
{
	std::cout << "***************************" << endl;
	for( int i=0; i<m_nSudokuSize; ++i )
	{
		for ( int k=0; k<m_nSudokuSize; ++k )
		{
			std::cout << this->GetNum( i, k ) << "\t";
		}
		std::cout << "\n";
	}
	std::cout << "*************************" << endl;
}
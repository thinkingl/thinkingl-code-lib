#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <math.h>
#include <set>
#include <sstream>
#include <algorithm>
#include <memory.h>
#include <complex>
#include <queue>
#include <stack>

#include "Sudoku.h"
#include <assert.h>

using namespace std;

#ifdef _WIN32
typedef __int64 n64;
typedef unsigned __int64 u64;
#else
typedef long long n64;
typedef unsigned long long u64;
#endif
typedef unsigned long u32;


bool DFS( CSudoku& sudoku , int row, int col )
{
	sudoku.DumpToCout();

	if ( 0 == sudoku.GetNum( row, col ) )
	{
		int oldRow = row;
		int oldCol = col;
		for ( int i=1; i<=9; ++i )
		{
			if ( sudoku.SetNumAndCheck( row, col, i ) )
			{
				bool bOk = sudoku.Next( row, col );
				if ( bOk )
				{
					bOk = DFS( sudoku, row, col );
					if ( bOk )
					{
						return bOk;
					}
				}			 
			}
		}
		// 还原.
		sudoku.SetNumAndCheck( oldRow, oldCol, 0 );
		return false;	// 从1到9都不行,那是真不行了.
	}
	else
	{
		// 试下一个点.
		if( sudoku.Next( row, col ) )
		{
			return DFS( sudoku, row, col );
		}
		else
		{
			return true;	// 都试完了,通过!!!
		}
	}
}

int main()
{
	// 读取数独阵列.
	const string strInFile = "sudoku.txt"; 
	ifstream fin ( strInFile.c_str() );

	int nSudokuSize;
	fin >> nSudokuSize;
	assert( nSudokuSize == 9 );
	
	CSudoku sudoku( nSudokuSize );

	for ( int i=0; i<nSudokuSize; ++i )
	{
		for ( int k=0; k<nSudokuSize; ++k )
		{
			int nNum;
			fin >> nNum;
			bool bOk = sudoku.SetNumAndCheck( i, k, nNum );
			assert( bOk );
			if ( !bOk )
			{
				cout << "Invalid Data!!!!!" << endl;
			}
		}
	}

	sudoku.DumpToCout();

	// 搜索.
	bool bOk = DFS( sudoku, 0, 0 );
	if ( bOk )
	{
		// 输出结果.
		sudoku.DumpToCout();
	}
	else
	{
		cout << "Invalid sudoku!!!!" << endl;
	}

	return 0;
}
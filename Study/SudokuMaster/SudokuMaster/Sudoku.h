#pragma once

class CSudoku
{
public:
	CSudoku( int nSize );
	~CSudoku();
	
	/** 获取数独数列中的值. */
	int GetNum( int row, int col )const;

	/** 设置某个值. 带校验. */
	bool SetNumAndCheck( int row, int col, int value );

	bool Next( int& row, int& col )const;

	/** 打印出来.*/
	void DumpToCout()const;

private:
	int GetIndex( int row, int col ) const;

	bool Check( int value, int row, int col )const;
private:
	int *m_arSudokuData;
	int m_nSudokuSize;
};

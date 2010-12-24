#pragma once

class CSudoku
{
public:
	CSudoku( int nSize );
	~CSudoku();
	
	/** ��ȡ���������е�ֵ. */
	int GetNum( int row, int col )const;

	/** ����ĳ��ֵ. ��У��. */
	bool SetNumAndCheck( int row, int col, int value );

	bool Next( int& row, int& col )const;

	/** ��ӡ����.*/
	void DumpToCout()const;

private:
	int GetIndex( int row, int col ) const;

	bool Check( int value, int row, int col )const;
private:
	int *m_arSudokuData;
	int m_nSudokuSize;
};

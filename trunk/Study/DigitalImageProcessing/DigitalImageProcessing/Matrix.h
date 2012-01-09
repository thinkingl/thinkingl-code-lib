#pragma once

/** Matrix
*	Created by thinkingl@20120105.
*/
#include <vector>
#include <assert.h>

template< typename _TypeOfMatrix>
class CMatrix
{
public:
	typedef std::vector< _TypeOfMatrix > CMatrixDataList;
public:
	inline CMatrix(void);

	inline CMatrix( const CMatrix& another );

	/**	构造且填充单一的初始值. */
	inline CMatrix( int width, int height, const _TypeOfMatrix& zeroValue, const _TypeOfMatrix& unitValue );

	/**	构造赋予初始值. */
	inline CMatrix( int width, int height, const CMatrixDataList& defaultValue );

	inline virtual ~CMatrix(void);

	inline int GetWidth() const;
	inline int GetHeight() const;

	inline const _TypeOfMatrix& Value( int x, int y ) const;
	inline _TypeOfMatrix& Value( int x, int y );

	inline CMatrix& operator = ( const CMatrix& another );
	inline CMatrix operator *( const CMatrix& another ) const;

	_TypeOfMatrix* operator[]( int rowIndex );
	//friend ;

	/**	求逆矩阵. 
	*	可逆返回true, 不可逆返回false.
	*/
	inline bool GetInverse( CMatrix& inverseMatrix ) const;

	/**	求行列式. 
	*	返回值表示矩阵是否是行列式.
	*/
	inline bool GetDeterminant( _TypeOfMatrix& determinantValue ) const;

	inline _TypeOfMatrix GetAlgebraicCofactor( int x, int y ) const;
private:
	int m_width;
	int m_height;
	CMatrixDataList m_matrixData;
	_TypeOfMatrix m_zeroValue;	// 表示 0 的值.( 零元 )
	_TypeOfMatrix m_unitValue;		// 表示 1 的值.( 单位元 )
};

/** 矩阵和值的乘法. */
template< typename _TypeOfAlpha, typename _TypeOfMatrix>
inline CMatrix<_TypeOfMatrix> operator *( const _TypeOfAlpha& alpha, const CMatrix<_TypeOfMatrix> & m1 )
{
	CMatrix<_TypeOfMatrix> m = m1;
	for(long i = 0; i < m.GetHeight(); i++)
	{
		for(long j = 0; j < m.GetWidth(); j++)
		{
			m.Value( i, j ) = alpha * m1.Value( i, j );
		}
	}
	return m;
}

template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix>::CMatrix() : m_width(0), m_height(0), m_zeroValue(0), m_unitValue( 1 )
{}

template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix>::~CMatrix()
{}

template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix>::CMatrix( const CMatrix<_TypeOfMatrix>& another ) 	
	: m_zeroValue( another.m_zeroValue )
	, m_unitValue( another.m_unitValue )
{
	this->operator = ( another );
}

template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix>::CMatrix( int width, int height, const CMatrixDataList& data ) 
	: m_width( width ), m_height( height )
	, m_matrixData( data ), m_zeroValue( 0 ), m_unitValue( 1 )
{}

template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix>::CMatrix( int width, int height, const _TypeOfMatrix& zeroValue, const _TypeOfMatrix& unitValue ) 
	: m_width( width ), m_height( height )
	, m_matrixData( width*height, zeroValue ), m_zeroValue( zeroValue ), m_unitValue( unitValue )
{}

template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix>& CMatrix<_TypeOfMatrix>::operator =( const CMatrix<_TypeOfMatrix>& another )
{
	if( this == &another )
	{
		return *this;
	}
	this->m_width = another.GetWidth();
	this->m_height = another.GetHeight();
	this->m_zeroValue = another.m_zeroValue;
	this->m_unitValue = another.m_unitValue;
	this->m_matrixData = another.m_matrixData;
	return *this;
}


template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix> CMatrix<_TypeOfMatrix>::operator *( const CMatrix<_TypeOfMatrix>& another ) const
{
	assert( this->GetWidth() == another.GetHeight() );	// 矩阵乘法要求.
	
	int newWidth = another.GetWidth();
	int newHeight = this->GetHeight();
	int equalPart = this->GetWidth();

	CMatrix<_TypeOfMatrix> newMatrix( newWidth, newHeight, m_zeroValue, m_unitValue );
	for ( int y=0; y<newHeight; y++ )
	{
		for( int x=0; x<newWidth; x++ )
		{
			_TypeOfMatrix valueOfwh = m_zeroValue;
			for( int i=0; i<equalPart; i++ )
			{
				valueOfwh += this->Value( i, y ) * another.Value( x, i );
			}
			newMatrix.Value( x,y ) = valueOfwh;
		}
	}
	return newMatrix;
}

template< typename _TypeOfMatrix>
_TypeOfMatrix* CMatrix<_TypeOfMatrix>::operator[]( int rowIndex )
{
	return &m_matrixData[ GetWidth() * rowIndex ];
}

template< typename _TypeOfMatrix>
int CMatrix<_TypeOfMatrix>::GetWidth() const
{
	return m_width;
}

template< typename _TypeOfMatrix>
int CMatrix<_TypeOfMatrix>::GetHeight() const
{
	return m_height;
}

template< typename _TypeOfMatrix>
const _TypeOfMatrix& CMatrix<_TypeOfMatrix>::Value( int x, int y ) const
{
	if ( x<0 || x>GetWidth() || y<0 || y>GetHeight() )
	{
		assert( false );
		return m_zeroValue;
	}
	return m_matrixData.at( x + this->GetWidth() * y );
}

template< typename _TypeOfMatrix>
_TypeOfMatrix& CMatrix<_TypeOfMatrix>::Value( int x, int y )
{
	if ( x<0 || x>GetWidth() || y<0 || y>GetHeight() )
	{
		assert( false );
		return m_zeroValue;
	}
	return m_matrixData.at( x + this->GetWidth() * y );
}

template< typename _TypeOfMatrix>
bool CMatrix<_TypeOfMatrix>::GetInverse( CMatrix<_TypeOfMatrix>& inverseMarix ) const
{
	CMatrix<_TypeOfMatrix> inversionMatrix( this->GetWidth(), this->GetHeight(), this->m_zeroValue, this->m_unitValue );

	// A^(-1)=(1/|A|)×A* ，其中A^(-1)表示矩阵A的逆矩阵，其中|A|为矩阵A的行列式，A*为矩阵A的伴随矩阵。
	_TypeOfMatrix detA;
	bool hasDeterminant = this->GetDeterminant( detA );
	if ( !hasDeterminant || detA == m_zeroValue )
	{
		return false;
	}

	CMatrix<_TypeOfMatrix> mIns( this->GetWidth(), this->GetHeight(), m_zeroValue, this->m_unitValue );
	if ( 1== this->GetWidth() && 1 == this->GetHeight() )
	{
		mIns.Value( 0, 0 ) = m_unitValue;
		inversionMatrix = ( m_unitValue / detA ) * mIns;
	}
	else
	{
		for (int i = 0; i < GetHeight(); i++)
		{
			for (int j = 0; j < GetWidth(); j++)
			{
				mIns.Value( j, i ) = GetAlgebraicCofactor(j, i);				
			}
		}
	}

	inverseMarix = m_unitValue / detA * mIns;

	return true;
}

template< typename _TypeOfMatrix>
inline bool CMatrix<_TypeOfMatrix>::GetDeterminant( _TypeOfMatrix& determinantValue ) const
{
	long i, j, m; 
	long lop = 0;
	determinantValue = this->m_zeroValue;
	_TypeOfMatrix mid = this->m_unitValue;

	if ( this->GetWidth() != this->GetHeight() 
		|| this->GetWidth() == 0
		|| this->GetHeight() == 0 )
	{
		return false;
	}

	int width = this->GetWidth();	
	if( 1 != width )
	{
		lop = (width == 2)? 1 : width;     
		for(m = 0; m < lop; m++)
		{
			mid=m_unitValue;
			//order add 
			for(i = 0, j = m; i < width; i++, j++)
			{
				mid *= this->Value( j%width, i ); //(*(pMatrix + i*width + j%width));
			}
			determinantValue += mid;
		}

		for(m = 0; m < lop; m++)
		{                      
			mid = 1;          
			//inverse order sub
			for(i = 0, j = width - 1 - m + width; i < width; i++, j--)
			{
				mid = mid * this->Value( j%width, i ); // (*(pMatrix + i*width + j%width));
			}
			determinantValue -= mid;
		}
	}
	else 
	{
		determinantValue = this->Value(0,0);
	}
	return true;
}

template< typename _TypeOfMatrix>
_TypeOfMatrix CMatrix<_TypeOfMatrix>::GetAlgebraicCofactor( int x, int y ) const
{
	// C(i,j) = (-1)^(i+j) * M(i,j)

	int width = GetWidth();
	int height = GetHeight();

	assert( x < width && y < height );
	assert( width == height );

	if ( x<0 || x > width 
		|| y > height || y<0
		|| width != height )
	{
		assert( false );
		return m_zeroValue;
	}

	// 去掉 (x,y) 所在的行和列.
	long tmpHeight = height - 1;
	long tmpWidth = width - 1;
	CMatrix<_TypeOfMatrix> tmpMatrix( tmpWidth, tmpHeight, m_zeroValue, m_unitValue );
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int tmpX = i;
			if ( i>x )
			{
				tmpX = i-1;
			}

			int tmpY = j;
			if( j>y )
			{
				tmpY = j-1;
			}
			if ( i != x && j != y )
			{
				tmpMatrix.Value( tmpX, tmpY ) = this->Value( i, j );
			}
		}
	}

	int nGene = ( x + y ) % 2 == 0 ? 1 : -1;	// 符号.
	
	_TypeOfMatrix detATmp;
	tmpMatrix.GetDeterminant( detATmp );
	return nGene * detATmp;
}

// end of the file


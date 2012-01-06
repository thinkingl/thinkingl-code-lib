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

	/**	构造且填充单一的初始值. */
	inline CMatrix( int width, int height, const _TypeOfMatrix& dataValue );

	/**	构造赋予初始值. */
	inline CMatrix( int width, int height, const CMatrixDataList& defaultValue );

	inline virtual ~CMatrix(void);

	inline int GetWidth() const;
	inline int GetHeight() const;

	inline const _TypeOfMatrix& Value( int x, int y ) const;
	inline _TypeOfMatrix& Value( int x, int y );

	inline CMatrix operator *( const CMatrix& another );
private:
	int m_width;
	int m_height;
	CMatrixDataList m_matrixData;
	const _TypeOfMatrix m_defaultValue;
};

template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix>::CMatrix() : m_width(0), m_height(0), m_defaultValue(0)
{}

template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix>::~CMatrix()
{}

template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix>::CMatrix( int width, int height, const CMatrixDataList& data ) 
	: m_width( width ), m_height( height )
	, m_matrixData( data ), m_defaultValue( 0 )
{}

template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix>::CMatrix( int width, int height, const _TypeOfMatrix& defaultValue ) 
	: m_width( width ), m_height( height )
	, m_matrixData( width*height, defaultValue ), m_defaultValue( defaultValue )
{}

template< typename _TypeOfMatrix>
CMatrix<_TypeOfMatrix> CMatrix<_TypeOfMatrix>::operator *( const CMatrix<_TypeOfMatrix>& another )
{
	assert( this->GetWidth() == another.GetHeight() );	// 矩阵乘法要求.
	
	int newWidth = another.GetWidth();
	int newHeight = this->GetHeight();
	int equalPart = this->GetWidth();

	CMatrix<_TypeOfMatrix> newMatrix( newWidth, newHeight, m_defaultValue );
	for ( int y=0; y<newHeight; y++ )
	{
		for( int x=0; x<newWidth; x++ )
		{
			_TypeOfMatrix valueOfwh = m_defaultValue;
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
	return m_matrixData.at( x + this->GetWidth() * y );
}

template< typename _TypeOfMatrix>
_TypeOfMatrix& CMatrix<_TypeOfMatrix>::Value( int x, int y )
{
	return m_matrixData.at( x + this->GetWidth() * y );
}

// end of the file


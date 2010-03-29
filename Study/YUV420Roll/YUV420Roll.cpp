// RotateYUV420.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <complex>
#include <iostream>
#include <fstream>

using namespace std;

struct TMatrix 
{
	int m_nWidth;
	int m_nHeight;
	char *m_pBuf;
};

typedef std::complex<int> TIntComplex;
typedef std::complex<float> TFloatComplex;

void Rotate90( int nT, const TMatrix& tSrcMatrix, TMatrix& tDstMatrix )
{
	// 旋转单位向量。
	TIntComplex rotateUnit( 0, 1 );
	TIntComplex unrotateUnit( 0, -1 );

	rotateUnit *= nT;
	unrotateUnit *= nT;

	// 起始位置的中心。
	TIntComplex tSrcCenter( tSrcMatrix.m_nWidth/2, tSrcMatrix.m_nHeight/2 );
	// 目标位置中心。
	TIntComplex tDstCenter( tDstMatrix.m_nWidth/2, tDstMatrix.m_nHeight/2 );

	// 旋转后的中心。
	TIntComplex tRotateSrccenter = tSrcCenter * rotateUnit;

	// 需要平移的向量。
	TIntComplex tOffset = tRotateSrccenter - tDstCenter;

	int nCount = 0;
	for( int nH = 0; nH < tDstMatrix.m_nHeight; ++ nH )
	{
		for ( int nW = 0; nW < tDstMatrix.m_nWidth; ++nW )
		{
			if ( nCount != ( nH * tDstMatrix.m_nWidth + nW ) )
			{
				int ere = 3;
			}
			nCount ++;

			TIntComplex tDstVector( nW, nH );
			TIntComplex tSrcVector = ( tDstVector + tOffset ) * unrotateUnit;

			int nX = tSrcVector.real() - 1;
			int nY = tSrcVector.imag() - 1;
			if ( nX >= 0 && nX < tSrcMatrix.m_nWidth && nY >= 0 && nY < tSrcMatrix.m_nHeight )
			{
				tDstMatrix.m_pBuf[ nH * tDstMatrix.m_nWidth + nW ] = tSrcMatrix.m_pBuf[ nY * tSrcMatrix.m_nWidth + nX ];
				int nSrcIndex = nY * tSrcMatrix.m_nWidth + nX;
				if ( nSrcIndex >= 32 * 24 || tSrcMatrix.m_pBuf[nSrcIndex] != 'x' )
				{
					int ere = 3;
				}
			}
			else
			{
				int erer = 3;
			}

		}
	}
}

void Rotate( float fRotateDegree, const TMatrix& tSrcMatrix, TMatrix& tDstMatrix )
{
	// 旋转单位向量
	TFloatComplex rotateUnit( cos( fRotateDegree ), sin( fRotateDegree ) );
	TFloatComplex unrotateUnit( cos( -fRotateDegree ), sin( -fRotateDegree ) );

	// 起始位置的中心。
	TFloatComplex tSrcCenter( tSrcMatrix.m_nWidth/2, tSrcMatrix.m_nHeight/2 ); // w/2 + i h/2
	// 目标位置中心。
	TFloatComplex tDstCenter( tDstMatrix.m_nWidth/2, tDstMatrix.m_nHeight/2 ); //  W/2 + i H/2

	// 旋转后的中心。
	TFloatComplex tRotateSrccenter = tSrcCenter * rotateUnit; //  ( w/2 + i h/2 ) * (  cosθ + i sinθ ) 

	// 需要平移的向量。
	TFloatComplex tOffset = tRotateSrccenter - tDstCenter; // ( W/2 + i H/2 ) – ( w/2 + i h/2 ) * (  cosθ + i sinθ )

	int nCount = 0;
	for( int nH = 0; nH < tDstMatrix.m_nHeight; ++ nH )
	{
		for ( int nW = 0; nW < tDstMatrix.m_nWidth; ++nW )
		{
			if ( nCount != ( nH * tDstMatrix.m_nWidth + nW ) )
			{
				int ere = 3;
			}
			nCount ++;

			TFloatComplex tDstVector( nW, nH );
			TFloatComplex tSrcVector = ( tDstVector + tOffset ) * unrotateUnit;

			// 四舍五入
			int nX = tSrcVector.real() + 0.5 - 1 ;
			int nY = tSrcVector.imag() + 0.5 - 1 ;
			if ( nX >= 0 && nX < tSrcMatrix.m_nWidth && nY >= 0 && nY < tSrcMatrix.m_nHeight )
			{
				tDstMatrix.m_pBuf[ nH * tDstMatrix.m_nWidth + nW ] = tSrcMatrix.m_pBuf[ nY * tSrcMatrix.m_nWidth + nX ];				
			}
			else
			{
				// 没有对应的点，这里不处理，可以填充黑色。
			}

		}
	}
}

void PrintMatrix( const TMatrix& matrix )
{
	for( int i=0; i<matrix.m_nHeight ; ++i )
	{
		for( int k=0; k<matrix.m_nWidth; ++k )
		{
			// 			int nValue = matrix.m_pBuf[ i * matrix.m_nWidth + k ];
			// 			
			// 			cout << nValue ;
			// 			if ( nValue >= 0 )
			// 			{
			// 				cout << ( " " );
			// 				if ( nValue < 10 )
			// 				{
			// 					cout << ( " " );
			// 				}
			// 				if ( nValue < 100 )
			// 				{
			// 					cout << ( " " );
			// 				}
			// 			}
			cout << matrix.m_pBuf[ i * matrix.m_nWidth + k ];
		}
		cout << endl;
	}
	cout << endl;
}

void RotateYUV420P( float fRotateDegree, const void *pSrcData, int nSrcWidth, int nSrcHeight, 
				   void *pDstData, int nDstWidth, int nDstHeight )
{
	TMatrix tYSrcMatrix, tYDstMatrix;
	tYSrcMatrix.m_nWidth = nSrcWidth;
	tYSrcMatrix.m_nHeight = nSrcHeight;
	tYSrcMatrix.m_pBuf = (char*)pSrcData;

	tYDstMatrix.m_nWidth = nDstWidth;
	tYDstMatrix.m_nHeight = nDstHeight;
	tYDstMatrix.m_pBuf = (char*)pDstData;

	Rotate( fRotateDegree, tYSrcMatrix, tYDstMatrix );

	int nYSrcOffset = nSrcWidth * nSrcHeight;
	int nUVSrcOffset = nYSrcOffset >> 2;

	int nYDstOffset = nDstWidth * nDstHeight;
	int nUVDstOffset = nYDstOffset >> 2;

	pSrcData = (char*)pSrcData + nYSrcOffset;
	pDstData = (char*)pDstData + nYDstOffset;

	for ( int i=0; i<2; ++i )
	{
		TMatrix tUVSrcMatrix, tUVDstMatrix;
		tUVSrcMatrix.m_pBuf = (char*)pSrcData;
		tUVSrcMatrix.m_nWidth = nSrcWidth >> 1;
		tUVSrcMatrix.m_nHeight = nSrcHeight >> 1;

		tUVDstMatrix.m_pBuf = (char*)pDstData;
		tUVDstMatrix.m_nWidth = nDstWidth >> 1;
		tUVDstMatrix.m_nHeight = nDstHeight >> 1;

		Rotate( fRotateDegree, tUVSrcMatrix, tUVDstMatrix );

		pSrcData = (char*)pSrcData + nUVSrcOffset;
		pDstData = (char*)pDstData + nUVDstOffset;

	}
}

/** 优化，空间换时间。 */
void RotateMatrixSpace2Time( float fRotateDegree, const TMatrix& tSrcMatrix, TMatrix& tDstMatrix )
{
	static int *s_indexBuf = NULL;
	static int s_nWidth = NULL;
	static int s_nHeight = NULL;
	static float s_fRotateDegree = 0.0;

	int nDstWidth = tDstMatrix.m_nWidth;
	int nDstHeight = tDstMatrix.m_nHeight;

	bool bShouldRelease = s_indexBuf && ( s_nWidth != nDstWidth || s_nHeight != nDstHeight );
	bool bShouldReinit = ( NULL == s_indexBuf ) || bShouldRelease || ( s_fRotateDegree - fRotateDegree > 0.0001 );

	if( bShouldRelease )
	{
		delete[] s_indexBuf;
		s_indexBuf = NULL;
		s_fRotateDegree = 0.0;
	}

	int nDstBufLen = nDstWidth * nDstHeight;

	if ( NULL == s_indexBuf )
	{
		s_indexBuf = new int[ nDstBufLen ];
		s_nWidth = nDstWidth;
		s_nHeight = nDstHeight;

		memset( s_indexBuf, -1, sizeof( s_indexBuf[0] ) * nDstBufLen );
	}

	if ( bShouldReinit )
	{
		// 旋转单位向量
		TFloatComplex rotateUnit( cos( fRotateDegree ), sin( fRotateDegree ) );
		TFloatComplex unrotateUnit( cos( -fRotateDegree ), sin( -fRotateDegree ) );

		// 起始位置的中心。
		TFloatComplex tSrcCenter( tSrcMatrix.m_nWidth/2, tSrcMatrix.m_nHeight/2 ); // w/2 + i h/2
		// 目标位置中心。
		TFloatComplex tDstCenter( tDstMatrix.m_nWidth/2, tDstMatrix.m_nHeight/2 ); //  W/2 + i H/2

		// 旋转后的中心。
		TFloatComplex tRotateSrccenter = tSrcCenter * rotateUnit; //  ( w/2 + i h/2 ) * (  cosθ + i sinθ ) 

		// 需要平移的向量。
		TFloatComplex tOffset = tRotateSrccenter - tDstCenter; // ( W/2 + i H/2 ) – ( w/2 + i h/2 ) * (  cosθ + i sinθ )

		int nCount = 0;
		for( int nH = 0; nH < tDstMatrix.m_nHeight; ++ nH )
		{
			for ( int nW = 0; nW < tDstMatrix.m_nWidth; ++nW )
			{
				if ( nCount != ( nH * tDstMatrix.m_nWidth + nW ) )
				{
					int ere = 3;
				}
				nCount ++;

				TFloatComplex tDstVector( nW, nH );
				TFloatComplex tSrcVector = ( tDstVector + tOffset ) * unrotateUnit;

				// 四舍五入
				int nX = tSrcVector.real() + 0.5 - 1 ;
				int nY = tSrcVector.imag() + 0.5 - 1 ;
				if ( nX >= 0 && nX < tSrcMatrix.m_nWidth && nY >= 0 && nY < tSrcMatrix.m_nHeight )
				{
					s_indexBuf[ nH * tDstMatrix.m_nWidth + nW ] = nY * tSrcMatrix.m_nWidth + nX;		// 这里换成序号。		
				}
				else
				{
					// 没有对应的点，这里不处理，可以填充黑色。
				}

			}
		}

	}
	

	if ( s_indexBuf )
	{
		for ( int i=0; i<nDstBufLen; ++i )
		{
			int nIndex = s_indexBuf[ i ];
			if ( -1 != nIndex )
			{
				tDstMatrix.m_pBuf[i] = tSrcMatrix.m_pBuf[ nIndex ];
			}
		}
	}
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	const int SRC_WIDTH = 16;
	const int SRC_HEIGHT = 12;
	const int DST_WIDTH = 20;
	const int DST_HEIGHT  = 20;
	const int SRC_Y_BUF_LEN = SRC_WIDTH * SRC_HEIGHT;
	const int DST_Y_BUF_LEN = DST_WIDTH * DST_HEIGHT;

	char arSRCYBuf[ SRC_Y_BUF_LEN  ];
	memset( arSRCYBuf, 'x', SRC_Y_BUF_LEN );
	// 	for ( int i=0; i<SRC_Y_BUF_LEN; ++i )
	// 	{
	// 		arSRCYBuf[i] = i+1;
	// 	}

	char arDSTYBuf[ DST_Y_BUF_LEN ] = { 0 };
	memset( arDSTYBuf, '0', DST_Y_BUF_LEN );
	TMatrix tSrc;
	tSrc.m_nWidth = SRC_WIDTH;
	tSrc.m_nHeight = SRC_HEIGHT;
	tSrc.m_pBuf = arSRCYBuf;


	TMatrix tDst;
	tDst.m_nWidth = DST_WIDTH;
	tDst.m_nHeight = DST_HEIGHT;
	tDst.m_pBuf = arDSTYBuf;

	double pi = 3.1415926535;

	double toRadian = pi / 180;
	float fRotateRadians = 265 * toRadian;

	cout << "dst matrix" << endl;
	PrintMatrix( tDst );
	cout << "src matrix: " << endl;

	PrintMatrix( tSrc );
	Rotate( fRotateRadians, tSrc, tDst );

	//	Rotate90( 1, tSrc, tDst );

	cout << "After rotate: " << endl;
	PrintMatrix( tDst );

	const int PIC_SRC_WIDTH = 352;
	const int PIC_SRC_HEIGHT = 288;
	FILE *pInYuv = fopen( "stefan_cif2.yuv", "rb" );
	
	char inbuf[ PIC_SRC_WIDTH * PIC_SRC_HEIGHT * 3 / 2 ] = {0};
	int nLen = sizeof( inbuf );
	int nRLen = fread( inbuf, 1, sizeof( inbuf ), pInYuv );
	fclose( pInYuv );

	const int PIC_DST_WIDTH = 288;
	const int PIC_DST_HEIGHT = 352;
	char outbuf[ PIC_DST_WIDTH * PIC_DST_HEIGHT * 3 / 2 ] = {0};

	fRotateRadians = 45 * toRadian;
	RotateYUV420P( fRotateRadians, inbuf, PIC_SRC_WIDTH, PIC_SRC_HEIGHT, outbuf, PIC_DST_WIDTH, PIC_DST_HEIGHT );

	// 测试优化后的算法，只旋转Y分量,与不优化的算法做内存比较。
	char outbufSpaceToTime[ PIC_DST_WIDTH * PIC_DST_HEIGHT ] = {0};
	TMatrix tSrcMatrix;
	tSrcMatrix.m_nWidth = PIC_SRC_WIDTH;
	tSrcMatrix.m_nHeight = PIC_SRC_HEIGHT;
	tSrcMatrix.m_pBuf = inbuf;
	TMatrix tDstMatrix;
	tDstMatrix.m_nWidth = PIC_DST_WIDTH;
	tDstMatrix.m_nHeight = PIC_DST_HEIGHT;
	tDstMatrix.m_pBuf = outbufSpaceToTime;	
	RotateMatrixSpace2Time( fRotateRadians, tSrcMatrix, tDstMatrix );

	int nRet = memcmp( outbuf, outbufSpaceToTime, sizeof( outbufSpaceToTime ) );

	FILE *pOutYuv = fopen( "out.yuv", "wb" );
	fwrite( outbuf, 1, sizeof( outbuf ), pOutYuv );
	fclose( pOutYuv );


	pOutYuv = fopen( "outspace2time.yuv", "wb" );
	fwrite( outbufSpaceToTime, 1, sizeof( outbufSpaceToTime ), pOutYuv ); // 写2遍凑长度，但用工具只看Y分量。
	fwrite( outbufSpaceToTime, 1, sizeof( outbufSpaceToTime ), pOutYuv );
	fclose( pOutYuv );

	return 0;
}


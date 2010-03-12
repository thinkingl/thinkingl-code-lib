// YUV420Roll.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "YUV420Roll.h"
#include <complex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

void RollClockwise( int nWidth, int nHeight,  void *pSrc, void *pDst )
{
	int i = 0;
	for ( int y = 0; y < nWidth; ++y )
	{
		for ( int x=0; x<nHeight; ++x )
		{
			int nIndex = nWidth * nHeight - nWidth * x - nWidth + y;
			(( unsigned char * )pDst)[i] = ( (unsigned char* )pSrc )[ nIndex ];
			i++;
		}
	}
}

void RollYUV420Clockwise( int nWidth, int nHeight, void * pSrc, void * pDst )
{
	int nOffset = 0;

	int nYWidth = nWidth;
	int nYHeight = nHeight;
	RollClockwise( nYWidth, nYHeight, pSrc, pDst );
	nOffset += nYWidth * nYHeight;

	int nUWidth = nWidth / 2;
	int nUHeight = nHeight / 2;
	RollClockwise( nUWidth, nUHeight, (char*)pSrc + nOffset, (char*)pDst + nOffset );
	nOffset += nUWidth * nUHeight;

	int nVWidth = nWidth / 2;
	int nVHeight = nHeight / 2;
	RollClockwise( nVWidth, nVHeight, (char*)pSrc + nOffset, (char*)pDst + nOffset );
}

typedef complex<int> TIntComplex;
typedef complex<float> TFloatComplex;

void Rotate( int nRotateDegree, int nSrcWidth, int nSrcHeight, void *pSrc, int nDstWidth, int nDstHeight, void *pDst )
{
	TFloatComplex rotateUnitImaginary( cos( nRotateDegree ), sin( nRotateDegree ) );
}

void Rotation( long nStep, int nWidth, int nHeight,   void *pSrc, void *pDst )
{
	
	// -i;
	TIntComplex negativeUnitImaginary( 0, -1 );

	TIntComplex rotateImaginary = pow( negativeUnitImaginary, nStep );

	

}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣

		// ��һ��YUV420�����ļ���
		CFile yuvIn;
		yuvIn.Open( _T( "yuv.yuv" ), CFile::modeRead );

		const int BufLen = 400000;
		unsigned char arBuf[BufLen] = { 0 };
		yuvIn.Read( arBuf, sizeof( arBuf ) );

		//for ( int i=0; i< BufLen; ++i )
		//{
		//	arBuf[i] = i;
		//}

		int nWidth = 320;
		int nHeight = 240;
		int nBufLen = nWidth * nHeight * 3 / 2;

		unsigned char arDst[BufLen] = { 0 };

		Rotation( 2, nWidth, nHeight, arBuf, arDst );
		
		// ˳ʱ�뷭ת90��
		RollYUV420Clockwise( nWidth, nHeight, arBuf, arDst );

		// �����ת������ݵ��ļ���
		CFile yuvOut;
		yuvOut.Open( _T( "out.yuv" ), CFile::modeWrite | CFile::modeCreate );
		yuvOut.Write( arDst, nBufLen );

	}

	return nRetCode;
}

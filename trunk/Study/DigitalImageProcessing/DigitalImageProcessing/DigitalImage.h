#pragma once

#include "portabledefine.h"
#include "stdafx.h"
#include "Matrix.h"

class CDigitalImage
{
public:
	/** ͼƬ����. */
	enum EImageType
	{
		DIT_Gray,	// �Ҷ�ͼ.
		DIT_RGB,		// RGBͼ.
	};

	/**	��ֵ����. */
	enum EInterpolateType
	{
		InterpolateNearestNeighbor,	// ����ٽ����ֵ.
		InterpolateBilinear,		// ˫���Բ�ֵ.
	};

public:
	CDigitalImage(void);
	virtual ~CDigitalImage(void);

	/** ����һ���ļ�. */
	bool Load( ctstring filePath );

	/** �Ƿ�������ļ�. */
	bool IsLoaded() const;

	/** ��ȡ�ļ�·��. */
	tstring GetFilePath() const;

	/** ��ȡͼƬ�ļ���С. */
	int GetFileLength() const;

	/** ��ȡͼƬ��ʽ. */
	EImageType GetImageType()const;

	/** ��ȡͼƬ��� */
	int GetWidth()const;

	/** ��ȡͼƬ�߶�. */
	int GetHeight() const;

	/** ��ȡͼƬ�����ֽ���. */
	int GetBitsPerPixel() const;

	/** ��ȡͼƬ�ĻҶȽ���. */
	int GetIntensityLevel() const;
	
	/** ת���ɻҶ�ͼƬ. ���� HSI ��ʽ ��RGBƽ��. 
	*	intensityLevels : �ҽ�. 1-8. 
	*/
	bool ToGrayscaleImageHSI( int intensityLevels );

	/** ת���ɻҶ�ͼƬ. ���� YUV ��ʽ RGB�в�ͬ��Ȩ. */
	bool ToGrayscaleImageYUV( int intensityLevels );

	/** �޸�ͼƬ�ĻҶȽ���. 
	*	����ǲ�ɫͼƬ, �޸�ÿһ����ɫ�Ļҽ�.
	*/
	bool SetIntensityLevel( int lvl );

	/** �Ҷȷ�ת */
	bool ImageNegitive();

	/** Log Transformation. 
	*	P. 131 3.2.2
	*	��ʽ: s = c * log( 1+r ) .
	*	c ��Ҫ����.
	*/
	bool IntensityLogTransform( double c );

	/** Power-law Transformation. 
	*	P.132 3.2.2
	*	��ʽ: s = c * r ^ v
	*/
	bool IntensityPowerTransform( double c, double v );

	/** Bit planes reconstruct 
	*	P.141 
	*/
	bool IntensityBitPlanesReconstruct( uint8 bitMask );

	/** ֻ����һλ, תΪ1bit�ĺڰ�ͼ. */
	bool IntensityBitPlanesOneBit( int bitIndex );

	/** ��ȡֱ��ͼ������. */
	typedef std::vector<int> THistogramData;
	bool GetHistogramData( THistogramData& grayHistoram, THistogramData& rh, THistogramData& gh, THistogramData& bh ) const;

	/** �Զ���ƽ��. */
	bool HistogramEqualization();

	/** ��¡��һ��һ���Ķ���. */
	CDigitalImage *Clone()const;

	/** ��ȡͼƬ������. */
	CImage *GetImageDrawer() const;

	/** �ͷ���Դ. */
	void Release();

	/** ��ʱ����ת(����ϵ��ĽǶȷ�������ʱ��.). */
	void Rotate( double angle, int rotationX, int rotationY, EInterpolateType interpolateType );

	/** λ��. */
	void Translate( int coordX, int coordY );

	/** ����. */
	void Scale( double multipleX, double multipleY, EInterpolateType interpolateType );

	/** Inversed Affine transform  ���������.
	*	affineMatrix : ����ľ���.
	*	interpolateType :��ֵ�㷨
	*	transformOriginX :�����е�ԭ��(������ת��)
	*	transformOriginY : �����е�ԭ��(������ת��)
	*	newWidth : ��ͼƬ�����Ŀ��.
	*	newHeight: ��ͼƬ�����ĸ߶�.
	*/
	void InverseAffineTransform( const CMatrix<double>& affineMatrix, EInterpolateType interpolateType
		, int transformOriginX, int transformOriginY, int newWidth, int newHeight );


	/** ���Ӹ�˹����.
	*	noiseMean : ��ֵ
	*	noiseVariance :���
	*/
	void AddGaussianNoise( double noiseMean, double noiseVariance );

	/** ���ӽ�������. 
	*	pixelNumPerSalt : ���ٸ���һ��������(�׵�)
	*	pixelNumPerPepper : ���ٸ�����һ����������(�ڵ�)
	*/
	void AddSaltAndPepperNoise( int pixelNumPerSalt, int pixelNumPerPepper );

	/** ȡһ������㴦������ֵ. */
	uint32 Pixel( int x, int y ) const;
	uint32& Pixel( int x, int y );
private:
	
	/** �ҶȻ���. 
	*	oldIntensity : ֮ǰ�ĻҶ�ֵ.
	*	oldGrayscaleLev :֮ǰ�ĻҶȼ��� 1-8
	*	newGrayscaleLev :�µĻҶȼ�����
	*	return : �µĻҶ�ֵ.
	*/
	int IntensityTrans( int oldIntensity, int oldGrayscaleLev, int newGrayscaleLev )const;

	/** ת��Ϊ�Ҷ�. */
	enum ERGB2GrayMode
	{
		R2G_HSI,
		R2G_YUV,
	};
	bool RGB2Gray( ERGB2GrayMode mod, int intensityLevel );

	/**	����һ����˹�����.
	*	u : ��ֵ
	*	g : ����.
	*/
	double GaussianRandom(double u,double g /*,double *r*/ ) ;

private:
//	CImage m_image;
	tstring m_imagePath;

	/** ����. 
	*	��ʱ�̶�����32λ��ʾһ������.
	*/
	typedef std::vector< uint32 > TImageDataBuf;
	TImageDataBuf m_imageDataBuf; 

	/** ͼƬ���Ƚ���.
	*	����RGB�Ĳ�ɫͼƬ,��ʾÿ��ԭɫ�����Ƚ���.
	*/
	int m_intensityLevels;

	/** m_imageDataBuf�д�ŵ�ͼƬ����. */
	EImageType m_imageType;

	/** ͼƬ��ȸ߶�. */
	int m_width;
	int m_height;
};

// end of the file




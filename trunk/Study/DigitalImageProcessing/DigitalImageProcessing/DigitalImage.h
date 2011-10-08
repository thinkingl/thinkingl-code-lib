#pragma once

#include "portabledefine.h"
#include "stdafx.h"

class CDigitalImage
{
public:
	/** ͼƬ����. */
	enum EImageType
	{
		DIT_Gray,	// �Ҷ�ͼ.
		DIT_RGB,		// RGBͼ.
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

	/** ��¡��һ��һ���Ķ���. */
	CDigitalImage *Clone()const;

	/** ��ȡͼƬ������. */
	CImage *GetImageDrawer() const;

	/** �ͷ���Դ. */
	void Release();
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




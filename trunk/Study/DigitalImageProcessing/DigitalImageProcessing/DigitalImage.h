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
	
	/** ת���ɻҶ�ͼƬ.
	*	intensityLevels : �ҽ�. 1-8. 
	*/
	bool ToGrayscaleImage( int intensityLevels );

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

private:
//	CImage m_image;
	tstring m_imagePath;

	/** ����. 
	*	��ʱ�̶�����32λ��ʾһ������.
	*/
	typedef std::vector< uint32 > TImageDataBuf;
	TImageDataBuf m_imageDataBuf; 

	/** ͼƬÿ���ص�λ��.
	*	��Ȼ�洢�̶�Ϊ32λ, �����е���Чλ�������ֵ.
	*/
	int m_bitsPerPixel;

	/** m_imageDataBuf�д�ŵ�ͼƬ����. */
	EImageType m_imageType;

	/** ͼƬ��ȸ߶�. */
	int m_width;
	int m_height;
};

// end of the file




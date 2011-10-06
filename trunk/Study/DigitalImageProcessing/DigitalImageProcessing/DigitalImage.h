#pragma once

#include "portabledefine.h"
#include "stdafx.h"

class CDigitalImage
{
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
	tstring GetImageType()const;

	/** ��ȡͼƬ��� */
	int GetWidth()const;

	/** ��ȡͼƬ�߶�. */
	int GetHeight() const;

	/** ��ȡͼƬ�����ֽ���. */
	int GetBitsPerPixel() const;
	
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
	

private:
	CImage m_image;
	tstring m_imagePath;

	/** ����. 
	*	��ʱ�̶�����32λ��ʾһ������.
	*/
	typedef std::vector< uint32 > TImageDataBuf;
	TImageDataBuf m_imageDataBuf; 

	/** ͼƬ���ݵ�ģʽ. */
	int m_bitsPerPixel;
};

// end of the file




#pragma once

#include "portabledefine.h"
#include "stdafx.h"

class CDigitalImage
{
public:
	/** 图片类型. */
	enum EImageType
	{
		DIT_Gray,	// 灰度图.
		DIT_RGB,		// RGB图.
	};
public:
	CDigitalImage(void);
	virtual ~CDigitalImage(void);

	/** 载入一个文件. */
	bool Load( ctstring filePath );

	/** 是否加载了文件. */
	bool IsLoaded() const;

	/** 获取文件路径. */
	tstring GetFilePath() const;

	/** 获取图片文件大小. */
	int GetFileLength() const;

	/** 获取图片格式. */
	EImageType GetImageType()const;

	/** 获取图片宽度 */
	int GetWidth()const;

	/** 获取图片高度. */
	int GetHeight() const;

	/** 获取图片像素字节数. */
	int GetBitsPerPixel() const;

	/** 获取图片的灰度阶数. */
	int GetIntensityLevel() const;
	
	/** 转换成灰度图片.
	*	intensityLevels : 灰阶. 1-8. 
	*/
	bool ToGrayscaleImage( int intensityLevels );

	/** 克隆出一个一样的对象. */
	CDigitalImage *Clone()const;

	/** 获取图片绘制者. */
	CImage *GetImageDrawer() const;

	/** 释放资源. */
	void Release();
private:
	
	/** 灰度换算. 
	*	oldIntensity : 之前的灰度值.
	*	oldGrayscaleLev :之前的灰度级数 1-8
	*	newGrayscaleLev :新的灰度级数．
	*	return : 新的灰度值.
	*/
	int IntensityTrans( int oldIntensity, int oldGrayscaleLev, int newGrayscaleLev )const;

private:
//	CImage m_image;
	tstring m_imagePath;

	/** 数据. 
	*	暂时固定的用32位表示一个像素.
	*/
	typedef std::vector< uint32 > TImageDataBuf;
	TImageDataBuf m_imageDataBuf; 

	/** 图片每像素的位数.
	*	虽然存储固定为32位, 但其中的有效位数是这个值.
	*/
	int m_bitsPerPixel;

	/** m_imageDataBuf中存放的图片类型. */
	EImageType m_imageType;

	/** 图片宽度高度. */
	int m_width;
	int m_height;
};

// end of the file




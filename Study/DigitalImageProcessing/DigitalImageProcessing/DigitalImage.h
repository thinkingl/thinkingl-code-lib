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
	
	/** 转换成灰度图片. 按照 HSI 方式 求RGB平均. 
	*	intensityLevels : 灰阶. 1-8. 
	*/
	bool ToGrayscaleImageHSI( int intensityLevels );

	/** 转换成灰度图片. 按照 YUV 方式 RGB有不同加权. */
	bool ToGrayscaleImageYUV( int intensityLevels );

	/** 修改图片的灰度阶数. 
	*	如果是彩色图片, 修改每一种颜色的灰阶.
	*/
	bool SetIntensityLevel( int lvl );

	/** 灰度反转 */
	bool ImageNegitive();

	/** Log Transformation. 
	*	P. 131 3.2.2
	*	公式: s = c * log( 1+r ) .
	*	c 需要输入.
	*/
	bool IntensityLogTransform( double c );

	/** Power-law Transformation. 
	*	P.132 3.2.2
	*	公式: s = c * r ^ v
	*/
	bool IntensityPowerTransform( double c, double v );

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

	/** 转换为灰度. */
	enum ERGB2GrayMode
	{
		R2G_HSI,
		R2G_YUV,
	};
	bool RGB2Gray( ERGB2GrayMode mod, int intensityLevel );

private:
//	CImage m_image;
	tstring m_imagePath;

	/** 数据. 
	*	暂时固定的用32位表示一个像素.
	*/
	typedef std::vector< uint32 > TImageDataBuf;
	TImageDataBuf m_imageDataBuf; 

	/** 图片亮度阶数.
	*	对于RGB的彩色图片,表示每个原色的亮度阶数.
	*/
	int m_intensityLevels;

	/** m_imageDataBuf中存放的图片类型. */
	EImageType m_imageType;

	/** 图片宽度高度. */
	int m_width;
	int m_height;
};

// end of the file




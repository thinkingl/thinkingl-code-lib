#pragma once

#include "portabledefine.h"
#include "stdafx.h"

class CDigitalImage
{
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
	tstring GetImageType()const;

	/** 获取图片宽度 */
	int GetWidth()const;

	/** 获取图片高度. */
	int GetHeight() const;

	/** 获取图片像素字节数. */
	int GetBitsPerPixel() const;
	
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
	

private:
	CImage m_image;
	tstring m_imagePath;

	/** 数据. 
	*	暂时固定的用32位表示一个像素.
	*/
	typedef std::vector< uint32 > TImageDataBuf;
	TImageDataBuf m_imageDataBuf; 

	/** 图片数据的模式. */
	int m_bitsPerPixel;
};

// end of the file




#pragma once

#include "portabledefine.h"

class CDigitalImageLoader
{
public:
	CDigitalImageLoader(void);
	~CDigitalImageLoader(void);

	/** 载入一个图片文件. */
	bool Load( ctstring filePath );


};


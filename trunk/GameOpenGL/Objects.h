#pragma once

#include "stdafx.h"

class Objects
{
public:
	Objects(void);
	~Objects(void);
//	int LoadGLTextures(void);

private:				//私有的函数.
//	GLuint texture[3];  //储存纹理的函数.
//	AUX_RGBImageRec * LoadBMP(char * FileName);
	typedef struct	//定义结构体,星星.
	{
		int r, g , b;//星星的颜色.
		float angle,//星星转动的角度.
			dist;//星星距离原点的距离.
	}star;
	star stars[50];//结构体数组存放星星的数据.


	float  ZDEEP ;
	float turn1,
		turn2,
		xrot,
		yrot;

public:
	void Box1(void);
	int filter;

	void test(void);
};

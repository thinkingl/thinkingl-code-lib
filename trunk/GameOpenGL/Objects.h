#pragma once

#include "stdafx.h"

class Objects
{
public:
	Objects(void);
	~Objects(void);
//	int LoadGLTextures(void);

private:				//˽�еĺ���.
//	GLuint texture[3];  //��������ĺ���.
//	AUX_RGBImageRec * LoadBMP(char * FileName);
	typedef struct	//����ṹ��,����.
	{
		int r, g , b;//���ǵ���ɫ.
		float angle,//����ת���ĽǶ�.
			dist;//���Ǿ���ԭ��ľ���.
	}star;
	star stars[50];//�ṹ�����������ǵ�����.


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

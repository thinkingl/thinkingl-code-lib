/////////////////////////////////////////////////////////////////////////////
//这个类模拟一下游戏服务器的接口.
//暂时的.等弄服务器的时候就用网络接口的类替换掉.
#include "MyMathArithmetic.h"

#pragma once

class VisualServerTemp
{
public:
	VisualServerTemp(void);
	~VisualServerTemp(void);
private:
	//用于存储英雄状态的结构体.
	struct HEROINSERVER
	{
		Vector3f vPos ; 	
	
	};
	HEROINSERVER Hero;
public:

	// 获得当前英雄的位置坐标.
	Vector3f GetHeroCoordinate(void);
};

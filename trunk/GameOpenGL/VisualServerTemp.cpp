#include ".\visualservertemp.h"
#using <mscorlib.dll>

VisualServerTemp::VisualServerTemp(void)
{
	//英雄状态的初始化.
	Hero.vPos.x = 0;
	Hero.vPos.y = 0;
	Hero.vPos.z = 0;
}

VisualServerTemp::~VisualServerTemp(void)
{
}



// 获得当前英雄的位置坐标.
Vector3f VisualServerTemp::GetHeroCoordinate(void)
{
	return Hero.vPos;
}

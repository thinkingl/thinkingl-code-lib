#include ".\visualservertemp.h"
#using <mscorlib.dll>

VisualServerTemp::VisualServerTemp(void)
{
	//Ӣ��״̬�ĳ�ʼ��.
	Hero.vPos.x = 0;
	Hero.vPos.y = 0;
	Hero.vPos.z = 0;
}

VisualServerTemp::~VisualServerTemp(void)
{
}



// ��õ�ǰӢ�۵�λ������.
Vector3f VisualServerTemp::GetHeroCoordinate(void)
{
	return Hero.vPos;
}

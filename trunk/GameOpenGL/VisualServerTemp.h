/////////////////////////////////////////////////////////////////////////////
//�����ģ��һ����Ϸ�������Ľӿ�.
//��ʱ��.��Ū��������ʱ���������ӿڵ����滻��.
#include "MyMathArithmetic.h"

#pragma once

class VisualServerTemp
{
public:
	VisualServerTemp(void);
	~VisualServerTemp(void);
private:
	//���ڴ洢Ӣ��״̬�Ľṹ��.
	struct HEROINSERVER
	{
		Vector3f vPos ; 	
	
	};
	HEROINSERVER Hero;
public:

	// ��õ�ǰӢ�۵�λ������.
	Vector3f GetHeroCoordinate(void);
};

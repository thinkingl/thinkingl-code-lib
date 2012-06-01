
////��֮��////
//����һЩ��ֵ����....


#pragma once

#include "stdafx.h"






class Vector3f
{
public:
	Vector3f();
	~Vector3f();
	Vector3f( float j,float k , float i );

	Vector3f         operator+(Vector3f v);
    Vector3f         operator-(Vector3f v);
    Vector3f         operator*(float n);
    Vector3f         operator/(float n); 
	void operator+=(Vector3f v);
    void operator-=(Vector3f v);

//  ������񻯣�������һ�������ĵ�λ������
	void Normalize();
	// �������㡣
	void Reset();
private:
public:
	float x;
	float y;
	float z;
};

/********************************************************************
*  ȫ�ֺ��������� 
********************************************************************/
// �������������������� 
Vector3f         Cross(Vector3f vVector1, Vector3f vVector2);

// �������������������� 
float           Magnitude(Vector3f vVector1, Vector3f vVector2);

// ������񻯣�������һ�������ĵ�λ������ 
Vector3f Normalize(Vector3f vVector);

// һ��������һ������תһ�Ƕ�.
bool RotateV( Vector3f &vRot, Vector3f v,float angle);

struct VIEWSTATUS	///�ӵ��״̬.
	{	
		Vector3f viewAt,
			lookTo,
			upDirection
			;

	}; 
//

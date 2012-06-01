
////李之兴////
//进行一些数值运算....


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

//  向量规格化（即计算一个向量的单位向量）
	void Normalize();
	// 向量清零。
	void Reset();
private:
public:
	float x;
	float y;
	float z;
};

/********************************************************************
*  全局函数的声明 
********************************************************************/
// 计算两个向量的向量积 
Vector3f         Cross(Vector3f vVector1, Vector3f vVector2);

// 计算两个向量的数量积 
float           Magnitude(Vector3f vVector1, Vector3f vVector2);

// 向量规格化（即计算一个向量的单位向量） 
Vector3f Normalize(Vector3f vVector);

// 一向量绕另一向量旋转一角度.
bool RotateV( Vector3f &vRot, Vector3f v,float angle);

struct VIEWSTATUS	///视点的状态.
	{	
		Vector3f viewAt,
			lookTo,
			upDirection
			;

	}; 
//

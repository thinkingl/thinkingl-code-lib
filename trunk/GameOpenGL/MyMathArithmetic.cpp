#include ".\mymatharithmetic.h"
#using <mscorlib.dll>


Vector3f::Vector3f()
{
}
Vector3f::~Vector3f()
{
}
Vector3f::Vector3f(float j, float k, float i)
{
	x = j;
	y = k;
	z = i;
}

Vector3f Vector3f::operator+(Vector3f v)
{
    return Vector3f( v.x+x , v.y+y , v.z+z );     
}

Vector3f Vector3f::operator-(Vector3f v)
{
    return Vector3f( x-v.x , y-v.y , z-v.z );
}

Vector3f Vector3f::operator*(float n)
{
    return Vector3f( x*n , y*n , z*n );
}

Vector3f Vector3f::operator/(float n)
{
    return Vector3f( x/n , y/n , z/n );    
}

void Vector3f::operator+=(Vector3f v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vector3f::operator-=(Vector3f v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}
// 向量规格化（即计算一个向量的单位向量）  分向量分别除以向量的模 
void Vector3f::Normalize()
{
	float  m = (float)sqrt( x*x + y*y + z*z ); //计算向量的模 
    x = x/m ;
	y = y/m;
	z = z/m;
}

void Vector3f::Reset()
{
	x = y = z = 0.0f ;
}
/***********************************************************************
*  以下定义的是全局函数 
************************************************************************/
// 计算两个向量的叉乘积 （向量积） 
Vector3f   Cross(Vector3f vVector1, Vector3f vVector2)
{
	Vector3f vNormal;
	
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// Return the cross product
	return vNormal;										 
}


// 计算两个向量的数量积 
/*
    公式： v1.x*v2.x + v1.y*v2.y + v1.z*v2.z
*/ 
float Magnitude(Vector3f vVector1, Vector3f vVector2)
{
    return (vVector1.x*vVector2.x + vVector1.y*vVector1.y + vVector1.z*vVector1.z);
} 

// 向量规格化（即计算一个向量的单位向量）  分向量分别除以向量的模 
Vector3f Normalize(Vector3f vVector)
{
    float  m = (float)sqrt( Magnitude(vVector , vVector) ); //计算向量的模 
    return vVector/m ;
}

// 一向量绕另一向量旋转一角度.
bool RotateV( Vector3f &vRot, Vector3f v,float angle)
{

	vRot = Cross( vRot , v ) * sin(angle) + vRot * cos(angle);
 
    return true;
}
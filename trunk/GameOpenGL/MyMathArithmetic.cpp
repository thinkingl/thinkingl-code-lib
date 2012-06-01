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
// ������񻯣�������һ�������ĵ�λ������  �������ֱ����������ģ 
void Vector3f::Normalize()
{
	float  m = (float)sqrt( x*x + y*y + z*z ); //����������ģ 
    x = x/m ;
	y = y/m;
	z = z/m;
}

void Vector3f::Reset()
{
	x = y = z = 0.0f ;
}
/***********************************************************************
*  ���¶������ȫ�ֺ��� 
************************************************************************/
// �������������Ĳ�˻� ���������� 
Vector3f   Cross(Vector3f vVector1, Vector3f vVector2)
{
	Vector3f vNormal;
	
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// Return the cross product
	return vNormal;										 
}


// �������������������� 
/*
    ��ʽ�� v1.x*v2.x + v1.y*v2.y + v1.z*v2.z
*/ 
float Magnitude(Vector3f vVector1, Vector3f vVector2)
{
    return (vVector1.x*vVector2.x + vVector1.y*vVector1.y + vVector1.z*vVector1.z);
} 

// ������񻯣�������һ�������ĵ�λ������  �������ֱ����������ģ 
Vector3f Normalize(Vector3f vVector)
{
    float  m = (float)sqrt( Magnitude(vVector , vVector) ); //����������ģ 
    return vVector/m ;
}

// һ��������һ������תһ�Ƕ�.
bool RotateV( Vector3f &vRot, Vector3f v,float angle)
{

	vRot = Cross( vRot , v ) * sin(angle) + vRot * cos(angle);
 
    return true;
}
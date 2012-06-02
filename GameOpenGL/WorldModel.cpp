#include ".\worldmodel.h"
#include "role.h"
#include "ModelManager.h"

#using <mscorlib.dll>




CWorldModel::CWorldModel(void)
{
//	m_visualServer = new VisualServerTemp();	//  先暂时定义一个临时的服务器类对象.
	// 获得角色实例的指针.	
}

CWorldModel::~CWorldModel(void)
{
}


// 向前走一步.
void CWorldModel::Forward(void)
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);//|SND_LOOP);

	m_mainRole.Walk( MoveDirector::Forward );
	//roleMoveStatus.vPos = roleMoveStatus.vPos + roleMoveStatus.vMoveDirector * roleMoveStatus.fSpeed ;
}

// 向后走一步.
void CWorldModel::Back(void)
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);

	m_mainRole.Walk( MoveDirector::Back );
	//roleMoveStatus.vPos = roleMoveStatus.vPos - roleMoveStatus.vMoveDirector * roleMoveStatus.fSpeed ;
}

// 向左一步.
void CWorldModel::Left(void)
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);


	m_mainRole.Walk( MoveDirector::Left );
	//Vector3f yAxis = Vector3f( 0.0f , 1.0f ,0.0f );
	//Vector3f croosAxis = Normalize( Cross( roleMoveStatus.vMoveDirector , yAxis) );
	//roleMoveStatus.vPos = roleMoveStatus.vPos - croosAxis * roleMoveStatus.fSpeed ;
}

// 向右一步.
void CWorldModel::Right(void)
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);


	m_mainRole.Walk( MoveDirector::Right );
	//Vector3f yAxis = Vector3f( 0.0f , 1.0f ,0.0f );
	//Vector3f croosAxis = Normalize( Cross( roleMoveStatus.vMoveDirector , yAxis) );
	//roleMoveStatus.vPos = roleMoveStatus.vPos + croosAxis * roleMoveStatus.fSpeed ;
}

void CWorldModel::RoleDosomeAction()
{
	static int action = ATTACK;
	m_mainRole.DoAct( action, 2000 );
	action ++;

	if ( action > BOOM )
	{
		action = ATTACK;
	}
}

void CWorldModel::Jump()
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);
	m_mainRole.DoAct( JUMP, 500 );
}

void CWorldModel::Fire()
{
	PlaySound("data\\sound\\Shot_low.wav",NULL,SND_ASYNC );
	m_mainRole.DoAct( ATTACK, 500 );
}

// 初始化
bool CWorldModel::Init()
{

	//roleMoveStatus.fSpeed = 0.05f;
	//roleMoveStatus.vPos = Vector3f( 0.0f ,0.0f ,0.0f );
	//roleMoveStatus.vMoveDirector = roleMoveStatus.vRoleDirector = Vector3f( 0.0f , 0.0f ,-1.0f);
	//roleMoveStatus.vUp = Vector3f( 0.0f , 1.0f , 0.0f );
	return 0;
}

//
//将向量vRot绕轴v旋转angle角度 
bool CWorldModel::RotateV( Vector3f &vRot, Vector3f v,float angle)
{

	vRot = Cross( vRot , v ) * sin(angle) + vRot * cos(angle);
 
    return 0;
}

// 更新现在角色的方向.
//void CWorldModel::UpdateRoleDirector(int xMoved , int yMoved)
//{
//	
//		// 利用向量乘法解决。
//		static Vector3f crossAxis;
//		if(!(( roleMoveStatus.vRoleDirector.y > 0.9 ||  yMoved > 0 )&&( roleMoveStatus.vRoleDirector.y < -0.9 || yMoved < 0)))
//		{
//			crossAxis = Cross( roleMoveStatus.vRoleDirector , roleMoveStatus.vUp );
//			RotateV( roleMoveStatus.vRoleDirector ,crossAxis,yMoved * ROTATE_SPEED );
//		}
//		RotateV( roleMoveStatus.vRoleDirector , roleMoveStatus.vUp , xMoved * ROTATE_SPEED );
//	
//		// 获得角色的当前移动向量.
//		roleMoveStatus.vMoveDirector.x = roleMoveStatus.vRoleDirector.x;
//		roleMoveStatus.vMoveDirector.y = 0.0f ;
//		roleMoveStatus.vMoveDirector.z = roleMoveStatus.vRoleDirector.z;
//		roleMoveStatus.vMoveDirector.Normalize();
//}

// 更新角色信息
//bool CWorldModel::UpdateRoleState()
//{
//	CRole::GetInstance()->SetRolePos( roleMoveStatus.vPos );
//	CRole::GetInstance()->SetRoleDirector( roleMoveStatus.vRoleDirector);
//	return true;
//}

bool CWorldModel::UpdateViewDirection( int xMoved , int yMoved )
{
	return this->m_mainRole.UpdateDirection( xMoved, yMoved );
}

CRolePointList CWorldModel::GetAllRole()
{
	CRolePointList allRole;
	allRole.push_back( &m_mainRole );
	return allRole;
}

CRole* CWorldModel::GetMainRole()
{
	return &m_mainRole;
}
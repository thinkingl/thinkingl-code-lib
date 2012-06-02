#include ".\worldmodel.h"
#include "role.h"
#include "ModelManager.h"

#using <mscorlib.dll>




CWorldModel::CWorldModel(void)
{
//	m_visualServer = new VisualServerTemp();	//  ����ʱ����һ����ʱ�ķ����������.
	// ��ý�ɫʵ����ָ��.	
}

CWorldModel::~CWorldModel(void)
{
}


// ��ǰ��һ��.
void CWorldModel::Forward(void)
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);//|SND_LOOP);

	m_mainRole.Walk( MoveDirector::Forward );
	//roleMoveStatus.vPos = roleMoveStatus.vPos + roleMoveStatus.vMoveDirector * roleMoveStatus.fSpeed ;
}

// �����һ��.
void CWorldModel::Back(void)
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);

	m_mainRole.Walk( MoveDirector::Back );
	//roleMoveStatus.vPos = roleMoveStatus.vPos - roleMoveStatus.vMoveDirector * roleMoveStatus.fSpeed ;
}

// ����һ��.
void CWorldModel::Left(void)
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);


	m_mainRole.Walk( MoveDirector::Left );
	//Vector3f yAxis = Vector3f( 0.0f , 1.0f ,0.0f );
	//Vector3f croosAxis = Normalize( Cross( roleMoveStatus.vMoveDirector , yAxis) );
	//roleMoveStatus.vPos = roleMoveStatus.vPos - croosAxis * roleMoveStatus.fSpeed ;
}

// ����һ��.
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

// ��ʼ��
bool CWorldModel::Init()
{

	//roleMoveStatus.fSpeed = 0.05f;
	//roleMoveStatus.vPos = Vector3f( 0.0f ,0.0f ,0.0f );
	//roleMoveStatus.vMoveDirector = roleMoveStatus.vRoleDirector = Vector3f( 0.0f , 0.0f ,-1.0f);
	//roleMoveStatus.vUp = Vector3f( 0.0f , 1.0f , 0.0f );
	return 0;
}

//
//������vRot����v��תangle�Ƕ� 
bool CWorldModel::RotateV( Vector3f &vRot, Vector3f v,float angle)
{

	vRot = Cross( vRot , v ) * sin(angle) + vRot * cos(angle);
 
    return 0;
}

// �������ڽ�ɫ�ķ���.
//void CWorldModel::UpdateRoleDirector(int xMoved , int yMoved)
//{
//	
//		// ���������˷������
//		static Vector3f crossAxis;
//		if(!(( roleMoveStatus.vRoleDirector.y > 0.9 ||  yMoved > 0 )&&( roleMoveStatus.vRoleDirector.y < -0.9 || yMoved < 0)))
//		{
//			crossAxis = Cross( roleMoveStatus.vRoleDirector , roleMoveStatus.vUp );
//			RotateV( roleMoveStatus.vRoleDirector ,crossAxis,yMoved * ROTATE_SPEED );
//		}
//		RotateV( roleMoveStatus.vRoleDirector , roleMoveStatus.vUp , xMoved * ROTATE_SPEED );
//	
//		// ��ý�ɫ�ĵ�ǰ�ƶ�����.
//		roleMoveStatus.vMoveDirector.x = roleMoveStatus.vRoleDirector.x;
//		roleMoveStatus.vMoveDirector.y = 0.0f ;
//		roleMoveStatus.vMoveDirector.z = roleMoveStatus.vRoleDirector.z;
//		roleMoveStatus.vMoveDirector.Normalize();
//}

// ���½�ɫ��Ϣ
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
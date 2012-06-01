#include ".\worldmodel.h"
#include "role.h"
#include "ModelManager.h"

#using <mscorlib.dll>




WorldModel::WorldModel(void)
{
//	m_visualServer = new VisualServerTemp();	//  ����ʱ����һ����ʱ�ķ����������.
	// ��ý�ɫʵ����ָ��.
	pRole = CRole::GetInstance();

}

WorldModel::~WorldModel(void)
{
}


// ��ǰ��һ��.
void WorldModel::Forward(void)
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);//|SND_LOOP);

	pRole->Walk( MoveDirector::Forward );
	//roleMoveStatus.vPos = roleMoveStatus.vPos + roleMoveStatus.vMoveDirector * roleMoveStatus.fSpeed ;
}

// �����һ��.
void WorldModel::Back(void)
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);

	pRole->Walk( MoveDirector::Back );
	//roleMoveStatus.vPos = roleMoveStatus.vPos - roleMoveStatus.vMoveDirector * roleMoveStatus.fSpeed ;
}

// ����һ��.
void WorldModel::Left(void)
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);


	pRole->Walk( MoveDirector::Left );
	//Vector3f yAxis = Vector3f( 0.0f , 1.0f ,0.0f );
	//Vector3f croosAxis = Normalize( Cross( roleMoveStatus.vMoveDirector , yAxis) );
	//roleMoveStatus.vPos = roleMoveStatus.vPos - croosAxis * roleMoveStatus.fSpeed ;
}

// ����һ��.
void WorldModel::Right(void)
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);


	pRole->Walk( MoveDirector::Right );
	//Vector3f yAxis = Vector3f( 0.0f , 1.0f ,0.0f );
	//Vector3f croosAxis = Normalize( Cross( roleMoveStatus.vMoveDirector , yAxis) );
	//roleMoveStatus.vPos = roleMoveStatus.vPos + croosAxis * roleMoveStatus.fSpeed ;
}

void WorldModel::RoleDosomeAction()
{
	static int action = ATTACK;
	pRole->DoAct( action, 2000 );
	action ++;

	if ( action > BOOM )
	{
		action = ATTACK;
	}
}

void WorldModel::Jump()
{
	PlaySound("data\\sound\\run.wav",NULL,SND_ASYNC | SND_NOSTOP);
	pRole->DoAct( JUMP, 500 );
}

void WorldModel::Fire()
{
	PlaySound("data\\sound\\Shot_low.wav",NULL,SND_ASYNC );
	pRole->DoAct( ATTACK, 500 );
}

// ��ʼ��
bool WorldModel::Init()
{

	//roleMoveStatus.fSpeed = 0.05f;
	//roleMoveStatus.vPos = Vector3f( 0.0f ,0.0f ,0.0f );
	//roleMoveStatus.vMoveDirector = roleMoveStatus.vRoleDirector = Vector3f( 0.0f , 0.0f ,-1.0f);
	//roleMoveStatus.vUp = Vector3f( 0.0f , 1.0f , 0.0f );
	return 0;
}

//
//������vRot����v��תangle�Ƕ� 
bool WorldModel::RotateV( Vector3f &vRot, Vector3f v,float angle)
{

	vRot = Cross( vRot , v ) * sin(angle) + vRot * cos(angle);
 
    return 0;
}

// �������ڽ�ɫ�ķ���.
//void WorldModel::UpdateRoleDirector(int xMoved , int yMoved)
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
//bool WorldModel::UpdateRoleState()
//{
//	CRole::GetInstance()->SetRolePos( roleMoveStatus.vPos );
//	CRole::GetInstance()->SetRoleDirector( roleMoveStatus.vRoleDirector);
//	return true;
//}


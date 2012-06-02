#include "Role.h"

//#include "MyMathArithmetic.cpp"

const float ROTATE_SPEED = 0.001f;
const int constTimeToRun = 500;

//CRole *CRole::m_singleton = 0;

CRole::CRole(void)
{
	Init();
}

CRole::~CRole(void)
{
}

bool CRole::Init()
{
	// ���ģ�͹������ʵ��ָ��.
//	pModelManager = ModelManager::GetInstance();

	roleState.vAbsolutePos = Vector3f( 0.0f ,0.0f ,0.0f );
	roleState.vDirection = Vector3f( 0.0f , 0.0f ,-1.0f );
	roleState.fSpeed = 0.03f;

	// ���ʱ����ʵ��.
//	m_pTimer = Timer::GetInstance();

	m_stopActionTimerId = 0;
	m_curRoleAct = STAND;
	return 0;
}

// ��ý�ɫ����.
stRolePropertiy CRole::GetRolePropertiy()
{
	return rolePropertiy;
}

// ��ý�ɫ״̬.
stRoleState CRole::GetRoleState()
{
	return roleState;
	
}

// CRole *CRole::GetInstance()
// {
// 	if( m_singleton == 0 )
// 		m_singleton = new CRole;
// 
// 	return ((CRole *)m_singleton);
// }
// 
// void CRole::FreeInstance()
// {
// 	if( m_singleton != 0 )
// 	{
// 		delete m_singleton;
// 		m_singleton = 0;
// 	}
// }

// ��ʾ��ɫ
void CRole::Show()
{
	glPushMatrix();

	//	ת����ǰ���ӵ�ͷ���Ϊ��ģ����׼��.
	glTranslatef( roleState.vAbsolutePos.x , roleState.vAbsolutePos.y + 1.0f , roleState.vAbsolutePos.z );
	glRotatef( 180/3.1416 * atan2( roleState.vDirection.z , roleState.vDirection.x ) - 90.0f  , 0.0f , -1.0f ,0.0f );
	ModelManager::GetInstance()->Show( 0 );
	glPopMatrix();
}

// ���ý�ɫλ�úͷ���
bool CRole::SetRolePos( Vector3f pos)
{
	roleState.vAbsolutePos = pos;
	return 0;
}

bool CRole::SetRoleDirector( Vector3f director )
{
	roleState.vDirection = director ;
	return 0;
}

// ��ý�ɫλ��.
Vector3f CRole::GetRolePos()
{
	return roleState.vAbsolutePos;
}

// ��ý�ɫ����
Vector3f CRole::GetRoleDirector()
{
	return roleState.vDirection;
}

// ��ָ��������һ��
bool CRole::Walk(enum MoveDirector director)
{
	static Vector3f yAxis  = Vector3f( 0.0f ,1.0f ,0.0f );
	static Vector3f crossAxis;
	static Vector3f moveDirection;
	int kl = 0;
	switch (director)
	{
	case Forward:	// ��ǰ

			//	��ý�ɫ�ƶ��ķ���.....����ɫ�ķ���y��������,Ȼ���׼��.
			moveDirection = Normalize( Vector3f( roleState.vDirection.x , 0.0f , roleState.vDirection.z ));
			roleState.vAbsolutePos = roleState.vAbsolutePos + moveDirection * roleState.fSpeed;
		{		
			static int timerForForward;				
			//	ģ�Ͷ���.ǰ��.
			DoAct( RUN, 500 );

			break;
		}
	case Back:
		{
			moveDirection = Normalize( Vector3f( roleState.vDirection.x , 0.0f , roleState.vDirection.z ));
			roleState.vAbsolutePos = roleState.vAbsolutePos - moveDirection * roleState.fSpeed;

			//  ģ�Ͷ���:����
			DoAct( RUN, 500 );

			break;
		}
	case Left:
		{
			crossAxis =  Normalize( Cross( roleState.vDirection , yAxis) );
			roleState.vAbsolutePos = roleState.vAbsolutePos - crossAxis * roleState.fSpeed ;

			//
			DoAct( RUN, 500 );

			break;
		}
	case Right:
		{
		crossAxis =  Normalize( Cross( roleState.vDirection , yAxis) );
		roleState.vAbsolutePos = roleState.vAbsolutePos + crossAxis * roleState.fSpeed ;

		DoAct( RUN, 500 );
		break;
		}
	}
	return true;
}

void WINAPI	CRole::StopActTimerProc (UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
{
	CRole *pThis = (CRole*)dwUser;
	if ( pThis )
	{
		pThis->DoAct( STAND, false );
	}
}

// �ı��ɫ����
bool CRole::UpdateDirection(int xMoved, int yMoved)
{
	// ���������˷������
		static Vector3f UpDirection = Vector3f( 0.0f ,1.0f ,0.0f );
		static Vector3f crossAxis;

		if(!(( roleState.vDirection.y > 0.9f ||  yMoved > 0 )&&( roleState.vDirection.y < -0.9f || yMoved < 0)))
		{
			crossAxis = Cross( roleState.vDirection , UpDirection );
			RotateV( roleState.vDirection ,crossAxis,yMoved * ROTATE_SPEED );

		}
		RotateV( roleState.vDirection , UpDirection , xMoved * ROTATE_SPEED );
//
	return true;
}

//	����
void CRole::DoAct(int action, int autoStopMSec )
{
	if ( action == m_curRoleAct )
	{	
		return;
	}
	else
	{
		this->m_curRoleAct = action;

		ModelManager::GetInstance()->SetAnimType( 0 , (animType_t)action );
		ModelManager::GetInstance()->SetAnimType( 1 , (animType_t)action );
	}

	if ( autoStopMSec > 0 )
	{
		this->ResetStopActionTimer( autoStopMSec );
	}
	else
	{
		this->KillStopActionTimer();
	}
	
}

void CRole::ResetStopActionTimer( int autoStopMSec )
{
	this->KillStopActionTimer();

	m_stopActionTimerId = timeSetEvent( autoStopMSec , 10 , (LPTIMECALLBACK) StopActTimerProc , (DWORD_PTR)this , TIME_ONESHOT );	
}

void CRole::KillStopActionTimer()
{
	if ( 0 != m_stopActionTimerId )
	{
		timeKillEvent( m_stopActionTimerId );
		m_stopActionTimerId = 0;
	}
}
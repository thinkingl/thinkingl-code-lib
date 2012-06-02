#include "Role.h"

//#include "MyMathArithmetic.cpp"

const float ROTATE_SPEED = 0.001f;
const int constTimeToRun = 500;

//CRole *CRole::m_singleton = 0;

CRole::CRole(void)
	:m_modelId( 0 )
{
	stRoleState initSt;
	initSt.vAbsolutePos = Vector3f( 0.0f ,0.0f ,0.0f );
	initSt.vDirection = Vector3f( 0.0f , 0.0f ,-1.0f );
	initSt.fSpeed = 0.03f;
	Init( initSt, 0 );
}

CRole::~CRole(void)
{
}

bool CRole::Init(  const stRoleState& initSt, int modelId  )
{
	// ���ģ�͹������ʵ��ָ��.
//	pModelManager = ModelManager::GetInstance();
	m_modelId = modelId;

	m_roleState = initSt;

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
	return m_roleState;
	
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
	glTranslatef( m_roleState.vAbsolutePos.x , m_roleState.vAbsolutePos.y + 1.0f , m_roleState.vAbsolutePos.z );
	glRotatef( 180/3.1416 * atan2( m_roleState.vDirection.z , m_roleState.vDirection.x ) - 90.0f  , 0.0f , -1.0f ,0.0f );
	ModelManager::GetInstance()->Show( m_modelId );
	glPopMatrix();
}

// ���ý�ɫλ�úͷ���
bool CRole::SetRolePos( Vector3f pos)
{
	m_roleState.vAbsolutePos = pos;
	return 0;
}

bool CRole::SetRoleDirector( Vector3f director )
{
	m_roleState.vDirection = director ;
	return 0;
}

// ��ý�ɫλ��.
Vector3f CRole::GetRolePos()
{
	return m_roleState.vAbsolutePos;
}

// ��ý�ɫ����
Vector3f CRole::GetRoleDirector()
{
	return m_roleState.vDirection;
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
			moveDirection = Normalize( Vector3f( m_roleState.vDirection.x , 0.0f , m_roleState.vDirection.z ));
			m_roleState.vAbsolutePos = m_roleState.vAbsolutePos + moveDirection * m_roleState.fSpeed;
		{		
			static int timerForForward;				
			//	ģ�Ͷ���.ǰ��.
			DoAct( RUN, 500 );

			break;
		}
	case Back:
		{
			moveDirection = Normalize( Vector3f( m_roleState.vDirection.x , 0.0f , m_roleState.vDirection.z ));
			m_roleState.vAbsolutePos = m_roleState.vAbsolutePos - moveDirection * m_roleState.fSpeed;

			//  ģ�Ͷ���:����
			DoAct( RUN, 500 );

			break;
		}
	case Left:
		{
			crossAxis =  Normalize( Cross( m_roleState.vDirection , yAxis) );
			m_roleState.vAbsolutePos = m_roleState.vAbsolutePos - crossAxis * m_roleState.fSpeed ;

			//
			DoAct( RUN, 500 );

			break;
		}
	case Right:
		{
		crossAxis =  Normalize( Cross( m_roleState.vDirection , yAxis) );
		m_roleState.vAbsolutePos = m_roleState.vAbsolutePos + crossAxis * m_roleState.fSpeed ;

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

		if(!(( m_roleState.vDirection.y > 0.9f ||  yMoved > 0 )&&( m_roleState.vDirection.y < -0.9f || yMoved < 0)))
		{
			crossAxis = Cross( m_roleState.vDirection , UpDirection );
			RotateV( m_roleState.vDirection ,crossAxis,yMoved * ROTATE_SPEED );

		}
		RotateV( m_roleState.vDirection , UpDirection , xMoved * ROTATE_SPEED );
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

		ModelManager::GetInstance()->SetAnimType( m_modelId , (animType_t)action );
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
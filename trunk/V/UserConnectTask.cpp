#include "UserConnectTask.h"


CUserConnectTask::CUserConnectTask( CVModel* pVModel, int userInternalId )
	:m_pVModel( pVModel )
	,m_userInternalId( userInternalId )
{

}


CUserConnectTask::~CUserConnectTask(void)
{
}

void CUserConnectTask::run()
{
	if ( m_pVModel && CUser::INVALID_USER_INTERNAL_ID != m_userInternalId )
	{
		//CUser userInfo;
		//bool bOk = m_pVModel->GetUser( m_userInternalId, userInfo );
		//Q_ASSERT( bOk );


	}
	else
	{
		Q_ASSERT( 0 );
	}
}

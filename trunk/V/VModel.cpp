#include "VModel.h"
#include <assert.h>
#include <QtDebug>
#include <QTimer>
#include <QThreadPool>

#include "UserConnectTask.h"

CVModel::CVModel(void)
	:m_maxUserInternalId( CUser::INVALID_USER_INTERNAL_ID )
	,m_curUserListPollIndex(0)
{
	QTimer *timer = new QTimer(this);
	connect( timer, SIGNAL(timeout()), this, SLOT( PollUserList() ) );
	timer->start(1000);
}

CVModel::~CVModel(void)
{
}

bool CVModel::AddUser( const string& ipAddr, int port )
{
	// �Ƿ��ظ����.
	for ( size_t i=0; i<m_userList.size(); ++i )
	{
		if ( m_userList[i].GetIP() == ipAddr && m_userList[i].GetPort() == port )
		{
			qDebug() << "User's net address already exist!";
			return false;
		}
	}

	// ���.
	m_maxUserInternalId++;
	CUser newUser( m_maxUserInternalId, ipAddr, port );
	m_userList.push_back( newUser );

	return true;
}

QRunnable* CVModel::GetUserConnectTask()
{
	return 0;
}

void CVModel::PollUserList()
{
	if ( m_userList.empty() )
	{
		return;
	}

	if ( m_curUserListPollIndex >= m_userList.size() )
	{
		m_curUserListPollIndex = 0;
	}

	CUser& userInfo = m_userList[m_curUserListPollIndex];

	CUserConnectTask* pTask = new CUserConnectTask( this, userInfo.GetTempInternalId() );
	QThreadPool::globalInstance()->start( pTask );

	m_curUserListPollIndex++;
}

bool CVModel::GetUser( int userInternalId, CUser& userInfo )
{
	for ( size_t i=0; i<m_userList.size(); ++i )
	{
		if ( userInternalId == m_userList[i].GetTempInternalId() )
		{
			userInfo = m_userList[i];
			return true;
		}
	}
	return false;
}

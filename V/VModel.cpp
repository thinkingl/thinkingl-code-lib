#include "VModel.h"
#include <assert.h>
#include <QtDebug>
#include <QTimer>
#include <QThreadPool>
#include <QTcpSocket>

#include "UserConnectTask.h"
#include "UserConfig.h"

CVModel::CVModel(void)
	:m_maxUserInternalId( CUser::INVALID_USER_INTERNAL_ID )
	,m_curUserListPollIndex(0)
{
	QTimer *timer = new QTimer(this);
	connect( timer, SIGNAL(timeout()), this, SLOT( PollUserList() ) );
	timer->start(1000);

	// 监听TCP端口,等待其它用户来连接.
	this->StartTcpService();
}

CVModel::~CVModel(void)
{
	for ( size_t i=0; i<m_userList.size(); ++i )
	{
		if ( m_userList[i] )
		{
			delete m_userList[i];
			m_userList[i] = 0;
		}
	}
	m_userList.clear();
}

bool CVModel::AddUser( const string& ipAddr, int port )
{
	// 是否重复添加.
	for ( size_t i=0; i<m_userList.size(); ++i )
	{
		if ( m_userList[i]->GetIP() == ipAddr && m_userList[i]->GetPort() == port )
		{
			qDebug() << "User's net address already exist!";
			return false;
		}
	}

	// 添加.
	m_maxUserInternalId++;
	CUser *pNewUser = new CUser( m_maxUserInternalId, ipAddr, port );
	m_userList.push_back( pNewUser );

	return true;
}

//QRunnable* CVModel::GetUserConnectTask()
//{
//	return 0;
//}

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

	CUser* userInfo = m_userList[m_curUserListPollIndex];

	userInfo->Work();

//	pNewConnect->connectToHost( userInfo.GetIP().c_str(), userInfo.GetPort() );



//	CUserConnectTask* pTask = new CUserConnectTask( this, userInfo.GetTempInternalId() );
//	QThreadPool::globalInstance()->start( pTask );

	m_curUserListPollIndex++;
}

//bool CVModel::GetUser( int userInternalId, CUser& userInfo )
//{
//	for ( size_t i=0; i<m_userList.size(); ++i )
//	{
//		if ( userInternalId == m_userList[i].GetTempInternalId() )
//		{
//			userInfo = m_userList[i];
//			return true;
//		}
//	}
//	return false;
//}

bool CVModel::StartTcpService()
{
	bool bOk = m_tcpServer.listen( QHostAddress::Any, 1984 );
	connect( &m_tcpServer, SIGNAL( newConnection() ), SLOT( OnUserConnect() ) );
	return bOk;
}

void CVModel::OnUserConnect()
{
	QTcpSocket* pSock = m_tcpServer.nextPendingConnection();
	if ( pSock && pSock->isValid() )
	{
		CUser* pInConnectingUser = new CUser;

		pInConnectingUser->FromConnect( pSock );
		m_inConnectingUserList.push_back( pInConnectingUser );
		//connect( pSock, SIGNAL( readyRead() ), SLOT( OnUserConnectReadyRead() ) );
		//pSock->write( "hello! On connect" );
	}
}

// void CVModel::OnUserConnectReadyRead()
// {
// 
// }
bool CVModel::Login(const QString& email, const QString& password)
{
	bool bOk = CUserConfig::CheckUserPassword(email, password);
	return bOk;
}

#include "VModel.h"
#include <assert.h>
#include <QtDebug>

CVModel::CVModel(void)
{
}


CVModel::~CVModel(void)
{
}

bool CVModel::AddUser( const string& ipAddr, int port )
{
	// 是否重复添加.
	for ( size_t i=0; i<m_userList.size(); ++i )
	{
		if ( m_userList[i].GetIP() == ipAddr && m_userList[i].GetPort() == port )
		{
			qDebug() << "User's net address already exist!";
			return false;
		}
	}

	// 添加.
	CUser newUser( ipAddr, port );
	m_userList.push_back( newUser );

	return true;
}

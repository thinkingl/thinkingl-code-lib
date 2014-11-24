#pragma once

/**
*	����ҵ��ģ����. ͬUI����.
*	+by lizhixing@2014-11-23
*/

#include "User.h"

#include <string>
using namespace std;

#include <QRunnable>
#include <QObject>

class CVModel : public QObject
{
	Q_OBJECT
public:
	CVModel(void);
	~CVModel(void);

public:
	// ���һ���û�.
	bool AddUser( const string& ipAddr, int port );

	// ��ȡ�û�.
	bool GetUser( int userInternalId, CUser& userInfo );

private slots:
	// ��ʱ����,�����û��б�.
	void PollUserList();

private:
	// ��ȡһ�������û���Task.
	QRunnable* GetUserConnectTask();

private:
	// �û��б�.
	CUserList m_userList;
	// �ۼӵ��û��ڲ�ID.
	int m_maxUserInternalId;

	// �����û��б��õĵ�ǰ�α�.
	int m_curUserListPollIndex;
};


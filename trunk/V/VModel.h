#pragma once

/**
*	����ҵ��ģ����. ͬUI����.
*	+by lizhixing@2014-11-23
*/

#include "User.h"

#include <string>
using namespace std;

class CVModel
{
public:
	CVModel(void);
	~CVModel(void);

public:
	// ���һ���û�.
	bool AddUser( const string& ipAddr, int port );

private:
	// �û��б�.
	CUserList m_userList;
};


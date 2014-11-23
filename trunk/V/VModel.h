#pragma once

/**
*	程序业务模型类. 同UI分离.
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
	// 添加一个用户.
	bool AddUser( const string& ipAddr, int port );

private:
	// 用户列表.
	CUserList m_userList;
};


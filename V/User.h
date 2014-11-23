#pragma once

/**
*	一个用户的类.
*	+by lizhixing@2014-11-23
*/

#include <string>
#include <vector>

using namespace std;

class CUser
{
public:
	CUser(void);
	CUser( const string& ip, int port );
	virtual ~CUser(void);

public:
	// 设置用户的地址.
	void SetNetAddr( const string& ip, int port );

	// 获取IP
	string GetIP() const;
	int GetPort() const;


private:
	// 用户的IP.
	string m_ip;
	// 用户的网络端口.
	int m_port;

	// 
};

typedef std::vector< CUser > CUserList;

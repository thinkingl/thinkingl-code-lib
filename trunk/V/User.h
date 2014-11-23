#pragma once

/**
*	һ���û�����.
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
	// �����û��ĵ�ַ.
	void SetNetAddr( const string& ip, int port );

	// ��ȡIP
	string GetIP() const;
	int GetPort() const;


private:
	// �û���IP.
	string m_ip;
	// �û�������˿�.
	int m_port;

	// 
};

typedef std::vector< CUser > CUserList;

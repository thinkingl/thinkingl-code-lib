#pragma once

#include <QStringList>

/** 
*	�����û���������Ϣ,ÿ���û������Լ�������.
*	+by lizhixing@2015-02-19
*/

class CUserConfig
{
public:
	CUserConfig(void);
	~CUserConfig(void);

	// ��ȡ�û����б�,��������Щ�û������ñ�����.
	static QStringList GetAllUserEmail();

	// ͨ������������ȡ.
	static CUserConfig* GetUserConfig( QString& email );

	// �û�ID.

	// �û�����.

	// �û���.

	// �û�EMAIL.

private:

};


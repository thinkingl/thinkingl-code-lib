#pragma once

#include <QStringList>

/** 
*	保存用户的配置信息,每个用户都有自己的配置.
*	+by lizhixing@2015-02-19
*/

class CUserConfig
{
public:
	CUserConfig(void);
	~CUserConfig(void);

	// 初始化.
	static bool Init();

	// 获取用户的列表,本地有哪些用户的配置保持着.
	static QStringList GetAllUserEmail();

	// 通过邮箱名来获取.
	static CUserConfig* GetUserConfig( QString& email );

	// 用户ID.

	// 用户密码.

	// 用户名.

	// 用户EMAIL.

private:


private:
	// 数据库对象.
};


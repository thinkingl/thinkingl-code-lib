#pragma once

#include <QStringList>
#include <QtSql/QSqlDatabase>

/** 
*	保存用户的配置信息,每个用户都有自己的配置.
*	+by lizhixing@2015-02-19
*/

class CUserConfig
{
public:
	CUserConfig(void);
	~CUserConfig(void);

	//static CUserConfig* GetInstance();
	//static void DestroyInstance();

	// 初始化.
	static bool Init();

	// 获取用户的列表,本地有哪些用户的配置保持着.
	static QStringList GetAllUserId();

	// 注册一个用户. 返回用户的uuid.
	static bool RegisterUser( const QString& email, const QString& userName, const QString& password );


	// 通过邮箱名来获取.
	static CUserConfig* GetUserConfig( const QString& userId );

	// 用户ID.
	QString GetUserId();

	// 用户密码.
	QString GetUserPassword();

	// 用户名.
	QString GetUserName();

	// 用户EMAIL.
	QString GetUserEmail();

private:


private:
	// 数据库对象.
	static QSqlDatabase s_database;

	// 单件对象.
	static CUserConfig* s_instance;
};


#pragma once

#include <QStringList>
#include <QtSql/QSqlDatabase>
#include "Friend.h"

/** 
*	保存用户的配置信息,每个用户都有自己的配置.
*	+by lizhixing@2015-02-19
*/

class CUserConfig
{
public:
	CUserConfig( const QString& userId );
	~CUserConfig(void);

	//static CUserConfig* GetInstance();
	//static void DestroyInstance();

	// 初始化.
	static bool Init();

	// 获取用户的列表,本地有哪些用户的配置保持着.
	static QStringList GetAllUserId();

	// 注册一个用户. 返回用户的uuid.
	static bool RegisterUser( const QString& email, const QString& userName, const QString& password );


	// 通过uuid来获取.
	//static CUserConfig* GetUserConfig( const QString& userId );

	// 检查用户密码.
	static bool CheckUserPassword( const QString& email, const QString& password );

	// 用户ID.
	QString GetUserId();

	// 用户密码.
	QString GetUserPassword();

	// 用户名.
	QString GetUserName();

	// 用户EMAIL.
	QString GetUserEmail();

	//////////////////////////////////////////////////////////////////////////
	// 用户好友数据的存储.
	CFriendList GetAllFriends();

	// 添加一个好友.
	bool AddFriend( const CFriend& f);
	// 删除一个好友.
	bool DelFriend( const QString& friendUuid );
	// 更新一个好友的信息.
	bool UpdateFriend(const CFriend& f);
	//////////////////////////////////////////////////////////////////////////

private:

	// 用户uuid.
	QString m_userId;
private:
	// 数据库对象.
	static QSqlDatabase s_database;

	// 单件对象.
	static CUserConfig* s_instance;
};


#include "UserConfig.h"
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QtSql/QSqlError>
#include <QtCore/qdebug.h>
#include <QtCore/QUuid>

CUserConfig* CUserConfig::s_instance = 0;
QSqlDatabase CUserConfig::s_database;

//CUserConfig* CUserConfig::GetInstance()
//{
//	if (0 == s_instance)
//	{
//		s_instance = new CUserConfig();
//	}
//	return s_instance;
//}
//
//void CUserConfig::DestroyInstance()
//{
//	if (s_instance)
//	{
//		delete s_instance;
//		s_instance = 0;
//	}
//}

CUserConfig::CUserConfig( const QString& userId )
	:m_userId( userId )
{
}


CUserConfig::~CUserConfig(void)
{
}

bool CUserConfig::Init()
{
	s_database = QSqlDatabase::addDatabase("QSQLITE");
	//s_database.setHostName("VUserConfigDB");
	s_database.setDatabaseName("usercfg.db");


	bool bOk = s_database.open();
	Q_ASSERT( bOk );

	QStringList tList = s_database.tables();

	const QString tableNameDbInfo = "dbinfo";

	if( !tList.contains( tableNameDbInfo ) )
	{
		// 创建数据表.
		// 一张数据库本身信息的表.
		// 数据库本身的信息表比较简单,只有一个字段, 版本.
		QSqlQuery sqlQuery(s_database);
		sqlQuery.prepare( "CREATE TABLE dbinfo(version TEXT PRIMARY KEY ASC);" );
		//	sqlQuery.bindValue( ":col1", "version" );
		bOk = sqlQuery.exec();

		// 插入一条记录,数据库的数据版本.
		if (bOk)
		{
			bOk = sqlQuery.exec("INSERT INTO dbinfo(version) VALUES('1.0');");
		}

		if( !bOk )
		{
			QSqlError err = sqlQuery.lastError();
			QString strEr = err.text();
			qDebug() << "db operation fail! er: " << strEr;
			Q_ASSERT( bOk );
		}
		s_database.commit();
	}
	else
	{
		// 查下版本,是否需要升级.
		// to be continued.
	}

	// 用户信息表.
	const QString tableNameUser = "user";
	if (!tList.contains(tableNameUser))
	{
		// 创建用户信息表.
		// 字段: 用户UUID ; 用户名; 用户邮箱; 
		QSqlQuery sqlCreateUserTable(s_database);
		bool bOk = sqlCreateUserTable.exec("CREATE TABLE user( uuid TEXT PRIMARY KEY, name TEXT, email TEXT UNIQUE, password TEXT);");
		if (!bOk)
		{
			qDebug() << "db operation fail! er: " << sqlCreateUserTable.lastError();
			Q_ASSERT(bOk);
		}
	}


// 	db.setHostName("MyServer");
// 	db.setDatabaseName("C:\\test.gdb");

	return true;
}

QString CUserConfig::GetUserId()
{
	Q_ASSERT(s_database.isValid());
	QSqlQuery sqlQuery(s_database);
	
	return "";
}

QStringList CUserConfig::GetAllUserId()
{
	Q_ASSERT(s_database.isValid());

	QStringList allUserId;
	QSqlQuery sqlQuery(s_database);
	sqlQuery.exec("select uuid from user;");
	while (sqlQuery.next())
	{
		QString userId = sqlQuery.value(0).toString();
		allUserId.push_back(userId);
	}
	return allUserId;
}

bool CUserConfig::RegisterUser(const QString& email, const QString& userName, const QString& password)
{
	QUuid uuid = QUuid::createUuid();

	QSqlQuery sqlQuery(s_database);
	sqlQuery.prepare("INSERT INTO user(uuid, email, name, password) VALUES(:uuid,:email,:name,:password);");
	sqlQuery.bindValue(":uuid", uuid.toString());
	sqlQuery.bindValue(":email", email);
	sqlQuery.bindValue(":name", userName);
	sqlQuery.bindValue(":password", password);
	bool bOk = sqlQuery.exec();
	if ( !bOk )
	{
		qDebug() << "Register user fail! er:" << sqlQuery.lastError().text();
	}
	return bOk;
}

bool CUserConfig::CheckUserPassword(const QString& email, const QString& password)
{
	QSqlQuery sqlQuery(s_database);
	sqlQuery.prepare("SELECT COUNT(0) from user where email=:email AND password=:password");
	sqlQuery.bindValue(":email", email);
	sqlQuery.bindValue(":password", password);
	bool bOk = sqlQuery.exec();
	if ( !bOk )
	{
		qDebug() << "Check user password sql error: " << sqlQuery.lastError().text();
	}

	if ( sqlQuery.next() )
	{
		int count = sqlQuery.value(0).toInt();
		return count > 0;
	}


	return bOk;
}

QString CUserConfig::GetUserEmail()
{
	QSqlQuery sqlQuery(s_database);
	sqlQuery.prepare("SELECT email from user where uuid=:uuid");
	sqlQuery.bindValue(":uuid", m_userId);
	bool bOk = sqlQuery.exec();
	if ( !bOk )
	{
		qDebug() << "sql: " << sqlQuery.lastQuery() << " er: " << sqlQuery.lastError().text();
		return "";
	}
	else
	{
		sqlQuery.next();
		QString email = sqlQuery.value(0).toString();
		return email;
	}
}

// CUserConfig* CUserConfig::GetUserConfig(const QString& userId)
// {
// 
// 	CUserConfig* pCfg = new CUserConfig( userId );
// 	return pCfg;
// }

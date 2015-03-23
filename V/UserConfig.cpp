#include "UserConfig.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QtSql/QSqlError>

CUserConfig::CUserConfig(void)
{
}


CUserConfig::~CUserConfig(void)
{
}

bool CUserConfig::Init()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	//db.setHostName("VUserConfigDB");
	db.setDatabaseName("usercfg.db");


	bool bOk = db.open();
	Q_ASSERT( bOk );

	QStringList tList = db.tables();

	const QString tableNameDbInfo = "dbinfo";

	if( !tList.contains( tableNameDbInfo ) )
	{
		// 创建数据表.
		// 一张数据库本身信息的表.
		QSqlQuery sqlQuery(db);
		sqlQuery.prepare( "CREATE TABLE dbinfo(version TEXT PRIMARY KEY ASC);" );
		//	sqlQuery.bindValue( ":col1", "version" );
		bOk = sqlQuery.exec();

		// 插入一条记录,数据库的数据版本.
		sqlQuery.exec( "INSERT INTO dbinfo(version) VALUES('1.0');" );

		if( !bOk )
		{
			QSqlError err = sqlQuery.lastError();
			QString strEr = err.text();
			Q_ASSERT( bOk );
		}
	}
	else
	{
		// 查下版本,是否需要
	}
	


	db.commit();

// 	db.setHostName("MyServer");
// 	db.setDatabaseName("C:\\test.gdb");

	return true;
}

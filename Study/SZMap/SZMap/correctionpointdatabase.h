#pragma once

#include "Common.h"

// 前置声明.
struct sqlite3;

class CCorrectionPointDatabase
{
public:

	CCorrectionPointDatabase();
	~CCorrectionPointDatabase();

	/** 打开并初始化数据库文件. */
	bool OpenDatabase( ctstring& databasePath );

	void Close();

	/** 插入一个校正点. */
	bool InsertCorrectionPoint( double earthLongitude, double earthLatitude, 
		double marsLongitude, double marsLatitude );

	/** 查询. */



private:
	/** sqlite databse object. */
	sqlite3 *m_pSqlite3;
};

// end of the file



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
	struct TRecord
	{
		double m_earthLongitude;
		double m_earthLatitude;
		double m_marsLongitude;
		double m_marsLatitude;
	};
	typedef std::vector< TRecord > TRecordList;
	TRecordList GetEarthPointsAround( double earthLongitude, double earthLatitude, double distance );
	TRecordList GetMarsPointsAround( double marsLongitude, double marsLatitude, double distance );


private:
	/** sqlite databse object. */
	sqlite3 *m_pSqlite3;

	/** 优化性能, 用事务批量插入. */
	int m_recordCountWaitCommit;
};

// end of the file



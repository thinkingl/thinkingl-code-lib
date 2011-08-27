#include "correctionpointdatabase.h"
#include "Log.h"

#include "sqlite3.h"
#include <iostream>

// 数据库表明.
ctstring DB_TABLE_NAME = _T( "CorrectionPoints" );

// 地球坐标经度.
ctstring DB_COL_EARTH_LONGITUDE = _T( "EarthLongitude" );
ctstring DB_COL_EARTH_LATITUDE = _T( "EarthLatitude" );

ctstring DB_COL_MARS_LONGITUDE = _T( "MarsLongitude" );
ctstring DB_COL_MARS_LATITUDE = _T( "MarsLatitude" );

ctstring DB_PK_TABLE = _T( "pk_CorrectionPoints" );
ctstring DB_IDX_MARS_COORATION = _T( "IdxMars" );

CCorrectionPointDatabase::CCorrectionPointDatabase()
{
	m_pSqlite3 = NULL;
	m_recordCountWaitCommit = 0;
}

CCorrectionPointDatabase::~CCorrectionPointDatabase()
{
	this->Close();
}

bool CCorrectionPointDatabase::OpenDatabase( ctstring& databasePath )
{
	if ( m_pSqlite3 )
	{
		Log( ) << _T( "Database has opened !!!!!!!!!!" ) << endl;
		ASSERT( FALSE );
	}
	
	bool bResult = TRUE;

	int nOpenRet = sqlite3_open16( databasePath.c_str(), &this->m_pSqlite3 );
	Log() << _T( "Open sqlite database file: " ) << databasePath << _T( " ret: " ) << nOpenRet << endl;
	ASSERT( SQLITE_OK==nOpenRet );
	bResult &= ( SQLITE_OK == nOpenRet );

	if ( m_pSqlite3 && bResult )
	{
		// 检测表是否存在。
		int nRow = 0, nColumn = 0;
		char **azResult; //二维数组存放结果
		char *errMsg = NULL;


		tstringstream ssSql;
		ssSql << _T( "select name from sqlite_master " << " where name='" ) << DB_TABLE_NAME << _T( "';" );
		string strUtf8Sql;
		CCommon::Utf16toUtf8( ssSql.str().c_str(), strUtf8Sql );
		int nGetTableRet = sqlite3_get_table( this->m_pSqlite3, strUtf8Sql.c_str(),&azResult, &nRow, &nColumn, &errMsg );
		bResult &= ( nGetTableRet == SQLITE_OK );
		ASSERT( bResult );
		ASSERT( nRow <= 1 );
		Log() << _T( "sql: " ) << strUtf8Sql.c_str() << _T( " ret: " ) << nGetTableRet 
			<< _T( " row: " ) << nRow << _T( " col: " ) << nColumn << endl;

		sqlite3_free_table( azResult );

		if ( errMsg )
		{
			Log() << errMsg << endl;
		}
		if ( nRow > 0 )
		{
			Log() << _T( "table already exist!!!" ) << endl;
		}
		else
		{
			// 创建表。
			errMsg = NULL;
			tstringstream ssSql;
			ssSql << _T( "CREATE TABLE " ) << DB_TABLE_NAME
				<< _T( " ( ") 
				<< DB_COL_EARTH_LONGITUDE << _T( " DECIMAL(10,10), " )				
				<< DB_COL_EARTH_LATITUDE	<< _T( " DECIMAL(10,10), ") 				
				<< DB_COL_MARS_LONGITUDE << _T( " DECIMAL(10,10), ")
				<< DB_COL_MARS_LATITUDE << _T( " DECIMAL(10,10), " ) 
				<< _T( "constraint " ) << DB_PK_TABLE << _T( "  PRIMARY KEY " )
				<< _T( "(" ) << DB_COL_EARTH_LONGITUDE << _T( ", " ) << DB_COL_EARTH_LATITUDE << _T( ")" )
				<< _T( ");" );
			string strUtf8Sql ;
			CCommon::Utf16toUtf8( ssSql.str().c_str(), strUtf8Sql );
			int nCreateTableRet = sqlite3_exec( m_pSqlite3, 
				strUtf8Sql.c_str(), 
				0, 0, &errMsg );
			bResult &= ( nCreateTableRet == SQLITE_OK );
			ASSERT( bResult );
			Log() << _T( "Create table ret: " ) << nCreateTableRet << endl;
			Log() << _T( "sql: " ) << strUtf8Sql.c_str() << endl;
			if ( errMsg )
			{
				Log() << _T( "Sqlite Error msg: " ) << errMsg << endl;
			}

			// 为火星坐标的两列创建索引.
			char * createMarsIndexErr = 0;
			tstringstream ssSqlCreateMarsIndex;
			ssSqlCreateMarsIndex << _T( "CREATE INDEX " ) << DB_IDX_MARS_COORATION 
				<< _T( " ON " ) << DB_TABLE_NAME 
				<< _T( "(" ) 
				<< DB_COL_MARS_LONGITUDE << _T( "," ) << DB_COL_MARS_LATITUDE 
				<< _T( ");" );
			string strUtf8CreateMarsIndex;
			CCommon::Utf16toUtf8( ssSqlCreateMarsIndex.str().c_str(), strUtf8CreateMarsIndex );
			int createIndexRet = sqlite3_exec( m_pSqlite3, 
				strUtf8CreateMarsIndex.c_str(), 
				0, 0, &createMarsIndexErr );
			bResult &= ( createIndexRet == SQLITE_OK );
			ASSERT( bResult );
			Log() << _T( "Create index ret: " ) << createIndexRet << endl;
			Log() << _T( "sql: " ) << strUtf8CreateMarsIndex << endl;
			if ( createMarsIndexErr )
			{
				Log() << _T( "Sqlite Error msg: " ) << errMsg << endl;
			}
		}
	}	
	ASSERT( bResult );
	return bResult;
}

void CCorrectionPointDatabase::Close()
{
	
	if( m_pSqlite3 )
	{
		// 关闭前要commit.
		if ( m_recordCountWaitCommit > 0 )
		{
			char *zErrMsg = NULL;
			int rc = sqlite3_exec( m_pSqlite3, "COMMIT;", 0, 0, &zErrMsg);
			if ( rc != SQLITE_OK )
			{
				Log() << "COMMIT fail! ermsg: " << zErrMsg << endl;
			}
			m_recordCountWaitCommit = 0;
		}

		sqlite3_close( m_pSqlite3 );
		m_pSqlite3 = NULL;
	}
	//	ASSERT( FALSE );
}

bool CCorrectionPointDatabase::InsertCorrectionPoint( double earthLongitude, double earthLatitude, double marsLongitude, double marsLatitude )
{
	bool bResult = TRUE;
	char *errMsg = NULL;

	// 启用事务.	
	if( m_recordCountWaitCommit > 10000 )
	{
		char *zErrMsg = NULL;
		int rc = sqlite3_exec( m_pSqlite3, "COMMIT;", 0, 0, &zErrMsg);
		if ( rc != SQLITE_OK )
		{
			ASSERT( FALSE );
			Log() << "COMMIT fail! ermsg: " << zErrMsg << endl;
		}
		m_recordCountWaitCommit = 0;
	}

	if ( m_recordCountWaitCommit == 0 )
	{
		int rc = sqlite3_exec( m_pSqlite3, "BEGIN;", 0, 0, &errMsg); 
		if ( errMsg )
		{
			ASSERT( FALSE );
			Log() << _T( "开启事务失败 msg:" ) << errMsg << endl;
		}
		errMsg = NULL;
	}
	m_recordCountWaitCommit ++;

	// 插入
	
	tstringstream ssSql;
	ssSql.precision( 16 );
	ssSql << _T( "INSERT INTO " ) << DB_TABLE_NAME
		<< _T( " ( ") << DB_COL_EARTH_LONGITUDE << _T( ", ") 
		<< DB_COL_EARTH_LATITUDE	<< _T( ", ") 
		<< DB_COL_MARS_LONGITUDE << _T( ", ")
		<< DB_COL_MARS_LATITUDE << _T( " )" )
		<< _T( " VALUES ( '" ) << earthLongitude
		<< _T( "', '" ) << earthLatitude
		<< _T( "', '" ) << marsLongitude
		<< _T( "', '" ) << marsLatitude
		<< _T( "' );" ) ;
	string strUtf8Sql ;
	CCommon::Utf16toUtf8( ssSql.str().c_str(), strUtf8Sql );
	int nInsertRet = sqlite3_exec( m_pSqlite3, 
		strUtf8Sql.c_str(), 
		0, 0, &errMsg );
	bResult &= ( nInsertRet == SQLITE_OK );
	ASSERT( bResult );
	Log() << _T( "Insert item ret: " ) << nInsertRet << endl;
	Log() << _T( "sql: " ) << strUtf8Sql.c_str() << endl;
	if ( errMsg )
	{
		Log() << _T( "Sqlite Error msg: " ) << errMsg << endl;
	}

	return bResult;
}

CCorrectionPointDatabase::TRecordList CCorrectionPointDatabase::GetMarsPointsAround( double marsLongitude, double marsLatitude, double distance )
{
	CCorrectionPointDatabase::TRecordList recordList;

	tstringstream ssSql;
	ssSql.precision( 16 );
	ssSql << _T( "SELECT * FROM " ) << DB_TABLE_NAME 
		<< _T( " WHERE  " ) << DB_COL_MARS_LONGITUDE << _T( ">= " ) << marsLongitude - distance
		<< _T( " AND " ) << DB_COL_MARS_LONGITUDE << _T( "<= " ) << marsLongitude + distance
		<< _T( " AND " ) << DB_COL_MARS_LATITUDE << _T( ">= " ) << marsLatitude - distance
		<< _T( " AND " ) << DB_COL_MARS_LATITUDE << _T( "<= " ) << marsLatitude + distance
	//	<< _T( "LIMIT " ) << nCount 
		<< _T( ";" );

	sqlite3_stmt *stmt = 0;
	const void *pzTail = 0;
	tstring sqlCmd = ssSql.str();
	int ret = sqlite3_prepare16_v2( m_pSqlite3,
		sqlCmd.c_str(),
		-1,
		&stmt,
		&pzTail );
	Log() << "sql: " << endl << sqlCmd << endl;
	if ( SQLITE_OK != ret )
	{
		Log() << "sqlite3_prepare16_v2 fail" << endl;
	}
	while( 1 )
	{
		ret = sqlite3_step( stmt );
		if ( SQLITE_OK != ret )
		{
			Log() << "sqlite3_step fail " << endl;
			break;
		}
		// 读取.
		// 地球经度 地球纬度 火星经度 火星纬度. 
		TRecord tmp;
		tmp.m_marsLongitude = sqlite3_column_double( stmt, 2 );
		tmp.m_marsLatitude = sqlite3_column_double( stmt, 3 );
		tmp.m_earthLongitude = sqlite3_column_double( stmt, 0 );
		tmp.m_earthLatitude = sqlite3_column_double( stmt, 1 );
		recordList.push_back( tmp );

	}
	
	ret = sqlite3_finalize( stmt );
	if ( SQLITE_OK != ret )
	{
		Log() << "" << endl;
	}

	// 测试插值算法是否正确.
	TRecord lt, lb, rt, rb;
	lt.m_marsLatitude = rt.m_marsLatitude = marsLatitude + 1;
	rt.m_marsLongitude = rb.m_marsLongitude = marsLongitude + 1;
	rb.m_marsLatitude = lb.m_marsLatitude = marsLatitude - 1;
	lb.m_marsLongitude = lt.m_marsLongitude = marsLongitude - 1;

	// 左下0,0 -> 右上10,10
	lt.m_earthLatitude = rt.m_earthLatitude = 10;
	rt.m_earthLongitude = rb.m_earthLongitude = 10;
	rb.m_earthLatitude = lb.m_earthLatitude = 0;
	lb.m_earthLongitude = lt.m_earthLongitude = 0;

	recordList.push_back( rb );
	recordList.push_back( rt );
	recordList.push_back( lb );
	recordList.push_back( lt );

	return recordList;
}
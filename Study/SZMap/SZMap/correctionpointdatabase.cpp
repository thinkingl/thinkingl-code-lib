#include "correctionpointdatabase.h"
#include "Log.h"

#include "sqlite3.h"

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
		sqlite3_close( m_pSqlite3 );
		m_pSqlite3 = NULL;
	}
	//	ASSERT( FALSE );
}
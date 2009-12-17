#include "..\include\SqliteDatabase.h"

#include "Common.h"
#include "Log.h"

LPCTSTR DATABASE_FILE_NAME = _T( "WEBFETCH.DB" );

LPCTSTR DB_TABLE_NAME = _T( "pagestate" );

LPCTSTR DB_COL_URL = _T( "Url" );
LPCTSTR DB_COL_CACHE = _T( "Cache" );
LPCTSTR DB_COL_SAVE = _T( "Save" );
LPCTSTR DB_COL_STATE = _T( "State" );


CSqliteDatabase::CSqliteDatabase(void)
{
	this->m_pSqlite3 = NULL;
}

CSqliteDatabase::~CSqliteDatabase(void)
{
}

BOOL CSqliteDatabase::Open()
{
	if ( m_pSqlite3 )
	{
		Log( ) << _T( "Database has opened !!!!!!!!!!" ) << endl;
		ASSERT( FALSE );
	}
	tstring strDatasePath = CCommon::GetAppDir();
	strDatasePath += DATABASE_FILE_NAME;

	BOOL bResult = TRUE;

	int nOpenRet = sqlite3_open16( strDatasePath.c_str(), &this->m_pSqlite3 );
	Log() << _T( "Open sqlite database file: " ) << strDatasePath << _T( " ret: " ) << nOpenRet << endl;
	ASSERT( SQLITE_OK==nOpenRet );
	bResult &= ( SQLITE_OK == nOpenRet );

	if ( m_pSqlite3 && bResult )
	{
		// 检测表是否存在。
		int nRow = 0, nColumn = 0;
		char **azResult; //二维数组存放结果
		char *errMsg = NULL;

		
		tstringstream ssSql;
		ssSql << _T( "select name from sqlite_master " << " where name='" ) << DB_TABLE_NAME << _T( "'" );
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
				<< _T( " ( ") << DB_COL_URL << _T( " TEXT PRIMARY KEY, ") 
				<< DB_COL_CACHE	<< _T( " TEXT, ") 
				<< DB_COL_SAVE << _T( " TEXT, ")
				<< DB_COL_STATE << _T( " INTEGER )" );
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
		}


		
	}	

	ASSERT( bResult );
	return bResult;
}

BOOL CSqliteDatabase::Close()
{
    if( m_pSqlite3 )
    {
        sqlite3_close( m_pSqlite3 );
        m_pSqlite3 = NULL;
    }
	ASSERT( FALSE );
	return FALSE;
}

BOOL CSqliteDatabase::GetUrlByStateByNum( EWebpageState eState, TUrlRecordItemList& tRecordItemList, int nCount )
{
	ASSERT( m_pSqlite3 );
	if ( m_pSqlite3 )
	{
		BOOL bResult = TRUE;
		// 查询。
		tstringstream ssSql;
		ssSql << _T( "SELECT * FROM " ) << DB_TABLE_NAME 
			<< _T( " WHERE  " ) << DB_COL_STATE << _T( "='" ) << (int)eState << _T( "' " )
			<< _T( "LIMIT " ) << nCount << _T( "" );

		int nRow = 0, nColumn = 0;
		char **azResult; //二维数组存放结果
		char *errMsg = NULL;

		string strUtf8Sql;
		CCommon::Utf16toUtf8( ssSql.str().c_str(), strUtf8Sql );

//test		strUtf8Sql = " select * from pagestate  limit 5;" ;

		int nGetTableRet = sqlite3_get_table( this->m_pSqlite3, strUtf8Sql.c_str(),&azResult, &nRow, &nColumn, &errMsg );
		bResult &= ( nGetTableRet == SQLITE_OK );
		ASSERT( bResult );
		Log() << _T( "sql: " ) << strUtf8Sql.c_str() << _T( " ret: " ) << nGetTableRet 
			<< _T( " row: " ) << nRow << _T( " col: " ) << nColumn << endl;
		if ( errMsg )
		{
			Log() << errMsg << endl;
		}

		// 第一行是列的名称。
		for ( int i=1; i<nRow; ++i )
		{			
			if ( i > 0 )
			{				
				TUrlRecordItem item;
				char ** azRow = &azResult[ i * nColumn ];
//				item.m_strUrl = *azRow;
				CCommon::Utf8toUtf16( *azRow, item.m_strUrl );
				azRow ++;
//				item.m_strCachePath = *azRow;
				CCommon::Utf8toUtf16( *azRow, item.m_strCachePath );
				azRow ++;
//				item.m_strSavePath = *azRow;
				CCommon::Utf8toUtf16( *azRow, item.m_strSavePath );
				azRow ++;
				item.m_eState = ( EWebpageState ) atoi( *azRow );

				tRecordItemList.push_back( item );
			}			
		}
		sqlite3_free_table( azResult );
		
		ASSERT( bResult );
		return bResult;
	}
	ASSERT( FALSE );
	return FALSE;
}

BOOL CSqliteDatabase::SearchUrl( LPCTSTR strUrl, TUrlRecordItem& item )
{
	ASSERT( m_pSqlite3 );
	if ( m_pSqlite3 )
	{
		BOOL bResult = TRUE;

		//test		
//		strUrl = _T( "url15" );

		// 查询。
		tstringstream ssSql;
		ssSql << _T( "SELECT * FROM " ) << DB_TABLE_NAME 
			<< _T( " WHERE  " ) << DB_COL_URL << _T( "='" ) << strUrl << _T( "' " )
			<< _T( "" );

		int nRow = 0, nColumn = 0;
		char **azResult; //二维数组存放结果
		char *errMsg = NULL;

		string strUtf8Sql;
		CCommon::Utf16toUtf8( ssSql.str().c_str(), strUtf8Sql );

		

		int nGetTableRet = sqlite3_get_table( this->m_pSqlite3, strUtf8Sql.c_str(),&azResult, &nRow, &nColumn, &errMsg );
		bResult &= ( nGetTableRet == SQLITE_OK );
		ASSERT( bResult );
		Log() << _T( "sql: " ) << strUtf8Sql.c_str() << _T( " ret: " ) << nGetTableRet 
			<< _T( " row: " ) << nRow << _T( " col: " ) << nColumn << endl;
		if ( errMsg )
		{
			Log() << errMsg << endl;
		}

		BOOL bHaveRet = ( nRow > 0 );
		if ( bHaveRet )
		{
			char ** azRow = &azResult[ nColumn ];
			//				item.m_strUrl = *azRow;
			CCommon::Utf8toUtf16( *azRow, item.m_strUrl );
			azRow ++;
			//				item.m_strCachePath = *azRow;
			CCommon::Utf8toUtf16( *azRow, item.m_strCachePath );
			azRow ++;
			//				item.m_strSavePath = *azRow;
			CCommon::Utf8toUtf16( *azRow, item.m_strSavePath );
			azRow ++;
			item.m_eState = ( EWebpageState ) atoi( *azRow );
		}

		sqlite3_free_table( azResult );

		return bResult & bHaveRet;
	}
	ASSERT( FALSE );
	return FALSE;
}

BOOL CSqliteDatabase::AddRecord( const TUrlRecordItem& item )
{
	ASSERT( FALSE );
	return FALSE;
}
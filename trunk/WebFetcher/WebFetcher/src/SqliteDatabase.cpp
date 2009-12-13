#include "..\include\SqliteDatabase.h"

#include "Common.h"
#include "Log.h"

LPCTSTR DATABASE_FILE_NAME = _T( "WEBFETCH.DB" );

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
		CLog( ) << _T( "Database has opened !!!!!!!!!!" ) << endl;
		ASSERT( FALSE );
	}
	tstring strDatasePath = CCommon::GetAppDir();
	strDatasePath += DATABASE_FILE_NAME;

	BOOL bResult = TRUE;

	int nOpenRet = sqlite3_open16( strDatasePath.c_str(), &this->m_pSqlite3 );
	CLog() << _T( "Open sqlite database file: " ) << strDatasePath << _T( " ret: " ) << nOpenRet << endl;
	ASSERT( SQLITE_OK==nOpenRet );
	bResult &= ( SQLITE_OK == nOpenRet );

	if ( m_pSqlite3 && bResult )
	{
		// 检测表是否存在。
		LPCTSTR strTableName = _T( "pagestate" );
		tstringstream ssSql;
		ssSql << _T( "select name from sqlite_master " << " where name='" ) << strTableName << _T( "'" );
		//sqlite3_stmt *pSqliteStmt;
		//const void *pTail;
		//int nPrepareRet = sqlite3_prepare16_v2( this->m_pSqlite3, ssSql.str().c_str(), ssSql.str().length(), &pSqliteStmt, &pTail );
		//ASSERT( nPrepareRet == SQLITE_OK );
		//int nStepRet = sqlite3_step( pSqliteStmt );
		//ASSERT( nStepRet == SQLITE_OK );

		//int nFinalizeRet = sqlite3_finalize( pSqliteStmt );
		//ASSERT( nFinalizeRet == SQLITE_OK );

		// 创建表。
		char *errMsg = NULL;
		int nCreateTableRet = sqlite3_exec( m_pSqlite3, 
			"CREATE TABLE test(name text)", 
			0, 0, &errMsg);
		bResult = ( nCreateTableRet == SQLITE_OK );
		CLog() << _T( "Create table ret: " ) << nCreateTableRet << endl;
		if ( errMsg )
		{
			CLog() << _T( "Sqlite Error msg: " ) << errMsg << endl;
		}
	}

	


	ASSERT( FALSE );
	return bResult;
}

BOOL CSqliteDatabase::Close()
{
	ASSERT( FALSE );
	return FALSE;
}
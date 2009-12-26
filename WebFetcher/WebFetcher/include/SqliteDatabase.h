#pragma once
#include "idatabase.h"

#include "../../../sqlite/include/sqlite3.h"

class CSqliteDatabase :
	public IDatabase
{
public:
	CSqliteDatabase(void);
	virtual ~CSqliteDatabase(void);

	// 打开。
	virtual BOOL Open() ;

	// 关闭。 
	virtual BOOL Close() ;

	// 根据状态获取一条url记录。
	/** 根据指定状态获取指定条数的 url记录。*/
	virtual BOOL GetUrlByStateByNum( EWebpageState eState, 
		TUrlRecordItemList& tRecordItemList, int nCount ) ;

	/** 查找一条url记录。 */
	virtual BOOL SearchUrl( LPCTSTR strUrl, TUrlRecordItem& item );

	/** 添加一条记录。 */
	virtual BOOL AddRecord( const TUrlRecordItem& item ) ;

	/** 修改一条已经存在的记录。 */
	virtual BOOL ModifyRecord( const TUrlRecordItem& item );

	/** 只修改一条记录中的状态。 */
	virtual BOOL ModifyWebpageState( LPCTSTR strUrl, EWebpageState eStat );

	/** 添加一条失败url记录. */
	virtual BOOL AddFailUrl( LPCTSTR strFailUrl, LPCTSTR strPageOfUrl ) ;

private:
	/** 创建表。 */
	BOOL CreateTable();

private:
	/** sqlite databse object. */
	sqlite3 *m_pSqlite3;
};

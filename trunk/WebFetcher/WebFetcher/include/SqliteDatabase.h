#pragma once
#include "idatabase.h"

#include "../../../sqlite/include/sqlite3.h"

class CSqliteDatabase :
	public IDatabase
{
public:
	CSqliteDatabase(void);
	virtual ~CSqliteDatabase(void);

	// �򿪡�
	virtual BOOL Open() ;

	// �رա� 
	virtual BOOL Close() ;

private:
	/** sqlite databse object. */
	sqlite3 *m_pSqlite3;
};

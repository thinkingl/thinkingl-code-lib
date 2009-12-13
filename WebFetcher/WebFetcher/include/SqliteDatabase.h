#pragma once
#include "idatabase.h"

#include "../../../sqlite/include/sqlite3.h"

class CSqliteDatabase :
	public IDatabase
{
public:
	CSqliteDatabase(void);
	virtual ~CSqliteDatabase(void);

	// ´ò¿ª¡£
	virtual BOOL Open() ;

	// ¹Ø±Õ¡£ 
	virtual BOOL Close() ;

private:
	/** sqlite databse object. */
	sqlite3 *m_pSqlite3;
};

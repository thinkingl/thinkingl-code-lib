#pragma once

#include "Common.h"

// ǰ������.
struct sqlite3;

class CCorrectionPointDatabase
{
public:

	CCorrectionPointDatabase();
	~CCorrectionPointDatabase();

	/** �򿪲���ʼ�����ݿ��ļ�. */
	bool OpenDatabase( ctstring& databasePath );

	void Close();

	/** ����һ��У����. */
	bool InsertCorrectionPoint( double earthLongitude, double earthLatitude, 
		double marsLongitude, double marsLatitude );

	/** ��ѯ. */



private:
	/** sqlite databse object. */
	sqlite3 *m_pSqlite3;
};

// end of the file



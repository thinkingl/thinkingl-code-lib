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

	/** �Ż�����, ��������������. */
	int m_recordCountWaitCommit;
};

// end of the file



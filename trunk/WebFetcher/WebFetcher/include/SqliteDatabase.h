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

	// ����״̬��ȡһ��url��¼��
	/** ����ָ��״̬��ȡָ�������� url��¼��*/
	virtual BOOL GetUrlByStateByNum( EWebpageState eState, 
		TUrlRecordItemList& tRecordItemList, int nCount ) ;

	/** ����һ��url��¼�� */
	virtual BOOL SearchUrl( LPCTSTR strUrl, TUrlRecordItem& item );

	/** ���һ����¼�� */
	virtual BOOL AddRecord( const TUrlRecordItem& item ) ;

	/** �޸�һ���Ѿ����ڵļ�¼�� */
	virtual BOOL ModifyRecord( const TUrlRecordItem& item );

	/** ֻ�޸�һ����¼�е�״̬�� */
	virtual BOOL ModifyWebpageState( LPCTSTR strUrl, EWebpageState eStat );

	/** ���һ��ʧ��url��¼. */
	virtual BOOL AddFailUrl( LPCTSTR strFailUrl, LPCTSTR strPageOfUrl ) ;

private:
	/** ������ */
	BOOL CreateTable();

private:
	/** sqlite databse object. */
	sqlite3 *m_pSqlite3;
};

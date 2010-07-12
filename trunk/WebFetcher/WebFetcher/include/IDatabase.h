#pragma once

#include "portabledefine.h"

class IDatabase
{
public:
	IDatabase(void);
	virtual ~IDatabase(void);

	enum EWebpageState
	{
		PageStateSaved,		// �Ѿ����档
		PageStateCached,	// �Ѿ������档
		PageStateWaiting,	// �������أ���û�����ص����ء�
	};

	/** ���ݿ��еļ�¼� */
	struct TUrlRecordItem
	{
		tstring m_strUrl;		// url������url��
		tstring m_strCachePath;	// ����·�������·����
		tstring m_strSavePath;	// ����·�������·����
		EWebpageState m_eState;	// ״̬��
	};
	typedef std::vector< TUrlRecordItem > TUrlRecordItemList;

//	virtual BOOL OpenDatabase( LPCTSTR strPath ) = 0;
	// �򿪡�
	virtual BOOL Open() = 0;

	// �رա� 
	virtual BOOL Close() = 0;

	/** ����ָ��״̬��ȡָ�������� url��¼��*/
	virtual BOOL GetUrlByStateByNum( EWebpageState eState, 
		TUrlRecordItemList& tRecordItemList, int nCount ) = 0;

	/** ����һ��url��¼�� */
	virtual BOOL SearchUrl( LPCTSTR strUrl, TUrlRecordItem& item ) = 0;

	/** ����һ����¼�� */
	virtual BOOL AddRecord( const TUrlRecordItem& item ) = 0;

	/** �޸�һ���Ѿ����ڵļ�¼�� */
	virtual BOOL ModifyRecord( const TUrlRecordItem& item ) = 0;

	/** ֻ�޸�һ����¼�е�״̬�� */
	virtual BOOL ModifyWebpageState( LPCTSTR strUrl, EWebpageState eStat ) = 0;

	/** ����һ��ʧ��url��¼. */
	virtual BOOL AddFailUrl( LPCTSTR strFailUrl, LPCTSTR strPageOfUrl ) = 0;

};
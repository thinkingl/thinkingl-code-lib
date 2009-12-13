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
	//	PageStateFeching,	// ���ڻ�ȡ��
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

	// ���һ�������¼��


};

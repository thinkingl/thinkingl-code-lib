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
		PageStateFeching,	// ���ڻ�ȡ��
	};

//	virtual BOOL OpenDatabase( LPCTSTR strPath ) = 0;
	// �򿪡�
	virtual BOOL Open() = 0;

	// �رա� 
	virtual BOOL Close() = 0;

	// ��ȡһ�������¼��

	// ���һ�������¼��


};

#pragma once
#include "supergridctrl.h"
#include "Process.h"

class CProcessTreeList :
	public CSuperGridCtrl
{
public:
	CProcessTreeList(void);
	virtual ~CProcessTreeList(void);

	/** ����һ�����̡� */
	CTreeItem *InsertItem( const CProcess& process ); 

	/** ˢ��һ�����̡� */

private:
	CImageList m_image;//Must provide an imagelist
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


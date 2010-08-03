#pragma once
#include "supergridctrl.h"
#include "Process.h"

class CProcessTreeList :
	public CSuperGridCtrl
{
public:
	CProcessTreeList(void);
	virtual ~CProcessTreeList(void);

	/** 插入一个进程。 */
	CTreeItem *InsertItem( const CProcess& process ); 

	/** 刷新一个进程。 */

private:
	CImageList m_image;//Must provide an imagelist
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


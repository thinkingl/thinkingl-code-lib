#pragma once

#include "ProcessTreeList.h"
#include "ProcessManage.h"

class CProcessToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// CProcessView

class CProcessView : public CDockablePane
{
	DECLARE_DYNAMIC(CProcessView)

public:
	CProcessView();
	virtual ~CProcessView();

	void AdjustLayout();
	void OnChangeVisualStyle();

private:
	/** 树列表控件。 */
	CProcessTreeList m_wndProcessTreeList;

	/** 进程管理 */
	CProcessManage m_processManage;

	// 重写
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);



	DECLARE_MESSAGE_MAP()


};



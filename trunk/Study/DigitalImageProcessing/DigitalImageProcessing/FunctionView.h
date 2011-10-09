
#pragma once

#include "dipconst.h"
#include "ViewTree.h"

#include "GrayscaleDialog.h"
#include "ImageNegativeDialog.h"
#include "IntensityLogTransformationDialog.h"
#include "IntensityPowerTransformationDialog.h"
#include "IntensityBitplaneSlicingDialog.h"
#include <map>


class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};


class CFunctionView : public CDockablePane
{
public:
	CFunctionView();
	virtual ~CFunctionView();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CClassToolBar m_wndToolBar;
	CViewTree m_wndFunctionView;
	CImageList m_ClassViewImages;
	UINT m_nCurrSort;

	void FillDIPFunctionTree();


// 重写
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:


	enum ECtrlId
	{
		ID_FunctionTree = 10,
	};

private:
	CGrayscaleDialog m_dlgRGB2GrayHSI;
	CGrayscaleDialog m_dlgRGB2GrayYUV;
	CGrayscaleDialog m_dlgIntensityLevel;

	CImageNegativeDialog m_dlgImageNegative;
	CIntensityLogTransformationDialog m_dlgIntensityLogTransformation;
	CIntensityPowerTransformationDialog m_dlgIntensityPowerTransformation;
	CIntensityBitplaneSlicingDialog m_dlgIntensityBitplaneSlicing;

	/** 功能与对话框的表. */
	typedef std::map< EDIPFunction, CWnd* > TFunctionDlgTable;
	TFunctionDlgTable m_functionDlgTable;

	/** 当前处理的功能. */
	EDIPFunction m_curFunction;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	afx_msg void OnFunctionTreeSelChange(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual HRESULT accSelect(long flagsSelect, VARIANT varChild);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL Create(LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect, BOOL bHasGripper, UINT nID, DWORD dwStyle, DWORD dwTabbedStyle = AFX_CBRS_REGULAR_TABS, DWORD dwControlBarStyle = AFX_DEFAULT_DOCKING_PANE_STYLE, CCreateContext* pContext = NULL);
};


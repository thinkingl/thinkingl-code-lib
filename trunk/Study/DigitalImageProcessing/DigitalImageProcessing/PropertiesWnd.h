
#pragma once



class CMFCPropertyGridDialogProperty;
class CHistogramDialog;

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CDigitalImage;

class CPropertiesWnd : public CDockablePane
{
// 构造
public:
	CPropertiesWnd();

	void AdjustLayout();

// 特性
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

	/** 更新当前显示的属性. */
	void UpdateProperties( CDigitalImage * pImg );


protected:
	CFont m_fntPropList;
//	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;

	CMFCPropertyGridProperty *m_pPropFilePath;
	CMFCPropertyGridProperty *m_pPropFileLength;
	/** 类型. rgb还是灰度 */
	CMFCPropertyGridProperty *m_pPropImageType;
	/** 灰度等级 1-8 */
	CMFCPropertyGridProperty *m_pPropImageIntensiveLevel;
	/** 每像素占用字节数. */
	CMFCPropertyGridProperty *m_pPropImageBPP;

	/** 宽度和高度. */
	CMFCPropertyGridProperty *m_pPropImageWidth;
	CMFCPropertyGridProperty *m_pPropImageHeight;

	CMFCPropertyGridDialogProperty *m_pPropHistogram;

	/** 直方图对话框. */
	CHistogramDialog *m_pHistogramDialog;;
// 实现
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
// 	afx_msg void OnProperties1();
// 	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
// 	afx_msg void OnProperties2();
// 	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};


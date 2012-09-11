
// SZMapDlg.h : 头文件
//

#pragma once

#include "MapCtrl.h"
#include "afxcmn.h"

#include "MapDownloadDialog.h"

// CSZMapDlg 对话框
class CSZMapDlg : public CDialogEx
{
// 构造
public:
	CSZMapDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SZMAP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	/** 排列控件。 */
	void Updatelayer( CRect *prcClient = NULL );
private:
	/** 地图控件。 */
	CMapCtrl m_mapCtrl;

	/** 地图下载窗口. */
	CMapDownloadDialog m_mapDownloadDlg;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CSliderCtrl m_sliderMapZlevel;
	afx_msg void OnTRBNThumbPosChangingSliderZLevle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderZLevle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonBackShanghai();
	afx_msg void OnNMThemeChangedSliderZLevle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonGoToKeda();
	double m_dbGoLatitude;
	double m_dbGoLongitude;
	afx_msg void OnBnClickedButtonGo();
	afx_msg void OnBnClickedButtonDownload();
	afx_msg void OnBnClickedButtonMarEarth();
	afx_msg void OnBnClickedButtonEarthMar();
};

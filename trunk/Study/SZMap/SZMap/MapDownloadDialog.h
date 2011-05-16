#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMapDownloadDialog 对话框

class CMapDownloadDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMapDownloadDialog)

public:
	CMapDownloadDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMapDownloadDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dbLongitudeLeftTop;
	double m_dbLatitudeLefttop;
	double m_dbLongitudeRightbottom;
	double m_dbLatitudeRightbottom;
	int m_nMinMapLevel;
	int m_nMaxLevel;
	BOOL m_bDownloadMap;
	BOOL m_bDownloadSatellite;
	CProgressCtrl m_progressDownload;
	CEdit m_editLog;
	CString m_strMapDownloadPath;
	CString m_strDownloadExePath;
	afx_msg void OnBnClickedButtonDownload();
};

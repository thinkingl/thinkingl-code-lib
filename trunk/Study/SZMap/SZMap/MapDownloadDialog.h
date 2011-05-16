#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMapDownloadDialog �Ի���

class CMapDownloadDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMapDownloadDialog)

public:
	CMapDownloadDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMapDownloadDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

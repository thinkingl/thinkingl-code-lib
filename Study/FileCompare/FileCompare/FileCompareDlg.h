
// FileCompareDlg.h : 头文件
//

#pragma once


// CFileCompareDlg 对话框
class CFileCompareDlg : public CDialogEx
{
// 构造
public:
	CFileCompareDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FILECOMPARE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	afx_msg void OnBnClickedButtonSelectSrc();
	CString m_strSrcFilePath;
	CString m_strDstFilePath;
	afx_msg void OnBnClickedButtonReadTest();
	afx_msg void OnBnClickedButtonCompare();
	afx_msg void OnBnClickedButtonMerge();
};

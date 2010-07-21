
// Win32ErrorCodeShowDlg.h : 头文件
//

#pragma once


// CWin32ErrorCodeShowDlg 对话框
class CWin32ErrorCodeShowDlg : public CDialogEx
{
// 构造
public:
	CWin32ErrorCodeShowDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WIN32ERRORCODESHOW_DIALOG };

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
	// 错误码。
	DWORD m_dwErrorCode;
	afx_msg void OnBnClickedBtnLookUp();
	CString m_strErrorMsg;
};


// KedaGiftDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CKedaGiftDlg 对话框
class CKedaGiftDlg : public CDialogEx
{
// 构造
public:
	CKedaGiftDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_KEDAGIFT_DIALOG };

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
	afx_msg void OnBnClickedStartStop();
	afx_msg LRESULT OnEmployRefresh( WPARAM, LPARAM );

private:
	static void CALLBACK CKedaGiftDlg::TimerCB(HWND hWnd, UINT eventId, UINT_PTR, DWORD);
private:
	// 定时器.
	enum ETimer
	{
		Timer_Refresh = 10,
	};

	/** 是否正在刷新员工. */
	bool m_bEmployRefreshing;
public:
	CStatic m_staticEmplyer;
	afx_msg void OnBnClickedUnittest();
};

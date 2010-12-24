
// VirtualTreeControlDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "VirtualTreeCtrl.h"

// CVirtualTreeControlDlg 对话框
class CVirtualTreeControlDlg : public CDialog
{
// 构造
public:
	CVirtualTreeControlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VIRTUALTREECONTROL_DIALOG };

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
	afx_msg void OnTvnSelchangedTreeTest(NMHDR *pNMHDR, LRESULT *pResult);
	CVirtualTreeCtrl m_treeTest;
	afx_msg void OnBnClickedTest();
};

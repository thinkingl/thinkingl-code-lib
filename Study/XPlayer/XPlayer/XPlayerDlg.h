
// XPlayerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include <dshow.h>

// CXPlayerDlg 对话框
class CXPlayerDlg : public CDialogEx
{
// 构造
public:
	CXPlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_XPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	IGraphBuilder *m_pGraph;
	IMediaControl *m_pControl;
	IMediaEvent* m_pEvent;

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
	afx_msg void OnBnClickedPlay();
	CEdit m_eidtLog;
	afx_msg void OnBnClickedShowlog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStop();
};


// KedaGiftDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "RandomPick.h"

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

	/** 显示一个人. */
	void ShowAMan( const CEmployer& oneMan );

	/** 获取显示的字符.*/
	CString GetShowText( const CEmployer& oneMan );
private:
	// 定时器.
	enum ETimer
	{
		Timer_Refresh = 10,
	};

	/** 是否正在刷新员工. */
	bool m_bEmployRefreshing;

	/** 幸运摇奖. */
	CRandomPick m_radomLuckyPick;

	CFont m_fontShowName;
public:
	CStatic m_staticEmplyer;
	afx_msg void OnBnClickedUnittest();
	CButton m_btnStartStop;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedGiftState();

	CEdit m_editGifted;
	CString m_strGifted;
	afx_msg void OnBnClickedNextTurn();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

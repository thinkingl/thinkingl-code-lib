#pragma once
#include "shockwaveflash_background.h"
#include "afxwin.h"

#include "PictureStatic.h"
#include "RandomPick.h"
// CFlashDialog 对话框

class CFlashDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFlashDialog)

public:
	CFlashDialog(CWnd* pParent, CRandomPick* pRandomPic );   // 标准构造函数
	virtual ~CFlashDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FLASH_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	/** 获取本轮所有中奖的人. */
	TKedaNoList GetAllLuckyMen()const
	{
		return m_tLuckyMen;
	}
private:
	// 定时器.
	enum ETimer
	{
		Timer_Refresh = 1111,
	};

	// 当前状态.
	enum EState
	{
		State_Ready,	// 等待开始抽奖.
		State_Running,	// 正在变化数字.
	};

	/** 显示... */
	void ShowAEmployer( const CEmployer& employer );

	/** 抽奖.. */
	void GoOrStop();

	static void CALLBACK TimerCB(HWND hWnd, UINT , UINT_PTR timerId, DWORD de);
	afx_msg LRESULT OnEmployRefresh( WPARAM, LPARAM );

	/** 更新本轮中奖人数. */
	void UpdateLuckNum();
private:

	CRandomPick * m_pRandomPick;
	EState m_eState;

	typedef std::vector< CPictureStatic * > TFlashCtrlList;
//	typedef std::vector< CShockwaveflash_background * > TFlashCtrlList;
	TFlashCtrlList m_tAllNumberFlashControls;

	CFont m_fontShowName;
	
	/** 中奖人列表. */
	TKedaNoList m_tLuckyMen;
public:
//	CShockwaveflash_background m_flashBackground;
	virtual BOOL OnInitDialog();
//	CShockwaveflash_background m_flashNumberFirst;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedTest();
	CButton m_btnTest;
	DECLARE_EVENTSINK_MAP()
	void FlashCallShockwaveflashBackground(LPCTSTR request);
	void FSCommandShockwaveflashBackground(LPCTSTR command, LPCTSTR args);
	void OnProgressShockwaveflashBackground(long percentDone);
	void OnReadyStateChangeShockwaveflashBackground(long newState);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CPictureStatic m_picK;
// 	CShockwaveflash_background m_flashNum2;
// 	CShockwaveflash_background m_flashNum3;
// 	CShockwaveflash_background m_flashNum4;
	CPictureStatic m_picD;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CPictureStatic m_picNum1;
	CPictureStatic m_picNum2;
	CPictureStatic m_picNum3;
	CPictureStatic m_picNum4;
	CPictureStatic m_picNum5;
	CPictureStatic m_picNum6;
	CStatic m_staticName;
	CStatic m_staticDepartment;	// 部门。

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	CButton m_btnClose;
	CButton m_btnAbsent;
	afx_msg void OnBnClickedAbsent();
	CStatic m_staticLuckyNum;
	afx_msg void OnDestroy();
};

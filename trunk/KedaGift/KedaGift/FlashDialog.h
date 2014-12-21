#pragma once
#include "shockwaveflash_background.h"
#include "afxwin.h"

#include "PictureStatic.h"
#include "RandomPick.h"
// CFlashDialog �Ի���

class CFlashDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFlashDialog)

public:
	CFlashDialog(CWnd* pParent, CRandomPick* pRandomPic );   // ��׼���캯��
	virtual ~CFlashDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FLASH_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	/** ��ȡ���������н�����. */
	TKedaNoList GetAllLuckyMen()const
	{
		return m_tLuckyMen;
	}
private:
	// ��ʱ��.
	enum ETimer
	{
		Timer_Refresh = 1111,
	};

	// ��ǰ״̬.
	enum EState
	{
		State_Ready,	// �ȴ���ʼ�齱.
		State_Running,	// ���ڱ仯����.
	};

	/** ��ʾ... */
	void ShowAEmployer( const CEmployer& employer );

	/** �齱.. */
	void GoOrStop();

	static void CALLBACK TimerCB(HWND hWnd, UINT , UINT_PTR timerId, DWORD de);
	afx_msg LRESULT OnEmployRefresh( WPARAM, LPARAM );

	/** ���±����н�����. */
	void UpdateLuckNum();
private:

	CRandomPick * m_pRandomPick;
	EState m_eState;

	typedef std::vector< CPictureStatic * > TFlashCtrlList;
//	typedef std::vector< CShockwaveflash_background * > TFlashCtrlList;
	TFlashCtrlList m_tAllNumberFlashControls;

	CFont m_fontShowName;
	
	/** �н����б�. */
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
	CStatic m_staticDepartment;	// ���š�

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	CButton m_btnClose;
	CButton m_btnAbsent;
	afx_msg void OnBnClickedAbsent();
	CStatic m_staticLuckyNum;
	afx_msg void OnDestroy();
};

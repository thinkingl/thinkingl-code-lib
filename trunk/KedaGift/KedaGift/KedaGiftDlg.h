
// KedaGiftDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "RandomPick.h"

// CKedaGiftDlg �Ի���
class CKedaGiftDlg : public CDialogEx
{
// ����
public:
	CKedaGiftDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_KEDAGIFT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	/** ��ʾһ����. */
	void ShowAMan( const CEmployer& oneMan );

	/** ��ȡ��ʾ���ַ�.*/
	CString GetShowText( const CEmployer& oneMan );
private:
	// ��ʱ��.
	enum ETimer
	{
		Timer_Refresh = 10,
	};

	/** �Ƿ�����ˢ��Ա��. */
	bool m_bEmployRefreshing;

	/** ����ҡ��. */
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

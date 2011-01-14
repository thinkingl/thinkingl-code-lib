
// KedaGiftDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


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
private:
	// ��ʱ��.
	enum ETimer
	{
		Timer_Refresh = 10,
	};

	/** �Ƿ�����ˢ��Ա��. */
	bool m_bEmployRefreshing;
public:
	CStatic m_staticEmplyer;
	afx_msg void OnBnClickedUnittest();
};

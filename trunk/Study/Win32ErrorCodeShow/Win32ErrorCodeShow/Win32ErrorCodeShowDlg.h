
// Win32ErrorCodeShowDlg.h : ͷ�ļ�
//

#pragma once


// CWin32ErrorCodeShowDlg �Ի���
class CWin32ErrorCodeShowDlg : public CDialogEx
{
// ����
public:
	CWin32ErrorCodeShowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WIN32ERRORCODESHOW_DIALOG };

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
	// �����롣
	DWORD m_dwErrorCode;
	afx_msg void OnBnClickedBtnLookUp();
	CString m_strErrorMsg;
};

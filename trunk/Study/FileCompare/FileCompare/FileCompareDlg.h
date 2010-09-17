
// FileCompareDlg.h : ͷ�ļ�
//

#pragma once


// CFileCompareDlg �Ի���
class CFileCompareDlg : public CDialogEx
{
// ����
public:
	CFileCompareDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FILECOMPARE_DIALOG };

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
	afx_msg void OnBnClickedButtonSelectSrc();
	CString m_strSrcFilePath;
	CString m_strDstFilePath;
	afx_msg void OnBnClickedButtonReadTest();
	afx_msg void OnBnClickedButtonCompare();
	afx_msg void OnBnClickedButtonMerge();
};

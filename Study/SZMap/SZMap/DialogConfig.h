#pragma once


// CDialogConfig �Ի���

class CDialogConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogConfig)

public:
	CDialogConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogConfig();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strImgDir;
	afx_msg void OnBnClickedButtonExplorer();
	afx_msg void OnBnClickedOk();
};
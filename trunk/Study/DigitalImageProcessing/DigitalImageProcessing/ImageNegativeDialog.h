#pragma once


// CImageNegativeDialog �Ի���

class CImageNegativeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CImageNegativeDialog)

public:
	CImageNegativeDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImageNegativeDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_INTENSITY_NEGITIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
};

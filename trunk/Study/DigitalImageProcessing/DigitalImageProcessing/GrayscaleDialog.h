#pragma once


// CGrayscaleDialog �Ի���
#include "resource.h"

class CGrayscaleDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CGrayscaleDialog)

public:
	CGrayscaleDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGrayscaleDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_GRAYSCALE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
};

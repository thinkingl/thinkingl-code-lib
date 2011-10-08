#pragma once


// CImageNegativeDialog 对话框

class CImageNegativeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CImageNegativeDialog)

public:
	CImageNegativeDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageNegativeDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_INTENSITY_NEGITIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
};

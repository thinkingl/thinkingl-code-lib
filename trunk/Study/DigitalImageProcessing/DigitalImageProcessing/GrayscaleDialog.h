#pragma once


// CGrayscaleDialog 对话框
#include "resource.h"

class CGrayscaleDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CGrayscaleDialog)

public:
	CGrayscaleDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGrayscaleDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_GRAYSCALE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
};

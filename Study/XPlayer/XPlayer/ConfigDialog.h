#pragma once


// CConfigDialog 对话框

class CConfigDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDialog)

public:
	CConfigDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfigDialog();

// 对话框数据
	enum { IDD = IDD_CONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

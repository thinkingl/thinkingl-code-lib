#pragma once


// CIntensityLogTransformationDialog 对话框

class CIntensityLogTransformationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CIntensityLogTransformationDialog)

public:
	CIntensityLogTransformationDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIntensityLogTransformationDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_INTENSITY_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
	float m_logTransformationConstant;
};

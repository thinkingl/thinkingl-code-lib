#pragma once


// CIntensityPowerTransformationDialog 对话框

class CIntensityPowerTransformationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CIntensityPowerTransformationDialog)

public:
	CIntensityPowerTransformationDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIntensityPowerTransformationDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_INTENSITY_POWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
	double m_const;
	double m_v;
};

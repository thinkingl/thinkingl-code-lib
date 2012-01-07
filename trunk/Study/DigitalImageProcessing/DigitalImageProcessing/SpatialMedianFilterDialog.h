#pragma once

#include "BaseDIPFunctionDialog.h"
// CSpatialMedianFilterDialog 对话框

class CSpatialMedianFilterDialog : public CBaseDIPFunctionDialog
{
	DECLARE_DYNAMIC(CSpatialMedianFilterDialog)

public:
	CSpatialMedianFilterDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpatialMedianFilterDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_NOISE_SPATIAL_MEDIAN_FILTER };
protected:
	virtual void OnDIP( CDigitalImage *pImg );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_maskSize;
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
};

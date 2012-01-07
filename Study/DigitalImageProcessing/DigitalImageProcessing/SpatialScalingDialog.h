#pragma once


// CSpatialScalingDialog 对话框
#include "BaseDIPFunctionDialog.h"

class CSpatialScalingDialog : public CBaseDIPFunctionDialog
{
	DECLARE_DYNAMIC(CSpatialScalingDialog)

public:
	CSpatialScalingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpatialScalingDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_SPATIAL_SCALING };

protected:
// 	virtual void OnPriview( CDigitalImage* pImg );
// 	virtual void OnApply( CDigitalImage *pImg );
	virtual void OnDIP( CDigitalImage *pImg );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_scaleX;
	double m_scaleY;
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
	int m_interpolateType;
};

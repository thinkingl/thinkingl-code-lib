#pragma once

#include "BaseDIPFunctionDialog.h"
// CFrequencyFourierTransformDialog 对话框

class CFrequencyFourierTransformDialog : public CBaseDIPFunctionDialog
{
	DECLARE_DYNAMIC(CFrequencyFourierTransformDialog)

public:
	CFrequencyFourierTransformDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFrequencyFourierTransformDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_FREQUENCY_FOURIER_TRANSFORM };
protected:
	virtual void OnDIP( CDigitalImage *pImg );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
private:
	int m_fourierTransformOrInverseFourierTransform;
public:
	BOOL m_centerTheTransform;
};

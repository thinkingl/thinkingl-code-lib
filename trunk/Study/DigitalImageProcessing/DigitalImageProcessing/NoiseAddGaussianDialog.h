#pragma once

#include "BaseDIPFunctionDialog.h"
// CNoiseAddGaussianDialog �Ի���

class CNoiseAddGaussianDialog : public CBaseDIPFunctionDialog
{
	DECLARE_DYNAMIC(CNoiseAddGaussianDialog)

public:
	CNoiseAddGaussianDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNoiseAddGaussianDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_NOISE_ADD_GAUSSIAN };

protected:
	virtual void OnDIP( CDigitalImage *pImg );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
	double m_gaussianMean;
	double m_gaussionVariance;
};

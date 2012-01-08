#pragma once

#include "BaseDIPFunctionDialog.h"
// CFrequencyFourierTransformDialog �Ի���

class CFrequencyFourierTransformDialog : public CBaseDIPFunctionDialog
{
	DECLARE_DYNAMIC(CFrequencyFourierTransformDialog)

public:
	CFrequencyFourierTransformDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFrequencyFourierTransformDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_FREQUENCY_FOURIER_TRANSFORM };
protected:
	virtual void OnDIP( CDigitalImage *pImg );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

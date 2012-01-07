#pragma once

#include "BaseDIPFunctionDialog.h"
// CSpatialMedianFilterDialog �Ի���

class CSpatialMedianFilterDialog : public CBaseDIPFunctionDialog
{
	DECLARE_DYNAMIC(CSpatialMedianFilterDialog)

public:
	CSpatialMedianFilterDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSpatialMedianFilterDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_NOISE_SPATIAL_MEDIAN_FILTER };
protected:
	virtual void OnDIP( CDigitalImage *pImg );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_maskSize;
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
};

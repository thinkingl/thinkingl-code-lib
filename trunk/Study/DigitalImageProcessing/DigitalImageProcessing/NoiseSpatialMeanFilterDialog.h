#pragma once


// CNoiseSpatialMeanFilterDialog �Ի���
#include "BaseDIPFunctionDialog.h"
class CNoiseSpatialMeanFilterDialog : public CBaseDIPFunctionDialog
{
	DECLARE_DYNAMIC(CNoiseSpatialMeanFilterDialog)

public:
	CNoiseSpatialMeanFilterDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNoiseSpatialMeanFilterDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_NOISE_SPATIAL_MEAN_FILTER };
protected:
	virtual void OnDIP( CDigitalImage *pImg );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
	int m_maskSize;
};

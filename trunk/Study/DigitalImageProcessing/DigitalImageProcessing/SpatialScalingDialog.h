#pragma once


// CSpatialScalingDialog �Ի���
#include "BaseDIPFunctionDialog.h"

class CSpatialScalingDialog : public CBaseDIPFunctionDialog
{
	DECLARE_DYNAMIC(CSpatialScalingDialog)

public:
	CSpatialScalingDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSpatialScalingDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_SPATIAL_SCALING };

protected:
// 	virtual void OnPriview( CDigitalImage* pImg );
// 	virtual void OnApply( CDigitalImage *pImg );
	virtual void OnDIP( CDigitalImage *pImg );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_scaleX;
	double m_scaleY;
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
	int m_interpolateType;
};

#pragma once

#include "BaseDIPFunctionDialog.h"
// CEdgeDetectionDialog �Ի���

enum EEdgeDetectFunction
{
	Roberts,
	Sobel,
	Laplacian,
	Prewitt,
};

class CEdgeDetectionDialog : public CBaseDIPFunctionDialog
{
	DECLARE_DYNAMIC(CEdgeDetectionDialog)

public:
	CEdgeDetectionDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEdgeDetectionDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_EDGE_DETECT };
protected:
	virtual void OnDIP( CDigitalImage *pImg );

	EEdgeDetectFunction m_edgeDetectType;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApplySobel();
	afx_msg void OnBnClickedApplyRoberts();
	afx_msg void OnBnClickedApplyPrewitt();
};

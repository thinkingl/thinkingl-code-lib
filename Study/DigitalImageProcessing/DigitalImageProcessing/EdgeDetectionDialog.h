#pragma once

#include "BaseDIPFunctionDialog.h"
// CEdgeDetectionDialog 对话框

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
	CEdgeDetectionDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEdgeDetectionDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_EDGE_DETECT };
protected:
	virtual void OnDIP( CDigitalImage *pImg );

	EEdgeDetectFunction m_edgeDetectType;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApplySobel();
	afx_msg void OnBnClickedApplyRoberts();
	afx_msg void OnBnClickedApplyPrewitt();
};

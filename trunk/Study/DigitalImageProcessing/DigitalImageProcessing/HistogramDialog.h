#pragma once
#include "afxwin.h"

#include "HistogramCtrl.h"
// CHistogramDialog 对话框

class CHistogramDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDialog)

public:
	CHistogramDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistogramDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_HISTOGRAM };

public:
	/** 设置直方图数据. */
	bool SetHistoramData( const CDigitalImage::THistogramData& data );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CHistogramCtrl m_histogramCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
};

#pragma once
#include "afxwin.h"

#include "HistogramCtrl.h"

// CIntensityHistogramEqualizationDialog 对话框
class CDigitalImage;

class CIntensityHistogramEqualizationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CIntensityHistogramEqualizationDialog)

public:
	CIntensityHistogramEqualizationDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIntensityHistogramEqualizationDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_HISTOGRAM_EQUALIZATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CDigitalImage *m_pImage;
public:
	CHistogramCtrl m_histogramCtrl;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedApply();
};

#pragma once
#include "afxwin.h"

#include "HistogramCtrl.h"

// CHistogramEqualizationDialog 对话框
class CDigitalImage;

class CHistogramEqualizationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramEqualizationDialog)

public:
	CHistogramEqualizationDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistogramEqualizationDialog();

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

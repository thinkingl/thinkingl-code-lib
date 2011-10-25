#pragma once
#include "afxwin.h"

#include "HistogramCtrl.h"

// CHistogramEqualizationDialog �Ի���
class CDigitalImage;

class CHistogramEqualizationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramEqualizationDialog)

public:
	CHistogramEqualizationDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistogramEqualizationDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_HISTOGRAM_EQUALIZATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CDigitalImage *m_pImage;
public:
	CHistogramCtrl m_histogramCtrl;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedApply();
};

#pragma once
#include "afxwin.h"

#include "HistogramCtrl.h"

// CIntensityHistogramEqualizationDialog �Ի���
class CDigitalImage;

class CIntensityHistogramEqualizationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CIntensityHistogramEqualizationDialog)

public:
	CIntensityHistogramEqualizationDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIntensityHistogramEqualizationDialog();

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

#pragma once
#include "afxwin.h"

#include "HistogramCtrl.h"
// CHistogramDialog �Ի���

class CHistogramDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDialog)

public:
	CHistogramDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistogramDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_HISTOGRAM };

public:
	/** ����ֱ��ͼ����. */
	bool SetHistoramData( const CDigitalImage::THistogramData& data );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CHistogramCtrl m_histogramCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
};

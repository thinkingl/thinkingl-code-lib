#pragma once
#include "afxwin.h"

#include "HistogramCtrl.h"
// CUnitTestDialog �Ի���

class CUnitTestDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitTestDialog)

public:
	CUnitTestDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUnitTestDialog();

// �Ի�������
	enum { IDD = IDD_UNITTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CHistogramCtrl m_histogramCtrl;
	virtual BOOL OnInitDialog();
};

#pragma once
#include "afxwin.h"

#include "HistogramCtrl.h"
// CUnitTestDialog 对话框

class CUnitTestDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitTestDialog)

public:
	CUnitTestDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUnitTestDialog();

// 对话框数据
	enum { IDD = IDD_UNITTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CHistogramCtrl m_histogramCtrl;
	virtual BOOL OnInitDialog();
};

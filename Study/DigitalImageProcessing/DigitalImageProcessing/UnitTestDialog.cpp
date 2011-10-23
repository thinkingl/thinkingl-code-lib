// UnitTestDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "UnitTestDialog.h"
#include "afxdialogex.h"


// CUnitTestDialog 对话框

IMPLEMENT_DYNAMIC(CUnitTestDialog, CDialogEx)

CUnitTestDialog::CUnitTestDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUnitTestDialog::IDD, pParent)
{

}

CUnitTestDialog::~CUnitTestDialog()
{
}

void CUnitTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_HISTOGRAM, m_histogramCtrl);
}


BEGIN_MESSAGE_MAP(CUnitTestDialog, CDialogEx)
END_MESSAGE_MAP()


// CUnitTestDialog 消息处理程序


BOOL CUnitTestDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDigitalImage::THistogramData hisData;
	hisData.push_back( 128 );
	hisData.push_back( 64 );

	this->m_histogramCtrl.SetHistoramData( hisData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// HistogramDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "HistogramDialog.h"
#include "afxdialogex.h"


// CHistogramDialog 对话框

IMPLEMENT_DYNAMIC(CHistogramDialog, CDialogEx)

CHistogramDialog::CHistogramDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistogramDialog::IDD, pParent)
{

}

CHistogramDialog::~CHistogramDialog()
{
}

void CHistogramDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_HISTOGRAM, m_histogramCtrl);
}


BEGIN_MESSAGE_MAP(CHistogramDialog, CDialogEx)
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// CHistogramDialog 消息处理程序


void CHistogramDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if ( this->GetSafeHwnd() && this->m_histogramCtrl.GetSafeHwnd() )
	{
		CRect rcClient;
		this->GetClientRect( rcClient );
		rcClient.DeflateRect( 1, 1 );
		this->m_histogramCtrl.MoveWindow( rcClient );
	}
}

bool CHistogramDialog::SetHistoramData( const CDigitalImage::THistogramData& data )
{
	return this->m_histogramCtrl.SetHistoramData( data );
}

BOOL CHistogramDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rcClient;
	this->GetClientRect( rcClient );
	rcClient.DeflateRect( 1, 1 );
	this->m_histogramCtrl.MoveWindow( rcClient );
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CHistogramDialog::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();

	// TODO: 在此处添加消息处理程序代码
 
}

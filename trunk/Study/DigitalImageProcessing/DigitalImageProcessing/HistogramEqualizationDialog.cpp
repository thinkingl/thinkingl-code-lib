// HistogramEqualizationDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "HistogramEqualizationDialog.h"
#include "afxdialogex.h"

#include "dipcommon.h"
#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"
// CHistogramEqualizationDialog 对话框

IMPLEMENT_DYNAMIC(CHistogramEqualizationDialog, CDialogEx)

CHistogramEqualizationDialog::CHistogramEqualizationDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistogramEqualizationDialog::IDD, pParent)
{
	this->m_pImage = 0;
}

CHistogramEqualizationDialog::~CHistogramEqualizationDialog()
{
}

void CHistogramEqualizationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HISTOGRAM, m_histogramCtrl);
}


BEGIN_MESSAGE_MAP(CHistogramEqualizationDialog, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_APPLY, &CHistogramEqualizationDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CHistogramEqualizationDialog 消息处理程序


void CHistogramEqualizationDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if ( bShow )
	{
		CDigitalImageProcessingView *pCurView = GetActiveDIPView();
		if ( pCurView )
		{
			if ( 0 == m_pImage )
			{
				CDigitalImage *pDI = pCurView->StartDIP();
				m_pImage = pDI;
			}

			CDigitalImage::THistogramData grayHistogram, rh, gh, bh;
			this->m_pImage->GetHistogramData( grayHistogram, rh, gh, bh );
			this->m_histogramCtrl.SetHistoramData( grayHistogram );
		}	
	}
}


void CHistogramEqualizationDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		if ( 0 == m_pImage )
		{
			CDigitalImage *pDI = pCurView->StartDIP();
			m_pImage = pDI;
		}

		m_pImage->HistogramEqualization();

		CDigitalImage::THistogramData grayHistogram, rh, gh, bh;
		this->m_pImage->GetHistogramData( grayHistogram, rh, gh, bh );
		this->m_histogramCtrl.SetHistoramData( grayHistogram );

		pCurView->EndDIP( m_pImage );
		m_pImage = NULL;
	}	
}

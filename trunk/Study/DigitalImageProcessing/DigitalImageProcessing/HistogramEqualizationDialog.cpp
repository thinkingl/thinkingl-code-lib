// HistogramEqualizationDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "HistogramEqualizationDialog.h"
#include "afxdialogex.h"

#include "dipcommon.h"
#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"
// CHistogramEqualizationDialog �Ի���

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


// CHistogramEqualizationDialog ��Ϣ�������


void CHistogramEqualizationDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

// HistogramEqualizationDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "IntensityHistogramEqualizationDialog.h"
#include "afxdialogex.h"

#include "dipcommon.h"
#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"
// CIntensityHistogramEqualizationDialog �Ի���

IMPLEMENT_DYNAMIC(CIntensityHistogramEqualizationDialog, CDialogEx)

CIntensityHistogramEqualizationDialog::CIntensityHistogramEqualizationDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIntensityHistogramEqualizationDialog::IDD, pParent)
{
	this->m_pImage = 0;
}

CIntensityHistogramEqualizationDialog::~CIntensityHistogramEqualizationDialog()
{
}

void CIntensityHistogramEqualizationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HISTOGRAM, m_histogramCtrl);
}


BEGIN_MESSAGE_MAP(CIntensityHistogramEqualizationDialog, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_APPLY, &CIntensityHistogramEqualizationDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CIntensityHistogramEqualizationDialog ��Ϣ�������


void CIntensityHistogramEqualizationDialog::OnShowWindow(BOOL bShow, UINT nStatus)
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


void CIntensityHistogramEqualizationDialog::OnBnClickedApply()
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

		if ( CDigitalImage::DIT_RGB == m_pImage->GetImageType() )
		{
			this->MessageBox( _T( "�Բ���, ���ڵ����ֱ��ͼƽ���㷨�Բ�ɫͼ��Ч���ܲ�." ) );
		}

		m_pImage->HistogramEqualization();

		CDigitalImage::THistogramData grayHistogram, rh, gh, bh;
		this->m_pImage->GetHistogramData( grayHistogram, rh, gh, bh );
		this->m_histogramCtrl.SetHistoramData( grayHistogram );

		pCurView->EndDIP( m_pImage );
		m_pImage = NULL;
	}	
}

// GrayscaleDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "GrayscaleDialog.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "dipcommon.h"
#include "DigitalImageProcessingView.h"
#include "DigitalImage.h"

// CGrayscaleDialog �Ի���

IMPLEMENT_DYNAMIC(CGrayscaleDialog, CDialogEx)

CGrayscaleDialog::CGrayscaleDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGrayscaleDialog::IDD, pParent)
	, m_intensityLevel(8)
{
	this->m_dipFunction = DIP_RGB2Gray_HSI;
}

CGrayscaleDialog::~CGrayscaleDialog()
{
}

void CGrayscaleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INTENSITY_LEVEL, m_intensityLevel);
	DDV_MinMaxInt(pDX, m_intensityLevel, 1, 8);
	DDX_Control(pDX, IDC_SLIDER_INTENSITY_LEVEL, m_sliderIntensityLevel);
}


BEGIN_MESSAGE_MAP(CGrayscaleDialog, CDialogEx)
	ON_BN_CLICKED(ID_APPLY, &CGrayscaleDialog::OnBnClickedApply)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_INTENSITY_LEVEL, &CGrayscaleDialog::OnTRBNThumbPosChangingSliderIntensityLevel)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CGrayscaleDialog ��Ϣ�������




void CGrayscaleDialog::OnBnClickedApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData( TRUE );
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		CDigitalImage *pDI = pCurView->StartDIP();
		switch( this->m_dipFunction )
		{
		case DIP_RGB2Gray_HSI:
			pDI->ToGrayscaleImageHSI( this->m_intensityLevel );
			break;
		case DIP_RGB2Gray_YUV:
			pDI->ToGrayscaleImageYUV( this->m_intensityLevel );
		    break;
		case DIP_IntensityLevels:
			pDI->SetIntensityLevel( m_intensityLevel );
			break;
		default:
			ASSERT( FALSE );
		    break;
		}
		
		pCurView->EndDIP();
	}	
}


void CGrayscaleDialog::OnTRBNThumbPosChangingSliderIntensityLevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �˹���Ҫ�� Windows Vista ����߰汾��
	// _WIN32_WINNT ���ű��� >= 0x0600��
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CGrayscaleDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CGrayscaleDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

	// Contains the current scroll-box position if the scroll-bar code is SB_THUMBPOSITION or SB_THUMBTRACK; otherwise not used. 
	if ( SB_THUMBPOSITION == nSBCode || SB_THUMBTRACK == nSBCode )
	{
		this->m_intensityLevel = nPos;
	}
	else
	{
		m_intensityLevel = m_sliderIntensityLevel.GetPos();
	}	
	this->UpdateData( FALSE );
}

BOOL CGrayscaleDialog::OnInitDialog()
{
	BOOL bRet = __super::OnInitDialog();

	this->m_sliderIntensityLevel.SetRange( 1, 8 );
	this->m_sliderIntensityLevel.SetPos( m_intensityLevel );
	return bRet;
}

void CGrayscaleDialog::SetDIPFunction( EDIPFunction fun )
{
	ASSERT( DIP_IntensityLevels == fun || DIP_RGB2Gray_HSI == fun || DIP_RGB2Gray_YUV == fun );
	this->m_dipFunction = fun;
}

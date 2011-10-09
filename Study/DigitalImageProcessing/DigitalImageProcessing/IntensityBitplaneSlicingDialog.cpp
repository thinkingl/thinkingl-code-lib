// IntensityBitplaneSlicingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "IntensityBitplaneSlicingDialog.h"
#include "afxdialogex.h"

#include "dipcommon.h"
#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"
// CIntensityBitplaneSlicingDialog 对话框

IMPLEMENT_DYNAMIC(CIntensityBitplaneSlicingDialog, CDialogEx)

CIntensityBitplaneSlicingDialog::CIntensityBitplaneSlicingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIntensityBitplaneSlicingDialog::IDD, pParent)
	, m_bit1(TRUE)
	, m_bit2(TRUE)
	, m_bit3(TRUE)
	, m_bit4(TRUE)
	, m_bit5(TRUE)
	, m_bit6(TRUE)
	, m_bit7(TRUE)
	, m_bit8(TRUE)
{
	m_pImage = NULL;
}

CIntensityBitplaneSlicingDialog::~CIntensityBitplaneSlicingDialog()
{
}

void CIntensityBitplaneSlicingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_BIT1, m_bit1);
	DDX_Check(pDX, IDC_CHECK_BIT2, m_bit2);
	DDX_Check(pDX, IDC_CHECK_BIT3, m_bit3);
	DDX_Check(pDX, IDC_CHECK_BIT4, m_bit4);
	DDX_Check(pDX, IDC_CHECK_BIT5, m_bit5);
	DDX_Check(pDX, IDC_CHECK_BIT6, m_bit6);
	DDX_Check(pDX, IDC_CHECK_BIT7, m_bit7);
	DDX_Check(pDX, IDC_CHECK_BIT8, m_bit8);
}


BEGIN_MESSAGE_MAP(CIntensityBitplaneSlicingDialog, CDialogEx)
	ON_BN_CLICKED(ID_PREVIEW, &CIntensityBitplaneSlicingDialog::OnBnClickedPreview)
	ON_BN_CLICKED(ID_APPLY, &CIntensityBitplaneSlicingDialog::OnBnClickedApply)
	ON_BN_CLICKED(ID_CANCEL, &CIntensityBitplaneSlicingDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CIntensityBitplaneSlicingDialog 消息处理程序


void CIntensityBitplaneSlicingDialog::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	uint8 bitsMask = 0xFF;
	int offset = 0;
	bitsMask &= ( ~( !m_bit1 << offset++ ) );
	bitsMask &= ( ~( !m_bit2 << offset++ ) );
	bitsMask &= ( ~( !m_bit3 << offset++ ) );
	bitsMask &= ( ~( !m_bit4 << offset++ ) );
	bitsMask &= ( ~( !m_bit5 << offset++ ) );
	bitsMask &= ( ~( !m_bit6 << offset++ ) );
	bitsMask &= ( ~( !m_bit7 << offset++ ) );
	bitsMask &= ( ~( !m_bit8 << offset++ ) );

	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		// 重新获取原图片.
		this->ReleaseImage();
		m_pImage = pCurView->StartDIP();

		this->m_pImage->IntensityBitPlanesReconstruct( bitsMask );

		pCurView->DIPPreview( m_pImage );
		
	}	
}


void CIntensityBitplaneSlicingDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	uint8 bitsMask = 0xFF;
	int offset = 0;
	bitsMask &= ( ~( !m_bit1 << offset++ ) );
	bitsMask &= ( ~( !m_bit2 << offset++ ) );
	bitsMask &= ( ~( !m_bit3 << offset++ ) );
	bitsMask &= ( ~( !m_bit4 << offset++ ) );
	bitsMask &= ( ~( !m_bit5 << offset++ ) );
	bitsMask &= ( ~( !m_bit6 << offset++ ) );
	bitsMask &= ( ~( !m_bit7 << offset++ ) );
	bitsMask &= ( ~( !m_bit8 << offset++ ) );

	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		if ( 0 == m_pImage )
		{
			CDigitalImage *pDI = pCurView->StartDIP();
			m_pImage = pDI;
		}

		this->m_pImage->IntensityBitPlanesReconstruct( bitsMask );

		pCurView->EndDIP( m_pImage );
	}	
}


void CIntensityBitplaneSlicingDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		pCurView->CancelPreview();
	}	
	this->ReleaseImage();
}

void CIntensityBitplaneSlicingDialog::ReleaseImage()
{
	if ( m_pImage )
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}
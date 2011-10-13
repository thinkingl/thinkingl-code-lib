// IntensityBitplaneSliceShowDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "IntensityBitplaneSliceShowDialog.h"
#include "afxdialogex.h"

#include "dipcommon.h"
#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"
// CIntensityBitplaneSliceShowDialog 对话框

IMPLEMENT_DYNAMIC(CIntensityBitplaneSliceShowDialog, CDialogEx)

CIntensityBitplaneSliceShowDialog::CIntensityBitplaneSliceShowDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIntensityBitplaneSliceShowDialog::IDD, pParent)
{
	m_pImage = 0;
}

CIntensityBitplaneSliceShowDialog::~CIntensityBitplaneSliceShowDialog()
{
	this->Release();
}

void CIntensityBitplaneSliceShowDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIntensityBitplaneSliceShowDialog, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_BIT_1, &CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit1)
	ON_BN_CLICKED(IDC_RADIO_BIT_2, &CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit2)
	ON_BN_CLICKED(IDC_RADIO_BIT_3, &CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit3)
	ON_BN_CLICKED(IDC_RADIO_BIT_4, &CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit4)
	ON_BN_CLICKED(IDC_RADIO_BIT_5, &CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit5)
	ON_BN_CLICKED(IDC_RADIO_BIT_6, &CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit6)
	ON_BN_CLICKED(IDC_RADIO_BIT_7, &CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit7)
	ON_BN_CLICKED(IDC_RADIO_BIT_8, &CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit8)
	ON_BN_CLICKED(ID_APPLY, &CIntensityBitplaneSliceShowDialog::OnBnClickedApply)
	ON_BN_CLICKED(ID_CANCEL, &CIntensityBitplaneSliceShowDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CIntensityBitplaneSliceShowDialog 消息处理程序


void CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit1()
{
	// TODO: 在此添加控件通知处理程序代码
	this->TryShowOneBit( 0 );
}


void CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit2()
{
	// TODO: 在此添加控件通知处理程序代码
	this->TryShowOneBit( 1 );
}


void CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit3()
{
	// TODO: 在此添加控件通知处理程序代码
	this->TryShowOneBit( 2 );
}


void CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit4()
{
	// TODO: 在此添加控件通知处理程序代码
	this->TryShowOneBit( 3 );
}


void CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit5()
{
	// TODO: 在此添加控件通知处理程序代码
	this->TryShowOneBit( 4 );
}


void CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit6()
{
	// TODO: 在此添加控件通知处理程序代码
	this->TryShowOneBit( 5 );
}


void CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit7()
{
	// TODO: 在此添加控件通知处理程序代码
	this->TryShowOneBit( 6 );
}


void CIntensityBitplaneSliceShowDialog::OnBnClickedRadioBit8()
{
	// TODO: 在此添加控件通知处理程序代码
	this->TryShowOneBit( 7 );
}


void CIntensityBitplaneSliceShowDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( m_pImage )
	{
		CDigitalImageProcessingView *pCurView = GetActiveDIPView();
		if ( pCurView )
		{
			pCurView->EndDIP( m_pImage );
			m_pImage = 0;
		}
	}
}


void CIntensityBitplaneSliceShowDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		pCurView->CancelPreview();
	}
}

void CIntensityBitplaneSliceShowDialog::TryShowOneBit( int bit )
{
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		this->Release();
		this->m_pImage = pCurView->StartDIP();
		m_pImage->IntensityBitPlanesOneBit( bit );

		pCurView->DIPPreview( m_pImage );
	}	
}

void CIntensityBitplaneSliceShowDialog::Release()
{
	if ( m_pImage )
	{
		delete m_pImage;
		m_pImage = 0;
	}
}
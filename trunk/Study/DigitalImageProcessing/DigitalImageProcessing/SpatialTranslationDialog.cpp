// SpatialTranslationDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "SpatialTranslationDialog.h"
#include "afxdialogex.h"

#include "dipcommon.h"
#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"
// CSpatialTranslationDialog 对话框

IMPLEMENT_DYNAMIC(CSpatialTranslationDialog, CDialogEx)


CSpatialTranslationDialog::CSpatialTranslationDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpatialTranslationDialog::IDD, pParent)
	, m_pImage( 0 )
	, m_newCoordX(0)
	, m_newCoordY(0)
{

}

CSpatialTranslationDialog::~CSpatialTranslationDialog()
{
}

void CSpatialTranslationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_newCoordX);
	DDX_Text(pDX, IDC_EDIT_Y, m_newCoordY);
}


BEGIN_MESSAGE_MAP(CSpatialTranslationDialog, CDialogEx)
	ON_BN_CLICKED(ID_PREVIEW, &CSpatialTranslationDialog::OnBnClickedPreview)
	ON_BN_CLICKED(ID_CANCEL, &CSpatialTranslationDialog::OnBnClickedCancel)
	ON_BN_CLICKED(ID_APPLY, &CSpatialTranslationDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CSpatialTranslationDialog 消息处理程序


void CSpatialTranslationDialog::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	

	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		// 重新获取原图片.
		this->ReleaseImage();
		m_pImage = pCurView->StartDIP();

		this->m_pImage->Translate( m_newCoordX, m_newCoordY );

		pCurView->DIPPreview( m_pImage );

	}	
}


void CSpatialTranslationDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		pCurView->CancelPreview();
	}	
	this->ReleaseImage();
}


void CSpatialTranslationDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		if ( 0 == m_pImage )
		{
			m_pImage = pCurView->StartDIP();			
		}

		m_pImage->Translate( m_newCoordX, m_newCoordY );

		pCurView->EndDIP( m_pImage );
		m_pImage = 0;
	}	
}

void CSpatialTranslationDialog::ReleaseImage()
{
	if ( m_pImage )
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

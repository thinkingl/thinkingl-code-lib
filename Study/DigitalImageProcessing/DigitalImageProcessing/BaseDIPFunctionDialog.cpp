// BaseDIPFunctionDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "BaseDIPFunctionDialog.h"
#include "afxdialogex.h"

#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"
#include "dipcommon.h"
// CBaseDIPFunctionDialog 对话框

IMPLEMENT_DYNAMIC(CBaseDIPFunctionDialog, CDialogEx)

CBaseDIPFunctionDialog::CBaseDIPFunctionDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBaseDIPFunctionDialog::IDD, pParent)
	, m_pImage( 0 )
{

}

CBaseDIPFunctionDialog::~CBaseDIPFunctionDialog()
{
	this->ReleaseImage();
}

void CBaseDIPFunctionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseDIPFunctionDialog, CDialogEx)
END_MESSAGE_MAP()


// CBaseDIPFunctionDialog 消息处理程序
void CBaseDIPFunctionDialog::ReleaseImage()
{
	if ( m_pImage )
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

void CBaseDIPFunctionDialog::CancelPreview()
{
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		pCurView->CancelPreview();
	}	
	this->ReleaseImage();
}

void CBaseDIPFunctionDialog::Preview()
{
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		// 重新获取原图片.
		this->ReleaseImage();
		m_pImage = pCurView->StartDIP();

		this->OnDIP( m_pImage );

		pCurView->DIPPreview( m_pImage );

	}	
}

void CBaseDIPFunctionDialog::Apply()
{
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		if ( 0 == m_pImage )
		{
			m_pImage = pCurView->StartDIP();		
			this->OnDIP( m_pImage );
		}
		pCurView->EndDIP( m_pImage );
		m_pImage = 0;
	}	
}

// void CBaseDIPFunctionDialog::OnPriview( CDigitalImage* pImg )
// {
// 	
// }
// 
// void CBaseDIPFunctionDialog::OnApply( CDigitalImage *pImg )
// {
// 	
// }




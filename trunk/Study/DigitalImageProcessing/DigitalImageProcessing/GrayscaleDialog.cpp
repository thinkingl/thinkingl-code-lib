// GrayscaleDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "GrayscaleDialog.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "dipcommon.h"
#include "DigitalImageProcessingView.h"
#include "DigitalImage.h"

// CGrayscaleDialog 对话框

IMPLEMENT_DYNAMIC(CGrayscaleDialog, CDialogEx)

CGrayscaleDialog::CGrayscaleDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGrayscaleDialog::IDD, pParent)
	, m_intensityLevel(8)
{

}

CGrayscaleDialog::~CGrayscaleDialog()
{
}

void CGrayscaleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INTENSITY_LEVEL, m_intensityLevel);
	DDV_MinMaxInt(pDX, m_intensityLevel, 1, 8);
}


BEGIN_MESSAGE_MAP(CGrayscaleDialog, CDialogEx)
	ON_BN_CLICKED(ID_APPLY, &CGrayscaleDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CGrayscaleDialog 消息处理程序




void CGrayscaleDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData( TRUE );
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		CDigitalImage *pDI = pCurView->StartDIP();
		pDI->ToGrayscaleImage( this->m_intensityLevel );
		pCurView->EndDIP();
	}	
}

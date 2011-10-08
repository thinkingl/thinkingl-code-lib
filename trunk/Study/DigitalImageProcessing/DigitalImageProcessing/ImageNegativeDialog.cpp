// ImageNegativeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "ImageNegativeDialog.h"
#include "afxdialogex.h"

#include "dipcommon.h"
#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"

// CImageNegativeDialog 对话框

IMPLEMENT_DYNAMIC(CImageNegativeDialog, CDialogEx)

CImageNegativeDialog::CImageNegativeDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageNegativeDialog::IDD, pParent)
{

}

CImageNegativeDialog::~CImageNegativeDialog()
{
}

void CImageNegativeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageNegativeDialog, CDialogEx)
	ON_BN_CLICKED(ID_APPLY, &CImageNegativeDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CImageNegativeDialog 消息处理程序


void CImageNegativeDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		CDigitalImage *pDI = pCurView->StartDIP();
		
		pDI->ImageNegitive();

		pCurView->EndDIP();
	}	
}

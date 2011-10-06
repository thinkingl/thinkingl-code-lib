// GrayscaleDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "GrayscaleDialog.h"
#include "afxdialogex.h"

#include "MainFrm.h"

// CGrayscaleDialog 对话框

IMPLEMENT_DYNAMIC(CGrayscaleDialog, CDialogEx)

CGrayscaleDialog::CGrayscaleDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGrayscaleDialog::IDD, pParent)
{

}

CGrayscaleDialog::~CGrayscaleDialog()
{
}

void CGrayscaleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGrayscaleDialog, CDialogEx)
	ON_BN_CLICKED(ID_APPLY, &CGrayscaleDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CGrayscaleDialog 消息处理程序




void CGrayscaleDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain = (CMainFrame*) AfxGetMainWnd();
	CView* pAcView = pMain->GetActiveView();
	CDocument *pDoc = pMain->GetActiveDocument();
	CWnd *pWnd = pMain->GetActiveWindow();
	CFrameWnd *pFrame = pMain->GetActiveFrame();
	CView* pAcView2 = pFrame->GetActiveView();
	CDocument *pDoc2 = pFrame->GetActiveDocument();
	int sdfs = 3;
}

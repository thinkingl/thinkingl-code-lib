// IntensityLogTransformationDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "IntensityLogTransformationDialog.h"
#include "afxdialogex.h"

#include "dipcommon.h"
#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"
// CIntensityLogTransformationDialog 对话框

IMPLEMENT_DYNAMIC(CIntensityLogTransformationDialog, CDialogEx)

CIntensityLogTransformationDialog::CIntensityLogTransformationDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIntensityLogTransformationDialog::IDD, pParent)
	, m_logTransformationConstant(20)
{

}

CIntensityLogTransformationDialog::~CIntensityLogTransformationDialog()
{
}

void CIntensityLogTransformationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOG_TRANSFORMATION_CONSTANT, m_logTransformationConstant);
	DDV_MinMaxFloat(pDX, m_logTransformationConstant, 0, 255);
}


BEGIN_MESSAGE_MAP(CIntensityLogTransformationDialog, CDialogEx)
	ON_BN_CLICKED(ID_APPLY, &CIntensityLogTransformationDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CIntensityLogTransformationDialog 消息处理程序


void CIntensityLogTransformationDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		CDigitalImage *pDI = pCurView->StartDIP();

		pDI->IntensityLogTransform( this->m_logTransformationConstant );

		pCurView->EndDIP();
	}	
}

// IntensityPowerTransformationDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "IntensityPowerTransformationDialog.h"
#include "afxdialogex.h"

#include "dipcommon.h"
#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"
// CIntensityPowerTransformationDialog 对话框

IMPLEMENT_DYNAMIC(CIntensityPowerTransformationDialog, CDialogEx)

CIntensityPowerTransformationDialog::CIntensityPowerTransformationDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIntensityPowerTransformationDialog::IDD, pParent)
	, m_const(1)
	, m_v(0.6)
{

}

CIntensityPowerTransformationDialog::~CIntensityPowerTransformationDialog()
{
}

void CIntensityPowerTransformationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POWER_TRANSFORMATION_CONSTANT, m_const);
	DDX_Text(pDX, IDC_EDIT_POWER_TRANSFORMATION_, m_v);
}


BEGIN_MESSAGE_MAP(CIntensityPowerTransformationDialog, CDialogEx)
	ON_BN_CLICKED(ID_APPLY, &CIntensityPowerTransformationDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CIntensityPowerTransformationDialog 消息处理程序


void CIntensityPowerTransformationDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		CDigitalImage *pDI = pCurView->StartDIP();

		pDI->IntensityPowerTransform( this->m_const, this->m_v );

		pCurView->EndDIP();
	}	
}

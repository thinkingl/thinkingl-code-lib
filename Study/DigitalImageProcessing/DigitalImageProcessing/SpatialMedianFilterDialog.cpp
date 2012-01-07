// SpatialMedianFilterDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "SpatialMedianFilterDialog.h"
#include "afxdialogex.h"

#include "DigitalImage.h"
// CSpatialMedianFilterDialog 对话框

IMPLEMENT_DYNAMIC(CSpatialMedianFilterDialog, CBaseDIPFunctionDialog)

CSpatialMedianFilterDialog::CSpatialMedianFilterDialog(CWnd* pParent /*=NULL*/)
	: CBaseDIPFunctionDialog( pParent)
	, m_maskSize(3)
{

}

CSpatialMedianFilterDialog::~CSpatialMedianFilterDialog()
{
}

void CSpatialMedianFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CBaseDIPFunctionDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MASK_SIZE, m_maskSize);
	DDV_MinMaxInt(pDX, m_maskSize, 3, 9999);
}


BEGIN_MESSAGE_MAP(CSpatialMedianFilterDialog, CBaseDIPFunctionDialog)
	ON_BN_CLICKED(ID_PREVIEW, &CSpatialMedianFilterDialog::OnBnClickedPreview)
	ON_BN_CLICKED(ID_CANCEL, &CSpatialMedianFilterDialog::OnBnClickedCancel)
	ON_BN_CLICKED(ID_APPLY, &CSpatialMedianFilterDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CSpatialMedianFilterDialog 消息处理程序


void CSpatialMedianFilterDialog::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Preview();
}


void CSpatialMedianFilterDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->CancelPreview();
}


void CSpatialMedianFilterDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Apply();
}

void CSpatialMedianFilterDialog::OnDIP( CDigitalImage *pImg )
{
	if ( m_maskSize % 2 != 1 )
	{
		MessageBox( _T("Mask大小必须是奇数!") );
		return;
	}
	pImg->SpatialMedianFilter( m_maskSize );
}
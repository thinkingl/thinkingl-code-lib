// NoiseSpatialMeanFilterDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "NoiseSpatialMeanFilterDialog.h"
#include "afxdialogex.h"
#include "DigitalImage.h"

// CNoiseSpatialMeanFilterDialog 对话框


IMPLEMENT_DYNAMIC(CNoiseSpatialMeanFilterDialog, CBaseDIPFunctionDialog)

CNoiseSpatialMeanFilterDialog::CNoiseSpatialMeanFilterDialog(CWnd* pParent /*=NULL*/)
	: CBaseDIPFunctionDialog( pParent)
	, m_maskSize(3)
{

}

CNoiseSpatialMeanFilterDialog::~CNoiseSpatialMeanFilterDialog()
{
}

void CNoiseSpatialMeanFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CBaseDIPFunctionDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MASK_SIZE, m_maskSize);
	DDV_MinMaxInt(pDX, m_maskSize, 3, 999);
}


BEGIN_MESSAGE_MAP(CNoiseSpatialMeanFilterDialog, CBaseDIPFunctionDialog)
	ON_BN_CLICKED(ID_PREVIEW, &CNoiseSpatialMeanFilterDialog::OnBnClickedPreview)
	ON_BN_CLICKED(ID_CANCEL, &CNoiseSpatialMeanFilterDialog::OnBnClickedCancel)
	ON_BN_CLICKED(ID_APPLY, &CNoiseSpatialMeanFilterDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CNoiseSpatialMeanFilterDialog 消息处理程序


void CNoiseSpatialMeanFilterDialog::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Preview();
}


void CNoiseSpatialMeanFilterDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->CancelPreview();
}


void CNoiseSpatialMeanFilterDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Apply();
}

void CNoiseSpatialMeanFilterDialog::OnDIP( CDigitalImage *pImg )
{
	if ( m_maskSize % 2 != 1 )
	{
		MessageBox( _T("Mask大小必须是奇数!") );
		return;
	}
	pImg->SpatialMeanFilter( m_maskSize );
}
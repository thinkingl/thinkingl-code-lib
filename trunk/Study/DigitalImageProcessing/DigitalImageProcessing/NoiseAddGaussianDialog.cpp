// NoiseAddGaussianDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "NoiseAddGaussianDialog.h"
#include "afxdialogex.h"

#include "DigitalImage.h"
// CNoiseAddGaussianDialog 对话框

IMPLEMENT_DYNAMIC(CNoiseAddGaussianDialog, CBaseDIPFunctionDialog)

CNoiseAddGaussianDialog::CNoiseAddGaussianDialog(CWnd* pParent /*=NULL*/)
	: CBaseDIPFunctionDialog( pParent)
	, m_gaussianMean(0)
	, m_gaussionVariance(50)
{

}

CNoiseAddGaussianDialog::~CNoiseAddGaussianDialog()
{
}

void CNoiseAddGaussianDialog::DoDataExchange(CDataExchange* pDX)
{
	CBaseDIPFunctionDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GAUSSIAN_MEAN, m_gaussianMean);
	DDX_Text(pDX, IDC_EDIT_GAUSSIAN_VARIANCE, m_gaussionVariance);
}


BEGIN_MESSAGE_MAP(CNoiseAddGaussianDialog, CBaseDIPFunctionDialog)
	ON_BN_CLICKED(ID_PREVIEW, &CNoiseAddGaussianDialog::OnBnClickedPreview)
	ON_BN_CLICKED(ID_CANCEL, &CNoiseAddGaussianDialog::OnBnClickedCancel)
	ON_BN_CLICKED(ID_APPLY, &CNoiseAddGaussianDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CNoiseAddGaussianDialog 消息处理程序
void CNoiseAddGaussianDialog::OnDIP( CDigitalImage *pImg )
{
	pImg->AddGaussianNoise( m_gaussianMean, m_gaussionVariance );
}

void CNoiseAddGaussianDialog::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Preview();
}


void CNoiseAddGaussianDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->CancelPreview();
}


void CNoiseAddGaussianDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Apply();
}

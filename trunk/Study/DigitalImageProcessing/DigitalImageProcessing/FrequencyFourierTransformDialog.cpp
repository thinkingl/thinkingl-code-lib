// FrequencyFourierTransformDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "FrequencyFourierTransformDialog.h"
#include "afxdialogex.h"

// CFrequencyFourierTransformDialog 对话框

IMPLEMENT_DYNAMIC(CFrequencyFourierTransformDialog, CBaseDIPFunctionDialog)

CFrequencyFourierTransformDialog::CFrequencyFourierTransformDialog(CWnd* pParent /*=NULL*/)
	: CBaseDIPFunctionDialog( pParent)
	, m_fourierTransformOrInverseFourierTransform(0)
	, m_centerTheTransform(TRUE)
{

}

CFrequencyFourierTransformDialog::~CFrequencyFourierTransformDialog()
{
}

void CFrequencyFourierTransformDialog::DoDataExchange(CDataExchange* pDX)
{
	CBaseDIPFunctionDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_FOURIER_TRANSFORM, m_fourierTransformOrInverseFourierTransform);
	DDX_Check(pDX, IDC_CHECK_CENTER, m_centerTheTransform);
}


BEGIN_MESSAGE_MAP(CFrequencyFourierTransformDialog, CBaseDIPFunctionDialog)
	ON_BN_CLICKED(ID_PREVIEW, &CFrequencyFourierTransformDialog::OnBnClickedPreview)
	ON_BN_CLICKED(ID_CANCEL, &CFrequencyFourierTransformDialog::OnBnClickedCancel)
	ON_BN_CLICKED(ID_APPLY, &CFrequencyFourierTransformDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CFrequencyFourierTransformDialog 消息处理程序


void CFrequencyFourierTransformDialog::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Preview();
}


void CFrequencyFourierTransformDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->CancelPreview();
}


void CFrequencyFourierTransformDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Apply();
}

void CFrequencyFourierTransformDialog::OnDIP( CDigitalImage *pImg )
{
	if( 0 == m_fourierTransformOrInverseFourierTransform )
	{
		pImg->FourierTransform( TRUE == this->m_centerTheTransform );
	}
	else
	{
		pImg->InverseFourierTransform( );
	}
}
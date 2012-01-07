// NoiseAddSaltAndPepperDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "NoiseAddSaltAndPepperDialog.h"
#include "afxdialogex.h"

#include "DigitalImage.h"
// CNoiseAddSaltAndPepperDialog 对话框

IMPLEMENT_DYNAMIC(CNoiseAddSaltAndPepperDialog, CBaseDIPFunctionDialog)

CNoiseAddSaltAndPepperDialog::CNoiseAddSaltAndPepperDialog(CWnd* pParent /*=NULL*/)
	: CBaseDIPFunctionDialog( pParent)
	, m_saltProbability(50)
	, m_pepperProbability(50)
{

}

CNoiseAddSaltAndPepperDialog::~CNoiseAddSaltAndPepperDialog()
{
}

void CNoiseAddSaltAndPepperDialog::DoDataExchange(CDataExchange* pDX)
{
	CBaseDIPFunctionDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SALT_PROBABILITY, m_saltProbability);
	DDV_MinMaxInt(pDX, m_saltProbability, 1, 99999999);
	DDX_Text(pDX, IDC_EDIT_PEPPER_PROBABILITY, m_pepperProbability);
	DDV_MinMaxInt(pDX, m_pepperProbability, 1, 99999999);
}


BEGIN_MESSAGE_MAP(CNoiseAddSaltAndPepperDialog, CBaseDIPFunctionDialog)
	ON_BN_CLICKED(ID_PREVIEW, &CNoiseAddSaltAndPepperDialog::OnBnClickedPreview)
	ON_BN_CLICKED(ID_CANCEL, &CNoiseAddSaltAndPepperDialog::OnBnClickedCancel)
	ON_BN_CLICKED(ID_APPLY, &CNoiseAddSaltAndPepperDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CNoiseAddSaltAndPepperDialog 消息处理程序


void CNoiseAddSaltAndPepperDialog::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Preview();
}


void CNoiseAddSaltAndPepperDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->CancelPreview();
}


void CNoiseAddSaltAndPepperDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Apply();
}

void CNoiseAddSaltAndPepperDialog::OnDIP( CDigitalImage *pImg )
{
	pImg->AddSaltAndPepperNoise( m_saltProbability, m_pepperProbability );
}

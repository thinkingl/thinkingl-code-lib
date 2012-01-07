// NoiseAddGaussianDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "NoiseAddGaussianDialog.h"
#include "afxdialogex.h"

#include "DigitalImage.h"
// CNoiseAddGaussianDialog �Ի���

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


// CNoiseAddGaussianDialog ��Ϣ�������
void CNoiseAddGaussianDialog::OnDIP( CDigitalImage *pImg )
{
	pImg->AddGaussianNoise( m_gaussianMean, m_gaussionVariance );
}

void CNoiseAddGaussianDialog::OnBnClickedPreview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->Preview();
}


void CNoiseAddGaussianDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->CancelPreview();
}


void CNoiseAddGaussianDialog::OnBnClickedApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->Apply();
}

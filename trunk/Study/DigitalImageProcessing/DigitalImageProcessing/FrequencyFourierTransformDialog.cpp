// FrequencyFourierTransformDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "FrequencyFourierTransformDialog.h"
#include "afxdialogex.h"

// CFrequencyFourierTransformDialog �Ի���

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


// CFrequencyFourierTransformDialog ��Ϣ�������


void CFrequencyFourierTransformDialog::OnBnClickedPreview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->Preview();
}


void CFrequencyFourierTransformDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->CancelPreview();
}


void CFrequencyFourierTransformDialog::OnBnClickedApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
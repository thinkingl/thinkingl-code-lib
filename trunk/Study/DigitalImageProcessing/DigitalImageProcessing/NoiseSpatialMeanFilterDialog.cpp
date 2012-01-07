// NoiseSpatialMeanFilterDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "NoiseSpatialMeanFilterDialog.h"
#include "afxdialogex.h"
#include "DigitalImage.h"

// CNoiseSpatialMeanFilterDialog �Ի���


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


// CNoiseSpatialMeanFilterDialog ��Ϣ�������


void CNoiseSpatialMeanFilterDialog::OnBnClickedPreview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->Preview();
}


void CNoiseSpatialMeanFilterDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->CancelPreview();
}


void CNoiseSpatialMeanFilterDialog::OnBnClickedApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->Apply();
}

void CNoiseSpatialMeanFilterDialog::OnDIP( CDigitalImage *pImg )
{
	if ( m_maskSize % 2 != 1 )
	{
		MessageBox( _T("Mask��С����������!") );
		return;
	}
	pImg->SpatialMeanFilter( m_maskSize );
}
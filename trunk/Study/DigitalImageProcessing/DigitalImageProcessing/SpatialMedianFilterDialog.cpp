// SpatialMedianFilterDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "SpatialMedianFilterDialog.h"
#include "afxdialogex.h"

#include "DigitalImage.h"
// CSpatialMedianFilterDialog �Ի���

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


// CSpatialMedianFilterDialog ��Ϣ�������


void CSpatialMedianFilterDialog::OnBnClickedPreview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->Preview();
}


void CSpatialMedianFilterDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->CancelPreview();
}


void CSpatialMedianFilterDialog::OnBnClickedApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->Apply();
}

void CSpatialMedianFilterDialog::OnDIP( CDigitalImage *pImg )
{
	if ( m_maskSize % 2 != 1 )
	{
		MessageBox( _T("Mask��С����������!") );
		return;
	}
	pImg->SpatialMedianFilter( m_maskSize );
}
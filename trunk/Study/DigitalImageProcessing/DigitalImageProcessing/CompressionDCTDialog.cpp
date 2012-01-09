// CompressionDCTDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "CompressionDCTDialog.h"
#include "afxdialogex.h"


// CCompressionDCTDialog �Ի���

IMPLEMENT_DYNAMIC(CCompressionDCTDialog, CBaseDIPFunctionDialog)

CCompressionDCTDialog::CCompressionDCTDialog(CWnd* pParent /*=NULL*/)
	: CBaseDIPFunctionDialog( pParent)
	, m_DCT( true )
	, m_compressionRate(0)
{

}

CCompressionDCTDialog::~CCompressionDCTDialog()
{
}

void CCompressionDCTDialog::DoDataExchange(CDataExchange* pDX)
{
	CBaseDIPFunctionDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_COMPRESSION_RATE, m_compressionRate);
//	DDV_MinMaxInt(pDX, m_compressionRate, 0, 128);
	DDX_Control(pDX, IDC_COMBO_COMPRESSION_RATE, m_cmbCompressionRate);
}


BEGIN_MESSAGE_MAP(CCompressionDCTDialog, CBaseDIPFunctionDialog)
	ON_BN_CLICKED(ID_PREVIEW, &CCompressionDCTDialog::OnBnClickedPreview)
	ON_BN_CLICKED(ID_CANCEL, &CCompressionDCTDialog::OnBnClickedCancel)
	ON_BN_CLICKED(ID_APPLY, &CCompressionDCTDialog::OnBnClickedApply)
	ON_BN_CLICKED(ID_APPLY_DCT, &CCompressionDCTDialog::OnBnClickedApplyDct)
	ON_BN_CLICKED(ID_APPLY_IDCT, &CCompressionDCTDialog::OnBnClickedApplyIdct)
END_MESSAGE_MAP()


// CCompressionDCTDialog ��Ϣ�������


void CCompressionDCTDialog::OnBnClickedPreview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->Preview();
}


void CCompressionDCTDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->CancelPreview();
}


void CCompressionDCTDialog::OnBnClickedApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	this->Apply();
}

void CCompressionDCTDialog::OnDIP( CDigitalImage *pImg )
{
	bool isPowerOf2 = false;
	for( int i=1; i<13; ++i )
	{
		if( (1<<i) == pImg->GetWidth() )
		{
			isPowerOf2 = true;
			break;
		}
	}

	if( pImg->GetWidth() != pImg->GetHeight() 
		|| !isPowerOf2 )
	{
		MessageBox( _T( "Sorry, ��Ϊ�㷨��ª, ֻ��֧�ֳ�����Ȳ�����2���ݴε�ͼƬ." ) );
		return;
	}

	if ( m_DCT )
	{
		pImg->DiscreteCosineTransform();
	}
	else
	{
		CString compressionRate;
		m_cmbCompressionRate.GetWindowText( compressionRate );
		int rate = _ttoi( (LPCTSTR)compressionRate );
		pImg->InverseDiscreteCosineTransform( (double)1/rate );
	}
	
}

void CCompressionDCTDialog::OnBnClickedApplyDct()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DCT = true;
	this->UpdateData();
	this->Apply();
}


void CCompressionDCTDialog::OnBnClickedApplyIdct()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DCT = false;
	this->UpdateData();
	this->Apply();
}

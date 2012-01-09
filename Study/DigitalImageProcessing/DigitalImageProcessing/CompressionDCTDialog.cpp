// CompressionDCTDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "CompressionDCTDialog.h"
#include "afxdialogex.h"


// CCompressionDCTDialog 对话框

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


// CCompressionDCTDialog 消息处理程序


void CCompressionDCTDialog::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Preview();
}


void CCompressionDCTDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->CancelPreview();
}


void CCompressionDCTDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
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
		MessageBox( _T( "Sorry, 因为算法简陋, 只能支持长宽相等并且是2的幂次的图片." ) );
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
	// TODO: 在此添加控件通知处理程序代码
	m_DCT = true;
	this->UpdateData();
	this->Apply();
}


void CCompressionDCTDialog::OnBnClickedApplyIdct()
{
	// TODO: 在此添加控件通知处理程序代码
	m_DCT = false;
	this->UpdateData();
	this->Apply();
}

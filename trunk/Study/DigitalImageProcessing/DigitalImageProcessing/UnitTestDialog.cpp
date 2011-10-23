// UnitTestDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "UnitTestDialog.h"
#include "afxdialogex.h"


// CUnitTestDialog �Ի���

IMPLEMENT_DYNAMIC(CUnitTestDialog, CDialogEx)

CUnitTestDialog::CUnitTestDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUnitTestDialog::IDD, pParent)
{

}

CUnitTestDialog::~CUnitTestDialog()
{
}

void CUnitTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_HISTOGRAM, m_histogramCtrl);
}


BEGIN_MESSAGE_MAP(CUnitTestDialog, CDialogEx)
END_MESSAGE_MAP()


// CUnitTestDialog ��Ϣ�������


BOOL CUnitTestDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CDigitalImage::THistogramData hisData;
	hisData.push_back( 128 );
	hisData.push_back( 64 );

	this->m_histogramCtrl.SetHistoramData( hisData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


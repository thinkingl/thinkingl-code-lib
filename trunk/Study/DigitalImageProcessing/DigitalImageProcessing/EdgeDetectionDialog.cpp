// EdgeDetectionDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "EdgeDetectionDialog.h"
#include "afxdialogex.h"


// CEdgeDetectionDialog �Ի���

IMPLEMENT_DYNAMIC(CEdgeDetectionDialog, CBaseDIPFunctionDialog)

CEdgeDetectionDialog::CEdgeDetectionDialog(CWnd* pParent /*=NULL*/)
	: CBaseDIPFunctionDialog( pParent)
	, m_edgeDetectType( Sobel )
{

}

CEdgeDetectionDialog::~CEdgeDetectionDialog()
{
}

void CEdgeDetectionDialog::DoDataExchange(CDataExchange* pDX)
{
	CBaseDIPFunctionDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEdgeDetectionDialog, CBaseDIPFunctionDialog)
	ON_BN_CLICKED(ID_APPLY_SOBEL, &CEdgeDetectionDialog::OnBnClickedApplySobel)
	ON_BN_CLICKED(ID_APPLY_ROBERTS, &CEdgeDetectionDialog::OnBnClickedApplyRoberts)
	ON_BN_CLICKED(ID_APPLY_PREWITT, &CEdgeDetectionDialog::OnBnClickedApplyPrewitt)
END_MESSAGE_MAP()


// CEdgeDetectionDialog ��Ϣ�������


void CEdgeDetectionDialog::OnBnClickedApplySobel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	m_edgeDetectType = Sobel;
	this->Preview();
}

void CEdgeDetectionDialog::OnDIP( CDigitalImage *pImg )
{
	switch( m_edgeDetectType )
	{
	case Sobel:
		pImg->SobelEdgeDetect();
		break;
	case Roberts:
		pImg->RobertsEdgeDetect();
	    break;
	case Prewitt:
		pImg->PrewittEdgeDetect();
		break;
	default:
	    break;
	}
	
}

void CEdgeDetectionDialog::OnBnClickedApplyRoberts()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	m_edgeDetectType = Roberts;
	this->Preview();
}


void CEdgeDetectionDialog::OnBnClickedApplyPrewitt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	m_edgeDetectType = Prewitt;
	this->Preview();
}

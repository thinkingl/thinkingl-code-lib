// EdgeDetectionDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "EdgeDetectionDialog.h"
#include "afxdialogex.h"


// CEdgeDetectionDialog 对话框

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


// CEdgeDetectionDialog 消息处理程序


void CEdgeDetectionDialog::OnBnClickedApplySobel()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	m_edgeDetectType = Roberts;
	this->Preview();
}


void CEdgeDetectionDialog::OnBnClickedApplyPrewitt()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	m_edgeDetectType = Prewitt;
	this->Preview();
}

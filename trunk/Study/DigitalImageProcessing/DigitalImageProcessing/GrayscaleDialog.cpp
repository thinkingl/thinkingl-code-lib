// GrayscaleDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "GrayscaleDialog.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "dipcommon.h"
#include "DigitalImageProcessingView.h"
#include "DigitalImage.h"

// CGrayscaleDialog �Ի���

IMPLEMENT_DYNAMIC(CGrayscaleDialog, CDialogEx)

CGrayscaleDialog::CGrayscaleDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGrayscaleDialog::IDD, pParent)
	, m_intensityLevel(8)
{

}

CGrayscaleDialog::~CGrayscaleDialog()
{
}

void CGrayscaleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INTENSITY_LEVEL, m_intensityLevel);
	DDV_MinMaxInt(pDX, m_intensityLevel, 1, 8);
}


BEGIN_MESSAGE_MAP(CGrayscaleDialog, CDialogEx)
	ON_BN_CLICKED(ID_APPLY, &CGrayscaleDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CGrayscaleDialog ��Ϣ�������




void CGrayscaleDialog::OnBnClickedApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData( TRUE );
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		CDigitalImage *pDI = pCurView->StartDIP();
		pDI->ToGrayscaleImage( this->m_intensityLevel );
		pCurView->EndDIP();
	}	
}

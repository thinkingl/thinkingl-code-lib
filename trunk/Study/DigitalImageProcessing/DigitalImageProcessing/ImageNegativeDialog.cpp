// ImageNegativeDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "ImageNegativeDialog.h"
#include "afxdialogex.h"

#include "dipcommon.h"
#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"

// CImageNegativeDialog �Ի���

IMPLEMENT_DYNAMIC(CImageNegativeDialog, CDialogEx)

CImageNegativeDialog::CImageNegativeDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageNegativeDialog::IDD, pParent)
{

}

CImageNegativeDialog::~CImageNegativeDialog()
{
}

void CImageNegativeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageNegativeDialog, CDialogEx)
	ON_BN_CLICKED(ID_APPLY, &CImageNegativeDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CImageNegativeDialog ��Ϣ�������


void CImageNegativeDialog::OnBnClickedApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		CDigitalImage *pDI = pCurView->StartDIP();
		
		pDI->ImageNegitive();

		pCurView->EndDIP();
	}	
}

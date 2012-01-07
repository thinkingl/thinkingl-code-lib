// SpatialScalingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "SpatialScalingDialog.h"
#include "afxdialogex.h"

#include "DigitalImage.h"
// CSpatialScalingDialog 对话框

IMPLEMENT_DYNAMIC(CSpatialScalingDialog, CBaseDIPFunctionDialog)

CSpatialScalingDialog::CSpatialScalingDialog(CWnd* pParent /*=NULL*/)
	: CBaseDIPFunctionDialog(pParent)
	, m_scaleX(1.5)
	, m_scaleY(1.5)
	, m_interpolateType(0)
{

}

CSpatialScalingDialog::~CSpatialScalingDialog()
{
}

void CSpatialScalingDialog::DoDataExchange(CDataExchange* pDX)
{
	CBaseDIPFunctionDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SCALE_X, m_scaleX);
	DDX_Text(pDX, IDC_EDIT_SCALE_Y, m_scaleY);
	DDV_MinMaxDouble(pDX, m_scaleX, 0.1, 5.0);
	DDV_MinMaxDouble(pDX, m_scaleY, 0.1, 5.0);
	DDX_Radio(pDX, IDC_RADIO_NEAREST_NEIGHBOR2, m_interpolateType);
}


BEGIN_MESSAGE_MAP(CSpatialScalingDialog, CBaseDIPFunctionDialog)
	ON_BN_CLICKED(ID_PREVIEW, &CSpatialScalingDialog::OnBnClickedPreview)
	ON_BN_CLICKED(ID_CANCEL, &CSpatialScalingDialog::OnBnClickedCancel)
	ON_BN_CLICKED(ID_APPLY, &CSpatialScalingDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// CSpatialScalingDialog 消息处理程序


void CSpatialScalingDialog::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Preview();
}


void CSpatialScalingDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->CancelPreview();
}


void CSpatialScalingDialog::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	this->Apply();
}

void CSpatialScalingDialog::OnDIP( CDigitalImage *pImg )
{
	CDigitalImage::EInterpolateType type = ( m_interpolateType == 0 ) ?  CDigitalImage::InterpolateNearestNeighbor : CDigitalImage::InterpolateBilinear;
	
	pImg->Scale( m_scaleX, m_scaleY, type );
}

// void CSpatialScalingDialog::OnPriview( CDigitalImage* pImg )
// {
// 
// }
// 
// void CSpatialScalingDialog::OnApply( CDigitalImage *pImg )
// {
// 
// }


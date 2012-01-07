// SpatialRotationDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "SpatialRotationDialog.h"
#include "afxdialogex.h"

#include "dipcommon.h"
#include "DigitalImage.h"
#include "DigitalImageProcessingView.h"
// CSpatialRotationDialog �Ի���

IMPLEMENT_DYNAMIC(CSpatialRotationDialog, CDialogEx)

CSpatialRotationDialog::CSpatialRotationDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpatialRotationDialog::IDD, pParent)
	, m_whichClockWiseChecked(0)
	, m_rotationAngle(10)
	, m_whichInterpolationFunction(0)
	, m_pImage( 0 )
	, m_rotationX(0)
	, m_rotationY(0)
{

}

CSpatialRotationDialog::~CSpatialRotationDialog()
{
}

void CSpatialRotationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_CLOCKWISE, m_whichClockWiseChecked);
	DDX_Text(pDX, IDC_EDIT_ROTATION_ANGLE, m_rotationAngle);
	DDX_Radio(pDX, IDC_RADIO_NEAREST_NEIGHBOR, m_whichInterpolationFunction);
	DDX_Text(pDX, IDC_EDIT_X, m_rotationX);
	DDX_Text(pDX, IDC_EDIT_Y, m_rotationY);
}


BEGIN_MESSAGE_MAP(CSpatialRotationDialog, CDialogEx)
	ON_BN_CLICKED(ID_APPLY, &CSpatialRotationDialog::OnBnClickedApply)
	ON_BN_CLICKED(IDC_RADIO_CLOCKWISE, &CSpatialRotationDialog::OnBnClickedRadioClockwise)
	ON_BN_CLICKED(ID_PREVIEW, &CSpatialRotationDialog::OnBnClickedPreview)
	ON_BN_CLICKED(ID_CANCEL, &CSpatialRotationDialog::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CSpatialRotationDialog ��Ϣ�������


void CSpatialRotationDialog::OnBnClickedApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();


	if ( 0 == m_whichClockWiseChecked )
	{
		m_rotationAngle = -m_rotationAngle;
	}

	CDigitalImage::EInterpolateType interpolateType = CDigitalImage::InterpolateNearestNeighbor;
	if ( m_whichInterpolationFunction == 0 )
	{
		interpolateType = CDigitalImage::InterpolateNearestNeighbor;
	}
	else
	{
		interpolateType = CDigitalImage::InterpolateBilinear;
	}

	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		if ( 0 == m_pImage )
		{
			m_pImage = pCurView->StartDIP();			
		}

		m_pImage->Rotate( m_rotationAngle, m_rotationX, m_rotationY, interpolateType );

		pCurView->EndDIP( m_pImage );
		m_pImage = 0;
	}	
}


void CSpatialRotationDialog::OnBnClickedRadioClockwise()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CSpatialRotationDialog::OnBnClickedPreview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData();
	if ( 0 == m_whichClockWiseChecked )
	{
		m_rotationAngle = -m_rotationAngle;
	}

	CDigitalImage::EInterpolateType interpolateType = CDigitalImage::InterpolateNearestNeighbor;
	if ( m_whichInterpolationFunction == 0 )
	{
		interpolateType = CDigitalImage::InterpolateNearestNeighbor;
	}
	else
	{
		interpolateType = CDigitalImage::InterpolateBilinear;
	}

	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		// ���»�ȡԭͼƬ.
		this->ReleaseImage();
		m_pImage = pCurView->StartDIP();

		this->m_pImage->Rotate( m_rotationAngle, m_rotationX, m_rotationY, interpolateType );

		pCurView->DIPPreview( m_pImage );

	}	
}


void CSpatialRotationDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDigitalImageProcessingView *pCurView = GetActiveDIPView();
	if ( pCurView )
	{
		pCurView->CancelPreview();
	}	
	this->ReleaseImage();
}

void CSpatialRotationDialog::ReleaseImage()
{
	if ( m_pImage )
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}


void CSpatialRotationDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	this->UpdateData();

	// ����ת֧���ƶ���ͼƬ����.
	if ( m_rotationX == 0 && m_rotationY == 0 )
	{
		if ( 0 == m_pImage )
		{
			CDigitalImageProcessingView *pCurView = GetActiveDIPView();
			if ( pCurView )
			{
				m_pImage = pCurView->StartDIP();	
			}				
		}

		if ( m_pImage )
		{
			m_rotationX = m_pImage->GetWidth() / 2;
			m_rotationY = m_pImage->GetHeight() / 2;

			this->UpdateData( FALSE );
		}
	}

	

}

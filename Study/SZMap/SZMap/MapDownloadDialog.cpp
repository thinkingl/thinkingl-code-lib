// MapDownloadDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SZMap.h"
#include "MapDownloadDialog.h"
#include "afxdialogex.h"


// CMapDownloadDialog 对话框

IMPLEMENT_DYNAMIC(CMapDownloadDialog, CDialogEx)

CMapDownloadDialog::CMapDownloadDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMapDownloadDialog::IDD, pParent)
	, m_dbLongitudeLeftTop(0)
	, m_dbLatitudeLefttop(0)
	, m_dbLongitudeRightbottom(0)
	, m_dbLatitudeRightbottom(0)
	, m_nMinMapLevel(0)
	, m_nMaxLevel(0)
	, m_bDownloadMap(FALSE)
	, m_bDownloadSatellite(FALSE)
	, m_strMapDownloadPath(_T(""))
	, m_strDownloadExePath(_T(""))
{
	// 用上海-2级别地图初始化参数.
	m_bDownloadMap = TRUE;
	m_bDownloadSatellite = FALSE;

	m_pMapDownloadMission = NULL;
}

CMapDownloadDialog::~CMapDownloadDialog()
{
	// 删除...
	if ( m_pMapDownloadMission )
	{
		delete m_pMapDownloadMission;
		m_pMapDownloadMission = NULL;
	}
}

void CMapDownloadDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LONGITUDE_LEFTTOP, m_dbLongitudeLeftTop);
	DDX_Text(pDX, IDC_EDIT_LATITUDE_LEFTTOP, m_dbLatitudeLefttop);
	DDX_Text(pDX, IDC_EDIT_LONGITUDE_RIGHTBOTTOM, m_dbLongitudeRightbottom);
	DDX_Text(pDX, IDC_EDIT_LATITUDE_RIGHTBOTTOM, m_dbLatitudeRightbottom);
	DDX_Text(pDX, IDC_EDIT_MIN_LEVEL, m_nMinMapLevel);
	DDX_Text(pDX, IDC_EDIT_MAX_LEVEL, m_nMaxLevel);
	DDX_Check(pDX, IDC_CHECK_MAP, m_bDownloadMap);
	DDX_Check(pDX, IDC_CHECK_SATELLITE, m_bDownloadSatellite);
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD, m_progressDownload);
	DDX_Control(pDX, IDC_EDIT_LOG, m_editLog);
	DDX_Text(pDX, IDC_EDIT_DOWNLOAD_DIR, m_strMapDownloadPath);
	DDX_Text(pDX, IDC_EDIT_DOWNLOAD_EXE_PATH, m_strDownloadExePath);
}


BEGIN_MESSAGE_MAP(CMapDownloadDialog, CDialogEx)
	ON_BN_CLICKED(ID_BUTTON_DOWNLOAD, &CMapDownloadDialog::OnBnClickedButtonDownload)
END_MESSAGE_MAP()


// CMapDownloadDialog 消息处理程序


void CMapDownloadDialog::OnBnClickedButtonDownload()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData( );
	
	if ( NULL == this->m_pMapDownloadMission )
	{
		
		m_pMapDownloadMission = new CMapDownloadMission(
			CCoord( m_dbLatitudeLefttop, m_dbLongitudeLeftTop, m_nMinMapLevel ),
			CCoord( m_dbLatitudeRightbottom, m_dbLongitudeRightbottom, m_nMaxLevel ),
			(LPCTSTR)m_strMapDownloadPath,
			GoogleMaps,
			MAP_NORMAL
			);
	}
	else
	{

	}
}

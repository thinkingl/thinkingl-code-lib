// DialogConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "SZMap.h"
#include "DialogConfig.h"
#include "afxdialogex.h"
#include "szmapconfig.h"

#include "correctionpointdatabase.h"
// CDialogConfig 对话框

IMPLEMENT_DYNAMIC(CDialogConfig, CDialogEx)

CDialogConfig::CDialogConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogConfig::IDD, pParent)
	, m_strImgDir(_T(""))
	, m_strImgSubDir(_T(""))
	, m_strDatabasePath(_T(""))
{

}

CDialogConfig::~CDialogConfig()
{
}

void CDialogConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MAP_IMAGE_DIRECTORY, m_strImgDir);
	DDX_CBString(pDX, IDC_COMBO_IMG_SUB_DIR, m_strImgSubDir);
	DDX_Text(pDX, IDC_EDIT_MAP_DATABASE_PATH, m_strDatabasePath);
}


BEGIN_MESSAGE_MAP(CDialogConfig, CDialogEx)
	ON_BN_CLICKED(ID_BUTTON_EXPLORER, &CDialogConfig::OnBnClickedButtonExplorer)
	ON_BN_CLICKED(IDOK, &CDialogConfig::OnBnClickedOk)
	ON_BN_CLICKED(ID_BUTTON_EXPLORER_DATABASE, &CDialogConfig::OnBnClickedButtonExplorerDatabase)
	ON_BN_CLICKED(ID_BUTTON_DATABASE_IMPORT, &CDialogConfig::OnBnClickedButtonDatabaseImport)
END_MESSAGE_MAP()


// CDialogConfig 消息处理程序


BOOL CDialogConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	this->m_strImgDir = CSZMapConfig::GetMapImageDir().c_str();

	this->m_strImgSubDir = CSZMapConfig::GetMapSubDir().c_str();

	this->m_strDatabasePath = CSZMapConfig::GetDatabasePath().c_str();

	this->UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialogConfig::OnBnClickedButtonExplorer()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFOW selInfo;
	memset( &selInfo, 0, sizeof( selInfo ) );
	selInfo.hwndOwner = this->GetSafeHwnd();
	selInfo.lpszTitle = _T( "选择地图图片文件夹" );

	
	LPITEMIDLIST pRet = SHBrowseForFolder( &selInfo );
	if ( pRet != NULL )
	{
		CString strTmp;
		SHGetPathFromIDList( pRet, strTmp.GetBuffer( MAX_PATH ) );
		strTmp.ReleaseBuffer();

		this->m_strImgDir = strTmp;
	}

	this->UpdateData( FALSE );

}


void CDialogConfig::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData( TRUE );

	CSZMapConfig::SetMapImageDir( (LPCTSTR)this->m_strImgDir );
	CSZMapConfig::SetMapSubDir( (LPCTSTR)this->m_strImgSubDir );
	CSZMapConfig::SetDatabasePath( (LPCTSTR)this->m_strDatabasePath );

	CDialogEx::OnOK();
}


void CDialogConfig::OnBnClickedButtonExplorerDatabase()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	CFileDialog fDlg( TRUE, 0, m_strDatabasePath );
	fDlg.DoModal();

	CString databasePath = fDlg.GetPathName();

	this->m_strDatabasePath = databasePath;

	UpdateData( FALSE );
}


void CDialogConfig::OnBnClickedButtonDatabaseImport()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fDlg( TRUE );
	fDlg.DoModal();

	CString textFile = fDlg.GetPathName();

	CCorrectionPointDatabase cpdb;
	cpdb.OpenDatabase( (LPCTSTR)m_strDatabasePath );

	// 格式: 地球经度 地球纬度 火星经度偏移量 火星纬度偏移量
	ifstream iFile( (LPCTSTR)textFile );
	while ( iFile )
	{
		double longitudeEarth, lantitudeEarth, marsLongitudeOffset, marsLantitudeOffset;
		iFile >> longitudeEarth >> lantitudeEarth >> marsLongitudeOffset >> marsLantitudeOffset;

		// 将这条记录导入数据库.
		double longitudeMars = longitudeEarth + marsLongitudeOffset;
		double latitudeMars = lantitudeEarth + marsLantitudeOffset;
		cpdb.InsertCorrectionPoint( longitudeEarth, lantitudeEarth, longitudeMars, latitudeMars );


	}

	// 关闭数据库.
	cpdb.Close();

}

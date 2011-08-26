// DialogConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SZMap.h"
#include "DialogConfig.h"
#include "afxdialogex.h"
#include "szmapconfig.h"

#include "correctionpointdatabase.h"
// CDialogConfig �Ի���

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


// CDialogConfig ��Ϣ�������


BOOL CDialogConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	this->m_strImgDir = CSZMapConfig::GetMapImageDir().c_str();

	this->m_strImgSubDir = CSZMapConfig::GetMapSubDir().c_str();

	this->m_strDatabasePath = CSZMapConfig::GetDatabasePath().c_str();

	this->UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDialogConfig::OnBnClickedButtonExplorer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFOW selInfo;
	memset( &selInfo, 0, sizeof( selInfo ) );
	selInfo.hwndOwner = this->GetSafeHwnd();
	selInfo.lpszTitle = _T( "ѡ���ͼͼƬ�ļ���" );

	
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData( TRUE );

	CSZMapConfig::SetMapImageDir( (LPCTSTR)this->m_strImgDir );
	CSZMapConfig::SetMapSubDir( (LPCTSTR)this->m_strImgSubDir );
	CSZMapConfig::SetDatabasePath( (LPCTSTR)this->m_strDatabasePath );

	CDialogEx::OnOK();
}


void CDialogConfig::OnBnClickedButtonExplorerDatabase()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	CFileDialog fDlg( TRUE, 0, m_strDatabasePath );
	fDlg.DoModal();

	CString databasePath = fDlg.GetPathName();

	this->m_strDatabasePath = databasePath;

	UpdateData( FALSE );
}


void CDialogConfig::OnBnClickedButtonDatabaseImport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fDlg( TRUE );
	fDlg.DoModal();

	CString textFile = fDlg.GetPathName();

	CCorrectionPointDatabase cpdb;
	cpdb.OpenDatabase( (LPCTSTR)m_strDatabasePath );

	// ��ʽ: ���򾭶� ����γ�� ���Ǿ���ƫ���� ����γ��ƫ����
	ifstream iFile( (LPCTSTR)textFile );
	while ( iFile )
	{
		double longitudeEarth, lantitudeEarth, marsLongitudeOffset, marsLantitudeOffset;
		iFile >> longitudeEarth >> lantitudeEarth >> marsLongitudeOffset >> marsLantitudeOffset;

		// ��������¼�������ݿ�.
		double longitudeMars = longitudeEarth + marsLongitudeOffset;
		double latitudeMars = lantitudeEarth + marsLantitudeOffset;
		cpdb.InsertCorrectionPoint( longitudeEarth, lantitudeEarth, longitudeMars, latitudeMars );


	}

	// �ر����ݿ�.
	cpdb.Close();

}

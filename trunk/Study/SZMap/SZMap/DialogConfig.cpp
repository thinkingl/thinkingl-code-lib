// DialogConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SZMap.h"
#include "DialogConfig.h"
#include "afxdialogex.h"


// CDialogConfig �Ի���

IMPLEMENT_DYNAMIC(CDialogConfig, CDialogEx)

CDialogConfig::CDialogConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogConfig::IDD, pParent)
	, m_strImgDir(_T(""))
	, m_strImgSubDir(_T(""))
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
}


BEGIN_MESSAGE_MAP(CDialogConfig, CDialogEx)
	ON_BN_CLICKED(ID_BUTTON_EXPLORER, &CDialogConfig::OnBnClickedButtonExplorer)
	ON_BN_CLICKED(IDOK, &CDialogConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogConfig ��Ϣ�������


BOOL CDialogConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strDefImgDir;
	::GetModuleFileName( NULL, strDefImgDir.GetBuffer( MAX_PATH ), MAX_PATH );
	strDefImgDir.ReleaseBuffer();
	int nPos = strDefImgDir.ReverseFind( '\\' );
	if ( nPos != -1 )
	{
		strDefImgDir = strDefImgDir.Left( nPos );
	}


	CString strImgPath;
	LPTSTR buffer = strImgPath.GetBuffer( MAX_PATH );
	GetPrivateProfileString( _T( "szmapcfg" ), _T( "mapdir" ), strDefImgDir, buffer, MAX_PATH, _T( "szmap.ini" ) );
	strImgPath.ReleaseBuffer();

	this->m_strImgDir = strImgPath;


	CString strImgSubDir;
	LPTSTR szSubDir = strImgSubDir.GetBuffer( MAX_PATH );
	GetPrivateProfileString( _T( "szmapcfg" ), _T( "subdir" ), _T("tiles"), szSubDir, MAX_PATH, _T( "szmap.ini" ) );
	strImgSubDir.ReleaseBuffer();
	this->m_strImgSubDir = strImgSubDir;

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

	::WritePrivateProfileString( _T( "szmapcfg" ), _T( "mapdir" ), this->m_strImgDir, _T( "szmap.ini" ) );

	::WritePrivateProfileString( _T( "szmapcfg" ), _T( "subdir" ), this->m_strImgSubDir, _T( "szmap.ini" ) );

	CDialogEx::OnOK();
}

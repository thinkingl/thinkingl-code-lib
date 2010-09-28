// DialogConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "SZMap.h"
#include "DialogConfig.h"
#include "afxdialogex.h"


// CDialogConfig 对话框

IMPLEMENT_DYNAMIC(CDialogConfig, CDialogEx)

CDialogConfig::CDialogConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogConfig::IDD, pParent)
	, m_strImgDir(_T(""))
{

}

CDialogConfig::~CDialogConfig()
{
}

void CDialogConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MAP_IMAGE_DIRECTORY, m_strImgDir);
}


BEGIN_MESSAGE_MAP(CDialogConfig, CDialogEx)
	ON_BN_CLICKED(ID_BUTTON_EXPLORER, &CDialogConfig::OnBnClickedButtonExplorer)
	ON_BN_CLICKED(IDOK, &CDialogConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogConfig 消息处理程序


BOOL CDialogConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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

	::WritePrivateProfileString( _T( "szmapcfg" ), _T( "mapdir" ), this->m_strImgDir, _T( "szmap.ini" ) );

	CDialogEx::OnOK();
}

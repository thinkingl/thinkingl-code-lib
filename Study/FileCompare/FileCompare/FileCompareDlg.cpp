
// FileCompareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileCompare.h"
#include "FileCompareDlg.h"
#include "afxdialogex.h"

#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileCompareDlg 对话框




CFileCompareDlg::CFileCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileCompareDlg::IDD, pParent)
	, m_strSrcFilePath(_T(""))
	, m_strDstFilePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRC_FILE_PATH, m_strSrcFilePath);
	DDX_Text(pDX, IDC_EDIT_DST_FILE_PATH, m_strDstFilePath);
}

BEGIN_MESSAGE_MAP(CFileCompareDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SELECT_SRC, &CFileCompareDlg::OnBnClickedButtonSelectSrc)
	ON_BN_CLICKED(IDC_BUTTON_READ_TEST, &CFileCompareDlg::OnBnClickedButtonReadTest)
	ON_BN_CLICKED(IDC_BUTTON_COMPARE, &CFileCompareDlg::OnBnClickedButtonCompare)
	ON_BN_CLICKED(IDC_BUTTON_MERGE, &CFileCompareDlg::OnBnClickedButtonMerge)
	ON_BN_CLICKED(IDC_BUTTON_SPLIT, &CFileCompareDlg::OnBnClickedButtonSplit)
END_MESSAGE_MAP()


// CFileCompareDlg 消息处理程序

BOOL CFileCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileCompareDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileCompareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileCompareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileCompareDlg::OnBnClickedButtonSelectSrc()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg( TRUE );
	if( IDOK == dlg.DoModal() )
	{
		CString strPath = dlg.GetPathName();

		this->m_strSrcFilePath = strPath;

		this->UpdateData( FALSE );
	}
}


void CFileCompareDlg::OnBnClickedButtonReadTest()
{
	this->UpdateData( );
	// TODO: 在此添加控件通知处理程序代码
	CFile fReader;
	fReader.Open( this->m_strSrcFilePath, CFile::modeRead | CFile::shareDenyWrite );

	if( fReader.m_hFile != CFile::hFileNull )
	{
		ULONGLONG nFileLen = fReader.GetLength();
		ULONGLONG nTotalLen = 0;
		while( 1 )
		{
			int nReadLen = 0;
			char buffer[100];
			try
			{
				nReadLen = fReader.Read( buffer, 10 );
			}
			catch( ... )
			{
				MessageBox( _T( "异常！" ) );
				break;
			}

			if( nReadLen == 0 )
			{
				break;
			}
			nTotalLen += nReadLen;
		}

		std::wstringstream ssMsg;
		ssMsg << _T( "file len: " ) << nFileLen << _T( " readlen: " ) << nTotalLen;

		MessageBox( ssMsg.str().c_str() );
	}
}


void CFileCompareDlg::OnBnClickedButtonCompare()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	CFile fSrc,fDst;
	fSrc.Open( this->m_strSrcFilePath, CFile::modeRead | CFile::shareDenyWrite );
	if( CFile::hFileNull == fSrc.m_hFile )
	{
		MessageBox( _T( "Open src file fail!" ) );
		return;
	}

	fDst.Open( this->m_strDstFilePath, CFile::modeRead | CFile::shareDenyWrite );
	if( CFile::hFileNull == fDst.m_hFile )
	{
		MessageBox( _T( "Open dst file fail!" ) );
		return;
	}

	if( fSrc.GetLength() != fDst.GetLength() )
	{
		MessageBox( _T( "File length not right!!" ) );
		return;
	}

	ULONGLONG fileLen = fSrc.GetLength();
	ULONGLONG compareLen = 0;
	while( compareLen < fileLen )
	{
		const int BUFFER_SIZE = 1000;
		char bufferSrc[BUFFER_SIZE] = { 0 };
		char bufferDst[BUFFER_SIZE] = { 0 };
		int nReadLenSrc = fSrc.Read( bufferSrc, sizeof( bufferSrc ) );
		int nReadLenDst = fDst.Read( bufferDst, sizeof( bufferDst ) );

		if( nReadLenSrc != nReadLenDst )
		{
			MessageBox( _T( "Read len diff！" ) );
			break;
		}

		compareLen += nReadLenSrc;

		if( memcmp( bufferSrc, bufferDst, BUFFER_SIZE ) != 0 )
		{
			MessageBox( _T( "data diff！" ) );
			break;
		}
	}

	MessageBox( _T( "two file is same!!!" ) );
}


void CFileCompareDlg::OnBnClickedButtonMerge()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	CFile fSrc,fDst;
	fSrc.Open( this->m_strSrcFilePath, CFile::modeRead | CFile::shareDenyWrite );
	if( CFile::hFileNull == fSrc.m_hFile )
	{
		MessageBox( _T( "Open src file fail!" ) );
		return;
	}

	fDst.Open( this->m_strDstFilePath, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyWrite );
	if( CFile::hFileNull == fDst.m_hFile )
	{
		MessageBox( _T( "Open dst file fail!" ) );
		return;
	}

	ULONGLONG fileLenSrc = fSrc.GetLength();
	ULONGLONG fileLenDst = fDst.GetLength();

	if( fileLenDst < fileLenSrc )
	{
		fSrc.Seek( fileLenDst, CFile::begin );
		fDst.Seek( 0, CFile::end );
	}

	while( fileLenDst < fileLenSrc )
	{
		const int BUFFER_SIZE = 1000;
		char buffer[BUFFER_SIZE] = { 0 };

		int nReadLenSrc = fSrc.Read( buffer, sizeof( buffer ) );

		fDst.Write( buffer, nReadLenSrc );
		
		fileLenDst += nReadLenSrc;
	}

	MessageBox( _T( "Merge Success!!!" ) );
}


void CFileCompareDlg::OnBnClickedButtonSplit()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	CFile fSrc,fDst;
	fSrc.Open( this->m_strSrcFilePath,
		CFile::modeReadWrite | CFile::modeNoTruncate |
		CFile::shareDenyRead | CFile::shareDenyWrite );
	if( CFile::hFileNull == fSrc.m_hFile )
	{
		MessageBox( _T( "Open src file fail!" ) );
		return;
	}

	const ULONGLONG dvdLen = 4700805020;// 4500 * 1024 * 1024;
	ULONGLONG flen = fSrc.GetLength();
	fSrc.Seek( dvdLen, CFile::begin );
	ULONGLONG curPos = fSrc.GetPosition();
	if ( curPos < dvdLen   )
	{
		MessageBox( _T( "File is not biger then dvd!" ) );
		return;
	}
	else
	{
		// 读取到另外一个文件...
		CString strBkFileName = m_strSrcFilePath + ".bk";
		fDst.Open( strBkFileName, 
			CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone );
		
		if( CFile::hFileNull == fDst.m_hFile )
		{
			int adfsdf = ::GetLastError();
			MessageBox( _T( "Open dst file fail!" ) );
			return;
		}

		char dataBuf[1000];
		while( 1 )
		{
			int nRead = fSrc.Read( dataBuf, sizeof( dataBuf ) );
			fDst.Write( dataBuf, nRead );
			if ( nRead <= 0 )
			{
				break;
			}
		}
		fDst.Close();

		fSrc.Seek( dvdLen-1, CFile::begin );

		fSrc.SetLength( dvdLen );
		
	}
	fSrc.Close();

}


// SZMapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SZMap.h"
#include "SZMapDlg.h"
#include "afxdialogex.h"
#include "DialogConfig.h"

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


// CSZMapDlg 对话框




CSZMapDlg::CSZMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSZMapDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSZMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SLIDER_Z_LEVLE, m_sliderMapZlevel);
}

BEGIN_MESSAGE_MAP(CSZMapDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_Z_LEVLE, &CSZMapDlg::OnTRBNThumbPosChangingSliderZLevle)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_Z_LEVLE, &CSZMapDlg::OnNMReleasedcaptureSliderZLevle)
	ON_BN_CLICKED(IDC_BUTTON_BACK_SHANGHAI, &CSZMapDlg::OnBnClickedButtonBackShanghai)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SLIDER_Z_LEVLE, &CSZMapDlg::OnNMThemeChangedSliderZLevle)
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CSZMapDlg::OnBnClickedButtonConfig)
END_MESSAGE_MAP()


// CSZMapDlg 消息处理程序

BOOL CSZMapDlg::OnInitDialog()
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

	ShowWindow(SW_MAXIMIZE);

	// TODO: 在此添加额外的初始化代码
	CRect rcMapCtrl( 0,0,10,10 );
	

	BOOL bMapCreate = this->m_mapCtrl.Create( NULL, _T( "map control" ), WS_CHILD, rcMapCtrl, this, 10022 );
	_ASSERT( bMapCreate );
	if( bMapCreate )
	{
		this->m_mapCtrl.ShowWindow( SW_SHOW );
	}

	this->m_sliderMapZlevel.SetRange( MIN_MAP_ZLEVEL, MAX_MAP_ZLEVEL );
	this->m_sliderMapZlevel.SetPos( 6 );

	// Latitude=31.233353, Longitude=121.502609
	this->OnBnClickedButtonBackShanghai();

	this->Updatelayer();

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSZMapDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSZMapDlg::OnPaint()
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
HCURSOR CSZMapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSZMapDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	this->m_mapCtrl.DestroyWindow();
}


void CSZMapDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);


	// TODO: 在此处添加消息处理程序代码
	this->Updatelayer( &CRect( 0,0, cx, cy ) );
}

void CSZMapDlg::Updatelayer( CRect *prcClient/* = NULL*/ )
{
	if( !this->GetSafeHwnd() )
	{
		return;
	}

	CRect rcClientLeft;
	if( prcClient )
	{
		rcClientLeft = *prcClient;
	}
	else
	{
		this->GetClientRect( rcClientLeft );
	}

	// 上边预留。
	rcClientLeft.top = 30;

	// 地图缩放控件。
	if( this->m_sliderMapZlevel.GetSafeHwnd() )
	{
		CRect rcMapZLevelCtrl;
		this->m_sliderMapZlevel.GetWindowRect( rcMapZLevelCtrl );

		CRect rcNewMapZLevelCtrl = rcClientLeft;
		rcNewMapZLevelCtrl.right = rcNewMapZLevelCtrl.left + rcMapZLevelCtrl.Width();

		this->m_sliderMapZlevel.MoveWindow( rcNewMapZLevelCtrl );

		rcClientLeft.left = rcNewMapZLevelCtrl.right + 20;
	}

	// 地图控件充满剩余区域。
	if( this->m_mapCtrl.GetSafeHwnd() )
	{
		CRect rcMapCtrl = rcClientLeft;
		
		rcMapCtrl.DeflateRect( 5,5,5,5 );

		this->m_mapCtrl.MoveWindow( rcMapCtrl );
	}
}


void CSZMapDlg::OnTRBNThumbPosChangingSliderZLevle(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CSZMapDlg::OnNMReleasedcaptureSliderZLevle(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int nZLevel = this->m_sliderMapZlevel.GetPos();
	this->m_mapCtrl.SetZLevel( nZLevel );
}


void CSZMapDlg::OnBnClickedButtonBackShanghai()
{
	// TODO: 在此添加控件通知处理程序代码
	CCoord shanghai;
	shanghai.SetZLevel( 6 );
	shanghai.SetLatitude( 31.233353 );
	shanghai.SetLongitude( 121.502609 );
	this->m_mapCtrl.Move2Center( shanghai );
}


void CSZMapDlg::OnNMThemeChangedSliderZLevle(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


BOOL CSZMapDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CSZMapDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);

	int nZLevel = this->m_sliderMapZlevel.GetPos();

	this->m_mapCtrl.SetZLevel( nZLevel );
}


void CSZMapDlg::OnBnClickedButtonConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogConfig dlg;
	dlg.DoModal();
}

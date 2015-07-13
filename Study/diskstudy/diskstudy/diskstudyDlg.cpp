
// diskstudyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "diskstudy.h"
#include "diskstudyDlg.h"
#include "afxdialogex.h"

#include "DiskHelper.h"
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


// CDiskStudyDlg 对话框



CDiskStudyDlg::CDiskStudyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiskStudyDlg::IDD, pParent)
	, m_curPhysicalDriveName(_T(""))
	, m_strDiskInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDiskStudyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOGICAL_VOL_LIST, m_combLogicalVolList);
	DDX_Text(pDX, IDC_EDIT_PHYSICAL_DRIVE, m_curPhysicalDriveName);
	DDX_Text(pDX, IDC_EDIT_DISK_INFO, m_strDiskInfo);
}

BEGIN_MESSAGE_MAP(CDiskStudyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDiskStudyDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_LOGICAL_VOL_LIST, &CDiskStudyDlg::OnCbnSelchangeComboLogicalVolList)
END_MESSAGE_MAP()


// CDiskStudyDlg 消息处理程序

BOOL CDiskStudyDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	// 获取磁盘列表, 填充到列表中.
	CLogicalVolList volList;
	::GetLogicalVolList(volList);
	for (size_t i = 0; i < volList.size(); ++i)
	{
		m_combLogicalVolList.AddString(volList[i].c_str());
	}
	m_combLogicalVolList.SetCurSel(0);

	// 更新一下.
	OnCbnSelchangeComboLogicalVolList();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDiskStudyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDiskStudyDlg::OnPaint()
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
HCURSOR CDiskStudyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDiskStudyDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDiskStudyDlg::OnCbnSelchangeComboLogicalVolList()
{
	// TODO:  在此添加控件通知处理程序代码

	int curSel = m_combLogicalVolList.GetCurSel();

	CString curVol;
	m_combLogicalVolList.GetLBText(curSel, curVol);

	int physicalNum = ::GetPhysicalDriveNumberByLogicalVolName((LPCTSTR)curVol);
	if (physicalNum != -1)
	{
		m_curPhysicalDriveName = GetPhysicalDriveName(physicalNum).c_str();
	}
	else
	{
		m_curPhysicalDriveName = L"不是硬盘!";
	}

	UpdateData(FALSE);

	this->UpdateDiskInfo();
}

void CDiskStudyDlg::UpdateDiskInfo()
{
	TDiskInfo diskInfo;
	bool bOk = ::GetDiskInfo((LPCTSTR)m_curPhysicalDriveName, diskInfo);

	if (!bOk)
	{
		m_strDiskInfo = L"获取磁盘信息失败!";
		UpdateData(FALSE);
		return;
	}

	std::wstringstream ss;
	ss << L"磁盘大小:\t" << (long long)diskInfo.m_diskSize << L"\r\n";
	ss << L"柱面数(Cylinders):\t" << diskInfo.m_cylinders << "\r\n";
	ss << L"每磁道扇区数(Sectors Per Track):\t" << diskInfo.m_sectorsPerTrack << "\r\n";
	ss << L"扇区大小(Bytes Per Sector):\t" << diskInfo.m_bytesPerSector << "\r\n";

	ss << L"媒体类型:\t";
	switch (diskInfo.m_mediaType)
	{
	case DiskMediaTypeFixHardDisk:
		ss << L"固定硬盘\r\n";
		break;
	case DiskMediaTypeRemovableMedia:
		ss << L"可移动硬盘\r\n";
		break;
	default:
		ss << L"未知的类型:\t" << diskInfo.m_mediaTypeRaw << "\r\n";
		break;
	}

	// SMART Version信息.
	ss << L"IDE Devcie Map Raw(IDE信息原始值):\t" << (void*)diskInfo.bIDEDeviceMap << "\r\n";
	ss << L"Capabilities Raw(能力集原始值):\t" << (void*)diskInfo.fCapabilities << "\r\n";

	m_strDiskInfo = ss.str().c_str();

	UpdateData(FALSE);
}

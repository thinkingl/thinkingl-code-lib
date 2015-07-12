
// LDAPDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LDAPDemo.h"
#include "LDAPDemoDlg.h"
#include "afxdialogex.h"

#include <winldap.h>

#pragma comment(lib, "wldap32.lib")

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


// CLDAPDemoDlg 对话框



CLDAPDemoDlg::CLDAPDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLDAPDemoDlg::IDD, pParent)
	, m_domainName(_T(""))
	, m_userName(_T(""))
	, m_password(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLDAPDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DOMAIN, m_domainName);
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_userName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
}

BEGIN_MESSAGE_MAP(CLDAPDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLDAPDemoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLDAPDemoDlg 消息处理程序

BOOL CLDAPDemoDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLDAPDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLDAPDemoDlg::OnPaint()
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
HCURSOR CLDAPDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLDAPDemoDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	this->UpdateData();

	if ( m_userName.IsEmpty() )
	{
		MessageBox(L"用户名不能为空!");
		return;
	}
	if ( m_domainName.IsEmpty() )
	{
		MessageBox(L"域名不能为空!");
		return;
	}

	LDAP*  pLdap = ldap_init( const_cast<LPTSTR>( (LPCTSTR)m_domainName ), LDAP_PORT);

	if ( NULL == pLdap )
	{
		ULONG er = LdapGetLastError();
		MessageBox(L"初始化失败!");

		return;
	}

	// bind,验证.
	if ( m_userName.Find( '\\' ) == -1 )
	{
		m_userName = m_domainName + "\\" + m_userName;
	}
	ULONG bindRet = ldap_simple_bind_s(pLdap, const_cast<LPTSTR>((LPCTSTR)m_userName), const_cast<LPTSTR>((LPCTSTR)m_password));
	if ( bindRet != LDAP_SUCCESS)
	{
		LDAP_INVALID_CREDENTIALS;
		MessageBox(L"认证失败!");
		ldap_unbind(pLdap);
		return;
	}

	// 连接.
	LDAP_TIMEVAL timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	ULONG connectRet = ldap_connect(pLdap, &timeout);
	if ( connectRet != LDAP_SUCCESS )
	{
		MessageBox(L"连接服务器失败!");
		ldap_unbind(pLdap);
		return;
	}

	// 打开CU客户端.
	CString cuPath;

	LPCTSTR cuPath1 = _T("C:\\Program Files (x86)\\kedacom\\CU2.0\\cu.exe");
	if ( _waccess(cuPath1,0) == 0 )
	{
		cuPath = cuPath1;
	}

	LPCTSTR cuPath2 = _T("C:\\Program Files\\kedacom\\CU2.0\\cu.exe");
	if (_waccess(cuPath2, 0) == 0)
	{
		cuPath = cuPath2;
	}

	if ( cuPath.IsEmpty() )
	{
		MessageBox(L"没有找到监控客户端!");
		ldap_unbind(pLdap);
		return;
	}

	// 打开监控客户端.
	// 内置CU用户名密码.
	CString cmd;

	ShellExecute(
		NULL,
		(L"open"),
		//NULL,
		cuPath,
		cmd,
		NULL,
		SW_SHOWDEFAULT);


	ldap_unbind(pLdap);
}


// Win32ErrorCodeShowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Win32ErrorCodeShow.h"
#include "Win32ErrorCodeShowDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CWin32ErrorCodeShowDlg �Ի���




CWin32ErrorCodeShowDlg::CWin32ErrorCodeShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWin32ErrorCodeShowDlg::IDD, pParent)
	, m_dwErrorCode(0)
	, m_strErrorMsg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWin32ErrorCodeShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ERROR_CODE, m_dwErrorCode);
	DDX_Text(pDX, IDC_EDIT_ERROR_MSG, m_strErrorMsg);
}

BEGIN_MESSAGE_MAP(CWin32ErrorCodeShowDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BTN_LOOK_UP, &CWin32ErrorCodeShowDlg::OnBnClickedBtnLookUp)
END_MESSAGE_MAP()


// CWin32ErrorCodeShowDlg ��Ϣ�������

BOOL CWin32ErrorCodeShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWin32ErrorCodeShowDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWin32ErrorCodeShowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWin32ErrorCodeShowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWin32ErrorCodeShowDlg::OnBnClickedBtnLookUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData( TRUE );

	const int MAX_ERROR_MSG_LEN = 10 * 1024;
	TCHAR szErrorMsg[ MAX_ERROR_MSG_LEN ] = { 0 };
	BOOL fOk = ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, this->m_dwErrorCode,
		NULL, szErrorMsg, MAX_ERROR_MSG_LEN, NULL );

	if (!fOk) 
	{
	   // Is it a network-related error?
	   HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL,
		  DONT_RESOLVE_DLL_REFERENCES);

		if (hDll != NULL) 
		{
			fOk = FormatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
				hDll, this->m_dwErrorCode, NULL,
				szErrorMsg, MAX_ERROR_MSG_LEN, NULL);
			FreeLibrary(hDll);
		}
	}

	
	if( fOk )
		this->m_strErrorMsg = szErrorMsg;
	else
		this->m_strErrorMsg = _T( " �� ��û�ҵ���" );

	this->UpdateData( FALSE );

}

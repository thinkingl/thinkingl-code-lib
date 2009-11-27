// ThinkVPNClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ThinkVPNClient.h"
#include "ThinkVPNClientDlg.h"

#include "ThinkVPNCommonLib.h"

#include <iostream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CThinkVPNClientDlg �Ի���




CThinkVPNClientDlg::CThinkVPNClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThinkVPNClientDlg::IDD, pParent)
    , m_dwPID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThinkVPNClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PID, m_dwPID);
}

BEGIN_MESSAGE_MAP(CThinkVPNClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(ID_BTN_INJECT, &CThinkVPNClientDlg::OnBnClickedBtnInject)
    ON_BN_CLICKED(ID_BTN_MESSAGEBOX, &CThinkVPNClientDlg::OnBnClickedBtnMessagebox)
END_MESSAGE_MAP()


// CThinkVPNClientDlg ��Ϣ�������

BOOL CThinkVPNClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
   // this->m_dwPID = 

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CThinkVPNClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CThinkVPNClientDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CThinkVPNClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CThinkVPNClientDlg::OnBnClickedBtnInject()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData( TRUE );

	if ( m_dwPID == NULL )
	{
		PROCESS_INFORMATION tPinfo;
		memset( &tPinfo, 0, sizeof( tPinfo ) );
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);


		LPCTSTR strCmdLine = _T( "D:\\view\\view.exe" );
		TCHAR szCmdLine[MAX_PATH] = {0};
		_tcscpy( szCmdLine, strCmdLine );
		BOOL bResult = CreateProcess( NULL, szCmdLine, NULL, NULL, NULL, NULL, NULL, NULL, 
			&si,
			&tPinfo );

		if ( !bResult )
		{
			DWORD dwEr = GetLastError();
			MessageBox( _T( "CreateProcess Fail!" ) );
			return;
		}

		m_dwPID = tPinfo.dwProcessId;
	}
	
	// Vista ��Ҫ�����Ȩ�޲���CreateRemoteThread�ɹ���
    DWORD dwDesireAccess = PROCESS_ALL_ACCESS;//PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE;
    HANDLE hProcess = ::OpenProcess( dwDesireAccess, FALSE, m_dwPID );

    tstring strAppDir = GetAppDir();
    
    vpn::log.SetLogFileDir( strAppDir.c_str(), _T( "ThinkVPNClient" ) );

    if ( NULL == hProcess )
    {
        vpn::log << "open process fail! errorcode: " << GetLastError() << endl;
        MessageBox( _T( "Open Process Fail!" ) );
        return;
    }
    else
    {
        // ����dll
        LPCTSTR pszLibFile = _T( "ThinkVPNSpyDll.dll" );

        tstring strDllFilePath = GetAppDir() + pszLibFile;

        pszLibFile = strDllFilePath.c_str();

        // Calculate the number of bytes needed for the DLL's pathname
        int cch = 1 + lstrlenW(pszLibFile);
        int cb  = cch * sizeof(WCHAR);

        // Allocate space in the remote process for the pathname
        PWSTR  pszLibFileRemote = (PWSTR) 
            VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
        if (pszLibFileRemote == NULL)
        {
            return;
        }

        // Copy the DLL's pathname to the remote process's address space
        if (!WriteProcessMemory(hProcess, pszLibFileRemote, 
            (PVOID) pszLibFile, cb, NULL)) 
        {
			MessageBox( _T( "WriteProcessMemory Fail!" ) );
            return;
        }

        // Get the real address of LoadLibraryW in Kernel32.dll
        PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
            GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
        if (pfnThreadRtn == NULL) 
        {
			MessageBox( _T( "Get LoadLibraryW addr Fail!" ) );
            return;
        }

        // Create a remote thread that calls LoadLibraryW(DLLPathname)
        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, 
            pfnThreadRtn, pszLibFileRemote, 0, NULL);
        if (hThread == NULL) 
        {
			DWORD dwEr = GetLastError();
			MessageBox( _T( "CreateRemoteThread Fail!" ) );
            return;
        }

        // Wait for the remote thread to terminate
        WaitForSingleObject(hThread, INFINITE);

//        fOk = TRUE; // Everything executed successfully


        // Now, we can clean everthing up

        // Free the remote memory that contained the DLL's pathname
        if (pszLibFileRemote != NULL) 
            VirtualFreeEx(hProcess, pszLibFileRemote, 0, MEM_RELEASE);

        if (hThread  != NULL) 
            CloseHandle(hThread);

        if (hProcess != NULL) 
            CloseHandle(hProcess);
    
        

    }
}

void CThinkVPNClientDlg::OnBnClickedBtnMessagebox()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    ::MessageBoxW( GetSafeHwnd(), L"Test MessageBoxW", L"Test ", MB_OK );
}

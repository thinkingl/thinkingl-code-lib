
// KedaGiftDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KedaGift.h"
#include "KedaGiftDlg.h"
#include "afxdialogex.h"
#include "KedaGiftDefine.h"
#include "RadomPick.h"
#include "UnitTest.h"

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


// CKedaGiftDlg �Ի���




CKedaGiftDlg::CKedaGiftDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKedaGiftDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bEmployRefreshing = false;
}

void CKedaGiftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_EMPLOYER, m_staticEmplyer);
}

BEGIN_MESSAGE_MAP(CKedaGiftDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_START_STOP, &CKedaGiftDlg::OnBnClickedStartStop)
	ON_MESSAGE( WM_REFRESH_EMPLOY, &CKedaGiftDlg::OnEmployRefresh)
	ON_BN_CLICKED(ID_UNITTEST, &CKedaGiftDlg::OnBnClickedUnittest)
END_MESSAGE_MAP()


// CKedaGiftDlg ��Ϣ�������
void CALLBACK CKedaGiftDlg::TimerCB(HWND hWnd, UINT , UINT_PTR timerId, DWORD de)
{
	if ( CKedaGiftDlg::Timer_Refresh == timerId )
	{
		::PostMessage( hWnd, WM_REFRESH_EMPLOY, 0, 0 );
	}
	
}

LRESULT CKedaGiftDlg::OnEmployRefresh( WPARAM, LPARAM )
{
	if ( m_bEmployRefreshing )
	{
		CRadomPick rp;
		int nRandNum = rp.GetRadomNum( 0, 450 );
		CString strTmp;
		strTmp.Format( _T( "%d" ), nRandNum  );

		this->m_staticEmplyer.SetWindowText( strTmp );
	}
	return 0;
}

BOOL CKedaGiftDlg::OnInitDialog()
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
	this->SetTimer( Timer_Refresh, 1, TimerCB );

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CKedaGiftDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKedaGiftDlg::OnPaint()
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
HCURSOR CKedaGiftDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CKedaGiftDlg::OnBnClickedStartStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->m_bEmployRefreshing = !m_bEmployRefreshing;
}


void CKedaGiftDlg::OnBnClickedUnittest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CUnitTest ut;
}

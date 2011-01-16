
// KedaGiftDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KedaGift.h"
#include "KedaGiftDlg.h"
#include "afxdialogex.h"
#include "KedaGiftDefine.h"
#include "RandomPick.h"
#include "UnitTest.h"

#include "EmployerInput.h"
#include "GiftStatusView.h"
#include "FlashDialog.h"

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
	, m_strGifted(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bEmployRefreshing = false;

	CEmployerInput emIn;
//	emIn.GetAllNoGiftEmployer();
	m_radomLuckyPick.SetEmployerList( emIn.GetAllNoGiftEmployer() );
}

void CKedaGiftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_EMPLOYER, m_staticEmplyer);
	DDX_Control(pDX, ID_START_STOP, m_btnStartStop);
	DDX_Control(pDX, IDC_EDIT_GIFTED, m_editGifted);
	DDX_Text(pDX, IDC_EDIT_GIFTED, m_strGifted);
}

BEGIN_MESSAGE_MAP(CKedaGiftDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_START_STOP, &CKedaGiftDlg::OnBnClickedStartStop)
	ON_MESSAGE( WM_REFRESH_EMPLOY, &CKedaGiftDlg::OnEmployRefresh)
	ON_BN_CLICKED(ID_UNITTEST, &CKedaGiftDlg::OnBnClickedUnittest)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(ID_GIFT_STATE, &CKedaGiftDlg::OnBnClickedGiftState)
	ON_BN_CLICKED(ID_NEXT_TURN, &CKedaGiftDlg::OnBnClickedNextTurn)
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_FLASH, &CKedaGiftDlg::OnBnClickedFlash)
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
		CEmployer randomShowEmployer = this->m_radomLuckyPick.RandomPickOneNoGiftToShow();

		this->ShowAMan( randomShowEmployer );
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
//	this->SetTimer( Timer_Refresh, 1, TimerCB );

	this->m_fontShowName.CreateFont(
		72,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial"));                 // lpszFacename


	this->m_staticEmplyer.SetFont( &m_fontShowName );

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

	if ( !m_bEmployRefreshing )
	{
		// ѡһ�����н�.
		CEmployer luckyOne = this->m_radomLuckyPick.GetLukyOne();
		this->ShowAMan( luckyOne );
		
		this->m_strGifted += this->GetShowText( luckyOne );
		m_strGifted += "\r\n";
		this->UpdateData( FALSE );
	}

	this->m_btnStartStop.SetWindowText( m_bEmployRefreshing ? _T( "ֹͣ" ) : _T( "��ʼ" ) );
}


void CKedaGiftDlg::OnBnClickedUnittest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CUnitTest ut;
}

void CKedaGiftDlg::ShowAMan( const CEmployer& oneMan )
{
	CString strText = this->GetShowText( oneMan );	
	this->m_staticEmplyer.SetWindowText( strText );
}

CString CKedaGiftDlg::GetShowText( const CEmployer& oneMan )
{
	CString strTmp;
	strTmp.Format( _T( "%s  %s" ), oneMan.m_strKedaNo, oneMan.m_strName );
	return strTmp;
}

void CKedaGiftDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->OnBnClickedStartStop();

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CKedaGiftDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);

	this->OnBnClickedStartStop();

}


void CKedaGiftDlg::OnBnClickedGiftState()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CGiftStatusView dlg;
	dlg.SetRandomPicker( &m_radomLuckyPick );
	dlg.DoModal();

	CEmployerGiftConfig config;
	TEmployerList tAllLucky = config.GetLuckies();
	if ( tAllLucky.empty() )
	{
		// ������н�����.
		this->m_staticEmplyer.SetWindowText( "KEDACOM" );

		this->m_editGifted.SetWindowText( "" );
	}
	this->m_radomLuckyPick;
}


void CKedaGiftDlg::OnBnClickedNextTurn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->m_strGifted = "";
	this->m_staticEmplyer.SetWindowText( "KEDACOM" );
	this->UpdateData( FALSE );

}


void CKedaGiftDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if ( this->GetSafeHwnd() && m_staticEmplyer.GetSafeHwnd() )
	{

	}
	// TODO: �ڴ˴������Ϣ����������
}


void CKedaGiftDlg::OnBnClickedFlash()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFlashDialog dlg( this, &this->m_radomLuckyPick );
	dlg.DoModal();
}

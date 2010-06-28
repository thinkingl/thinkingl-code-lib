
// Socks5DemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Socks5Demo.h"
#include "Socks5DemoDlg.h"

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


// CSocks5DemoDlg �Ի���




CSocks5DemoDlg::CSocks5DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocks5DemoDlg::IDD, pParent)
	, m_strProxyIp(_T("127.0.0.1"))
	, m_nProxyPort( 2001 )
	, m_strConnectMsg(_T(""))
	, m_strAssociateStat(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	this->m_hProxyControl = INVALID_SOCKET;

}

void CSocks5DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_strProxyIp);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nProxyPort);
	DDX_Text(pDX, IDC_EDIT_STATE, m_strConnectMsg);
	DDX_Text(pDX, IDC_EDIT_ASSOCIATE_STAT, m_strAssociateStat);
}

BEGIN_MESSAGE_MAP(CSocks5DemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSocks5DemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_UDP_ASSOCIATE, &CSocks5DemoDlg::OnBnClickedButtonUdpAssociate)
END_MESSAGE_MAP()


// CSocks5DemoDlg ��Ϣ�������

BOOL CSocks5DemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

void CSocks5DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSocks5DemoDlg::OnPaint()
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
HCURSOR CSocks5DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSocks5DemoDlg::OnBnClickedButton1()
{
	UpdateData();

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( INVALID_SOCKET != m_hProxyControl )
	{
		// �Ͽ����ӡ�
		this->Release();
		this->ShowConnectMsg( "δ����" );
	}
	else
	{
		// ����socket��
		m_hProxyControl = socket( AF_INET, SOCK_STREAM, 0 );
		if( m_hProxyControl == INVALID_SOCKET )
		{
			ShowConnectMsg( _T( "create socket failed!" ) );
			return ;
		}

		// ���ӡ�
		UINT nProxyIp = inet_addr( m_strProxyIp );

		SOCKADDR_IN tSvrINAddr;
		memset( &tSvrINAddr, 0, sizeof(tSvrINAddr) );
		tSvrINAddr.sin_family = AF_INET;
		tSvrINAddr.sin_port = htons( m_nProxyPort );
		tSvrINAddr.sin_addr.s_addr = nProxyIp;

		if( connect( m_hProxyControl, (SOCKADDR*)&tSvrINAddr, sizeof(tSvrINAddr) ) == SOCKET_ERROR )
		{
			ShowConnectMsg( _T( "connect proxy fail!" ) );
			this->Release();
			return;
		}

		// ���Ͱ汾/����У�����
		/** ��ʽ
			+----+----------+----------+
			|VER | NMETHODS | METHODS  |
			+----+----------+----------+
			| 1  |    1     | 1 to 255 |
			+----+----------+----------+
		*/

		const int BUF_SIZE = 1024;

		//sock5����汾��,��Ȼ��5�ˡ�
		const int SOCK5_PROXY_VERSION =	0x05;
		
		// ���������֤У�鷽ʽ��	
		#define METHOD_AUTH_NO		0x00	// ����У�顣
		#define METHOD_AUTH_NAMEPWD	0x02	// �û���/���롣
		#define METHOD_GSSAPI		0x01	// �������п���google�ɡ�
		#define METHOD_NO_ACCEPT	0xff	// ɶ�������ܣ���˼��ž���û�š�

		const int METHOD_NUM = 4;

		char abyMsgVersionMethodCheck[ BUF_SIZE ] = { 0 };
		abyMsgVersionMethodCheck[0] = SOCK5_PROXY_VERSION;
		abyMsgVersionMethodCheck[1] = METHOD_NUM;
		
		abyMsgVersionMethodCheck[2] = METHOD_AUTH_NO;
		abyMsgVersionMethodCheck[3] = METHOD_AUTH_NAMEPWD;
		abyMsgVersionMethodCheck[4] = METHOD_GSSAPI;
		abyMsgVersionMethodCheck[5] = METHOD_NO_ACCEPT;


		if( SOCKET_ERROR == send( m_hProxyControl , abyMsgVersionMethodCheck,  2+METHOD_NUM, 0 ) )
		{
			this->ShowConnectMsg( "send version and method check msg fail!" );
			this->Release();			
			return;
		}

		// ���ջ�Ӧ��
		/**
			+----+--------+
			|VER | METHOD |
			+----+--------+
			| 1  |   1    |
			+----+--------+
			*/
		memset( abyMsgVersionMethodCheck, 0, sizeof( abyMsgVersionMethodCheck ) );
		if ( 2 != recv( m_hProxyControl, abyMsgVersionMethodCheck, BUF_SIZE, 0 ) )
		{
			ShowConnectMsg( "Receive version and method fail!!" );
			this->Release();
			return;
		}

		// У����յ������ݡ�
		if ( abyMsgVersionMethodCheck[0] != SOCK5_PROXY_VERSION )
		{
			ShowConnectMsg( "Version wrong " );
			this->Release();
			return;
		}

		switch( abyMsgVersionMethodCheck[1] )
		{
		case METHOD_AUTH_NO:
			break;
		case METHOD_AUTH_NAMEPWD:
			this->ShowConnectMsg( "ʱ�䲻����ûʵ���û���������֤��@_@" );
			this->MessageBox( "@_@ ûʱ�����û���������֤��" );
			this->Release();
			return;
			break;
		case METHOD_GSSAPI:
		case METHOD_NO_ACCEPT:		    
		default:
			this->ShowConnectMsg( "Not surport method!" );
			this->Release();
			return;
		    break;
		}

		// ok��
		this->ShowConnectMsg( "Connect success!" );
		
	}


}

void CSocks5DemoDlg::ShowConnectMsg( LPCTSTR strMsg )
{
	this->m_strConnectMsg = strMsg;
	UpdateData( FALSE );
}

void CSocks5DemoDlg::Release()
{
	if ( INVALID_SOCKET != m_hProxyControl )
	{
		closesocket( m_hProxyControl );
		m_hProxyControl = INVALID_SOCKET;
	}
}
void CSocks5DemoDlg::OnBnClickedButtonUdpAssociate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

#if 0 

	API BOOL32 OspUdpAssociateThroughSock5Proxy( SOCKHANDLE hSocket , u32 dwLocalIP, u16 wLocalPort ,
		u32* pdwProxyMapIP, u16* pwProxyMapPort , u32 dwTimeoutMs )
	{
		SOCKADDR_IN tSvrINAddr;
		struct timeval tTimeVal;
		struct timeval *ptTimeVal = NULL;
		u8 abyMsgBuffer[0xff] = {0};
		fd_set tWaitFd;
		u32 dwProxyMapIP = 0;
		u16 wProxyMapPort = 0;
		s32 nRet = 0;
#ifdef _VXWORKS_
		char StringIPAddr[INET_ADDR_LEN];
#endif

		memset( &tSvrINAddr, 0, sizeof(tSvrINAddr) );
		tSvrINAddr.sin_family = AF_INET;
		tSvrINAddr.sin_port = htons(wLocalPort);
		tSvrINAddr.sin_addr.s_addr = dwLocalIP;

#ifdef _VXWORKS_
		inet_ntoa_b( tSvrINAddr.sin_addr, StringIPAddr);
		OspLog( 1, "Osp: OspUdpAssociateThroughSock5Proxy from local %s@%d, please wait...!\n",
			StringIPAddr, wLocalPort );
#else
		OspLog( 1, "Osp: OspUdpAssociateThroughSock5Proxy from local %s@%d, please wait...\n",
			inet_ntoa( tSvrINAddr.sin_addr ), wLocalPort );
#endif

		//����UDP Associate����
		abyMsgBuffer[0] = SOCK5_PROXY_VERSION;
		abyMsgBuffer[1] = SOCK5_PROXY_CMD_UDP_ASSOCIATE;
		abyMsgBuffer[2] = SOCK5_PROXY_RESERVED_DATA;
		abyMsgBuffer[3] = SOCK5_PROXY_IPV4_ADDR;
		memcpy( abyMsgBuffer+4 , &dwLocalIP , 4 );
		wLocalPort = htons(wLocalPort);
		memcpy( abyMsgBuffer+8 , &wLocalPort , 2 );
		if( SOCKET_ERROR == send( hSocket , (char*)abyMsgBuffer , 10 , 0 ) )
		{
			OspLog(1, "Osp: OspUdpAssociateThroughSock5Proxy() send udp associate request failed!\n");
			return FALSE;
		}

		//�ȴ���������ظ�
		if( dwTimeoutMs > 0 )
		{
			memset(&tTimeVal, 0, sizeof(tTimeVal));
			tTimeVal.tv_sec = dwTimeoutMs/1000;
			tTimeVal.tv_usec = (dwTimeoutMs%1000)*1000;

			ptTimeVal = &tTimeVal;
		}
		FD_ZERO(&tWaitFd);
		FD_SET( hSocket , &tWaitFd );
		nRet = select(FD_SETSIZE, &tWaitFd, NULL, NULL, ptTimeVal);
		if( 0 >= nRet )
		{
			OspLog(1, "Osp: OspUdpAssociateThroughSock5Proxy() recv udp associate reply failed! nRet =%d\n",nRet);
#ifdef _MSC_VER
			if(nRet < 0)
			{
				OspLog(1,"Osp: OspUdpAssociateThroughSock5Proxy select failed! WSAGetLastError = %d\n", WSAGetLastError);
			}
#else
			if(nRet < 0)
			{
				OspLog(1,"Osp: OspUdpAssociateThroughSock5Proxy select failed! errno = %d\n", errno);
			}

#endif   
			return FALSE;
		}
		if( ( 10 != recv( hSocket, (char *)abyMsgBuffer, 0xff, 0 ) ) ||
			( SOCK5_PROXY_VERSION != abyMsgBuffer[0] ) ||
			( SOCK5_PROXY_SUCCESS != abyMsgBuffer[1] ) ||
			( SOCK5_PROXY_RESERVED_DATA != abyMsgBuffer[2] ) ||
			( SOCK5_PROXY_IPV4_ADDR != abyMsgBuffer[3] ) )
		{
			OspLog(1, "Osp: OspUdpAssociateThroughSock5Proxy() udp associate failed!\n");
			return FALSE;
		}

		memcpy( &dwProxyMapIP , abyMsgBuffer+4 , 4 );
		memcpy( &wProxyMapPort , abyMsgBuffer+8 , 2 );
		wProxyMapPort = ntohs(wProxyMapPort);

		memset( &tSvrINAddr, 0, sizeof(tSvrINAddr) );
		tSvrINAddr.sin_family = AF_INET;
		tSvrINAddr.sin_port = wProxyMapPort;
		tSvrINAddr.sin_addr.s_addr = dwProxyMapIP;
#ifdef _VXWORKS_
		inet_ntoa_b( tSvrINAddr.sin_addr, StringIPAddr );
		OspLog(1, "Osp: OspUdpAssociateThroughSock5Proxy to %s@%d OK!\n\n",
			StringIPAddr, wProxyMapPort );
#else
		OspLog(1, "Osp: OspUdpAssociateThroughSock5Proxy to %s@%d OK!\n\n",
			inet_ntoa( tSvrINAddr.sin_addr ), wProxyMapPort );
#endif

		if( NULL != pdwProxyMapIP )
		{
			*pdwProxyMapIP = dwProxyMapIP;
		}
		if( NULL != pwProxyMapPort )
		{
			*pwProxyMapPort = wProxyMapPort;
		}

		return TRUE;
	}

#endif
}


// Socks5DemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Socks5Demo.h"
#include "Socks5DemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSocks5DemoDlg 对话框




CSocks5DemoDlg::CSocks5DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocks5DemoDlg::IDD, pParent)
	, m_strProxyIp(_T("172.16.65.94"))
	, m_nProxyPort( 2001 )
	, m_strConnectMsg(_T(""))
	, m_strAssociateStat(_T(""))
	, m_strIPProxyReply(_T(""))
	, m_sPortProxyReply(0)
	, m_strLocalIp(_T("127.0.0.1"))
	, m_nPortReq( 50128 )
	, m_strMsgSentToRemote(_T("Hello!How are you?"))
	, m_strRemoteIp(_T("192.168.1.84"))
	, m_nRemotePort( 12345 )
	, m_nLocalInnerBindPort(0)
	, m_strClientRcvMsg(_T(""))
	, m_nRemoteBindPort( 12345 )
	, m_strRemoteSentMsg(_T("Fine,3ku."))
	, m_strRemoteServerRcvMsg(_T(""))
	, m_strRemoteProxyIp(_T(""))
	, m_nRemoteProxyPort(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	this->m_hProxyControl = INVALID_SOCKET;
	m_hUDPProxyClient = INVALID_SOCKET;
	m_dwTimerCheckSendInUDP = NULL;
	m_hUDPRemoteServer = INVALID_SOCKET;

}

void CSocks5DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_strProxyIp);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nProxyPort);
	DDX_Text(pDX, IDC_EDIT_STATE, m_strConnectMsg);
	DDX_Text(pDX, IDC_EDIT_ASSOCIATE_STAT, m_strAssociateStat);
	DDX_Text(pDX, IDC_EDIT_IP_PROXY_REPLY, m_strIPProxyReply);
	DDX_Text(pDX, IDC_EDIT_PORT_REPLY, m_sPortProxyReply);
	DDX_Text(pDX, IDC_EDIT_IP_LOCAL, m_strLocalIp);
	DDX_Text(pDX, IDC_EDIT_PORT_WANT, m_nPortReq);
	DDX_Text(pDX, IDC_EDIT_MSG_SENT_TO_REMOTE, m_strMsgSentToRemote);
	DDX_Text(pDX, IDC_EDIT_IP_REMOTE, m_strRemoteIp);
	DDX_Text(pDX, IDC_EDIT_PORT_REMOTE, m_nRemotePort);
	DDX_Text(pDX, IDC_EDIT_PORT_LOCAL_IN_BIND, m_nLocalInnerBindPort);
	DDX_Text(pDX, IDC_EDIT_UDP_CLIENT_RCV_MSG, m_strClientRcvMsg);
	DDX_Text(pDX, IDC_EDIT_PORT_BIND_REMOTE, m_nRemoteBindPort);
	DDX_Text(pDX, IDC_EDIT_MSG_SENT_TO_CLIENT, m_strRemoteSentMsg);
	DDX_Text(pDX, IDC_EDIT_REMOTE_SERVER_RCV_MSG, m_strRemoteServerRcvMsg);
	DDX_Text(pDX, IDC_EDIT_PROXY_IP, m_strRemoteProxyIp);
	DDX_Text(pDX, IDC_EDIT_PROXY_PORT, m_nRemoteProxyPort);
}

BEGIN_MESSAGE_MAP(CSocks5DemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSocks5DemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_UDP_ASSOCIATE, &CSocks5DemoDlg::OnBnClickedButtonUdpAssociate)
	ON_BN_CLICKED(IDC_BUTTON_SEND_UDP_OUT, &CSocks5DemoDlg::OnBnClickedButtonSendUdpOut)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SEND_BIND_REMOTE_SERVER, &CSocks5DemoDlg::OnBnClickedButtonSendBindRemoteServer)
	ON_BN_CLICKED(IDC_BUTTON_SEND_UDP_IN, &CSocks5DemoDlg::OnBnClickedButtonSendUdpIn)
END_MESSAGE_MAP()


// CSocks5DemoDlg 消息处理程序

BOOL CSocks5DemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	
	// 开启检测接收UDP数据的定时器。
	m_dwTimerCheckSendInUDP = this->SetTimer( TimerCheckRcvUDP, 1, NULL );

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSocks5DemoDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSocks5DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSocks5DemoDlg::OnBnClickedButton1()
{
	UpdateData();

	// TODO: 在此添加控件通知处理程序代码
	if ( INVALID_SOCKET != m_hProxyControl )
	{
		// 断开连接。
		this->Release();
		this->ShowConnectMsg( "未连接" );
	}
	else
	{
		// 建立socket。
		m_hProxyControl = socket( AF_INET, SOCK_STREAM, 0 );
		if( m_hProxyControl == INVALID_SOCKET )
		{
			ShowConnectMsg( _T( "create socket failed!" ) );
			return ;
		}

		// 连接。
		UINT nProxyIp = inet_addr( m_strProxyIp );

		SOCKADDR_IN tSvrINAddr;
		memset( &tSvrINAddr, 0, sizeof(tSvrINAddr) );
		tSvrINAddr.sin_family = AF_INET;
		tSvrINAddr.sin_port = htons( m_nProxyPort );
		tSvrINAddr.sin_addr.s_addr = nProxyIp;

		if( connect( m_hProxyControl, (SOCKADDR*)&tSvrINAddr, sizeof(tSvrINAddr) ) == SOCKET_ERROR )
		{
			int dfwef = GetLastError();
			ShowConnectMsg( _T( "connect proxy fail!" ) );
			this->Release();
			return;
		}

		// 发送版本/功能校验命令。
		/** 格式
			+----+----------+----------+
			|VER | NMETHODS | METHODS  |
			+----+----------+----------+
			| 1  |    1     | 1 to 255 |
			+----+----------+----------+
		*/

		const int BUF_SIZE = 1024;

		//sock5代理版本号,当然是5了。
		const int SOCK5_PROXY_VERSION =	0x05;
		
		// 几种身份认证校验方式。	
		#define METHOD_AUTH_NO		0x00	// 不用校验。
		#define METHOD_AUTH_NAMEPWD	0x02	// 用户名/密码。
//		#define METHOD_GSSAPI		0x01	// 不懂，有空再google吧。
		#define METHOD_NO_ACCEPT	0xff	// 啥都不接受，意思大概就是没门。

		const int METHOD_NUM = 4-1;

		char abyMsgVersionMethodCheck[ BUF_SIZE ] = { 0 };
		abyMsgVersionMethodCheck[0] = SOCK5_PROXY_VERSION;
		abyMsgVersionMethodCheck[1] = METHOD_NUM;
		
		abyMsgVersionMethodCheck[2] = METHOD_AUTH_NO;
		abyMsgVersionMethodCheck[3] = METHOD_AUTH_NAMEPWD;
//		abyMsgVersionMethodCheck[4] = METHOD_GSSAPI;
		abyMsgVersionMethodCheck[4] = METHOD_NO_ACCEPT;


		if( SOCKET_ERROR == send( m_hProxyControl , abyMsgVersionMethodCheck,  2+METHOD_NUM, 0 ) )
		{
			this->ShowConnectMsg( "send version and method check msg fail!" );
			this->Release();			
			return;
		}

		// 接收回应。
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

		// 校验接收到的数据。
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
			this->ShowConnectMsg( "时间不够，没实现用户名密码认证！@_@" );
			this->MessageBox( "@_@ 没时间做用户名密码认证。" );
			this->Release();
			return;
			break;
//		case METHOD_GSSAPI:
		case METHOD_NO_ACCEPT:		    
		default:
			this->ShowConnectMsg( "Not surport method!" );
			this->Release();
			return;
		    break;
		}

		// ok！
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
	// TODO: 在此添加控件通知处理程序代码

	if ( INVALID_SOCKET == this->m_hProxyControl )
	{
		this->ShowAssociateMsg( "Not connect to proxy!" );
		this->Release();
		return;
	}

	UpdateData();

	// 发送UDP 穿透信令。
	/**   
	+----+-----+-------+------+----------+----------+
	|VER | CMD |  RSV  | ATYP | DST.ADDR | DST.PORT |
	+----+-----+-------+------+----------+----------+
	| 1  |  1  | X'00' |  1   | Variable |    2     |
	+----+-----+-------+------+----------+----------+
	Where:

	o  VER    protocol version: X'05'
	o  CMD
	o  CONNECT X'01'
	o  BIND X'02'
	o  UDP ASSOCIATE X'03'
	o  RSV    RESERVED
	o  ATYP   address type of following address
	o  IP V4 address: X'01'
	o  DOMAINNAME: X'03'
	o  IP V6 address: X'04'
	o  DST.ADDR       desired destination address
	o  DST.PORT desired destination port in network octet
	order

*/
	char abyUdpAssociateBuf[1024] = { 0 };

	//sock5代理版本号,当然是5了。
	const int SOCK5_PROXY_VERSION =	0x05;
	const int CMD_UDP_ASSOCIATE = 0x03;
	const int RESERVED = 0;
	const int IP_TYPE = 0x01;	// ipv4

	int nAddr = inet_addr( m_strLocalIp );
	short nPort = htons( (short)m_nPortReq );

	abyUdpAssociateBuf[0] = SOCK5_PROXY_VERSION;
	abyUdpAssociateBuf[1] = CMD_UDP_ASSOCIATE;
	abyUdpAssociateBuf[2] = RESERVED;
	abyUdpAssociateBuf[3] = IP_TYPE;
	memcpy( &abyUdpAssociateBuf[4], &nAddr, 4 );
	memcpy( &abyUdpAssociateBuf[8], &nPort, 2 );

	if( SOCKET_ERROR == send( this->m_hProxyControl, abyUdpAssociateBuf, 10 , 0 ) )
	{
		this->ShowAssociateMsg( "Send associate msg fail!" );
		this->Release();
		return ;
	}

	// 接收回应。
	/** 
	+----+-----+-------+------+----------+----------+
	|VER | REP |  RSV  | ATYP | BND.ADDR | BND.PORT |
	+----+-----+-------+------+----------+----------+
	| 1  |  1  | X'00' |  1   | Variable |    2     |
	+----+-----+-------+------+----------+----------+

	Where:

	o  VER    protocol version: X'05'
	o  REP    Reply field:
	o  X'00' succeeded
	o  X'01' general SOCKS server failure
	o  X'02' connection not allowed by ruleset
	o  X'03' Network unreachable
	o  X'04' Host unreachable
	o  X'05' Connection refused
	o  X'06' TTL expired
	o  X'07' Command not supported
	o  X'08' Address type not supported
	o  X'09' to X'FF' unassigned
	o  RSV    RESERVED
	o  ATYP   address type of following address
	o  IP V4 address: X'01'
	o  DOMAINNAME: X'03'
	o  IP V6 address: X'04'
	o  BND.ADDR       server bound address
	o  BND.PORT       server bound port in network octet order
	*/

	if( 10 != recv( this->m_hProxyControl, abyUdpAssociateBuf, sizeof(abyUdpAssociateBuf), 0 ) )
	{
		this->ShowAssociateMsg( "Receive reply of UDP Associate req fail!" );
		return;
	}

	// 校验返回值。
	const int SOCK5_PROXY_SUCCESS = 0;
	if ( SOCK5_PROXY_VERSION != abyUdpAssociateBuf[0] 
		|| SOCK5_PROXY_SUCCESS != abyUdpAssociateBuf[1]
		|| IP_TYPE != abyUdpAssociateBuf[3] )
	{
		this->ShowAssociateMsg( "proxy error！" );
		return;
	}

	// 看服务器返回的地址。
	m_strIPProxyReply = inet_ntoa( *(IN_ADDR*)(&abyUdpAssociateBuf[4]) );
	m_sPortProxyReply = ntohs( *(short*)( &abyUdpAssociateBuf[8] ) );

	this->ShowAssociateMsg( "UDP Associate success!" );


	// 绑定客户端接收/发送端口。
	if( INVALID_SOCKET != m_hUDPProxyClient )
	{
		closesocket( m_hUDPProxyClient );
		m_hUDPProxyClient = INVALID_SOCKET;
	}
	if ( INVALID_SOCKET == m_hUDPProxyClient )
	{
		this->m_hUDPProxyClient = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	}

	if ( INVALID_SOCKET == m_hUDPProxyClient )
	{
		MessageBox( "创建UDP socket都失败了，还混个P呀。" );
		int nLastError = WSAGetLastError();
		return;
	}

	// 绑定端口。
	sockaddr_in localAddr;
	memset( &localAddr, 0, sizeof( localAddr ) );
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons( m_nPortReq );
	::bind( m_hUDPProxyClient, (sockaddr*)&localAddr, sizeof( localAddr ) );

	// 获取绑定的端口。
	int nSize = sizeof( localAddr );
	::getsockname( m_hUDPProxyClient, (sockaddr*)&localAddr, &nSize );
	this->m_nLocalInnerBindPort = ntohs( localAddr.sin_port );
	
	this->UpdateData( FALSE );

}

void CSocks5DemoDlg::ShowAssociateMsg( LPCTSTR strMsg )
{
	this->m_strAssociateStat = strMsg;
	this->UpdateData( FALSE );
}


void CSocks5DemoDlg::OnBnClickedButtonSendUdpOut()
{
	// TODO: 在此添加控件通知处理程序代码

	// 发送UDP数据要附加头。
	/**
	+----+------+------+----------+----------+----------+
	|RSV | FRAG | ATYP | DST.ADDR | DST.PORT |   DATA   |
	+----+------+------+----------+----------+----------+
	| 2  |  1   |  1   | Variable |    2     | Variable |
	+----+------+------+----------+----------+----------+

	The fields in the UDP request header are:

	o  RSV  Reserved X'0000'
	o  FRAG    Current fragment number
	o  ATYP    address type of following addresses:
	o  IP V4 address: X'01'
	o  DOMAINNAME: X'03'
	o  IP V6 address: X'04'
	o  DST.ADDR       desired destination address
	o  DST.PORT       desired destination port
	o  DATA     user data
	*/

	if ( INVALID_SOCKET == this->m_hProxyControl )
	{
		this->MessageBox( "Not connect to proxy!" );
		return;
	}

	UpdateData();

	const int BUF_SIZE = 1024;
	char abySentBuf[ BUF_SIZE ];
	char *pCursor = abySentBuf;

	*(short*)pCursor = 0;	// RSV  Reserved X'0000'
	pCursor += 2;
	
	*pCursor = 0;	// Current fragment number
	pCursor++;

	*pCursor = 0x01;	// IP V4 address: X'01'
	pCursor ++;

	int nIp = inet_addr( m_strRemoteIp );
	*(int*)pCursor = nIp;	// desired destination address
	pCursor += 4;

	*(short*)pCursor = htons( m_nRemotePort );
	pCursor += 2;

	// 最后是我们的消息。
	strcpy( pCursor, this->m_strMsgSentToRemote );
	pCursor += this->m_strMsgSentToRemote.GetLength() + 1;

	int nDataLen = pCursor - abySentBuf;

	// 把数据发出去。
	if ( INVALID_SOCKET == m_hUDPProxyClient )
	{
		this->m_hUDPProxyClient = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	}

	if ( INVALID_SOCKET == m_hUDPProxyClient )
	{
		MessageBox( "创建UDP socket都失败了，还混个P呀。" );
		int nLastError = WSAGetLastError();
		return;
	}

	// 绑定端口。
	sockaddr_in localAddr;
	memset( &localAddr, 0, sizeof( localAddr ) );
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons( m_nPortReq );
	::bind( m_hUDPProxyClient, (sockaddr*)&localAddr, sizeof( localAddr ) );

	// 获取绑定的端口。
	int nSize = sizeof( localAddr );
	::getsockname( m_hUDPProxyClient, (sockaddr*)&localAddr, &nSize );
	this->m_nLocalInnerBindPort = ntohs( localAddr.sin_port );

	// 发送
	sockaddr_in serverAddr;
	memset( &serverAddr, 0, sizeof( serverAddr ) );

	int nProxyIp = inet_addr( this->m_strIPProxyReply );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = nProxyIp;
	serverAddr.sin_port = htons( this->m_sPortProxyReply );

	int nSentLen = ::sendto( m_hUDPProxyClient, abySentBuf, nDataLen, 0, (sockaddr*)&serverAddr, sizeof( serverAddr ) );

	if( SOCKET_ERROR == nSentLen )
	{
		MessageBox( "Send UDP msg fail!" );
		this->Release();
		return ;
	}

	UpdateData( FALSE );
	
}

void CSocks5DemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( TimerCheckRcvUDP == nIDEvent )
	{
		// 检测发送到客户端的数据。
		if ( INVALID_SOCKET != m_hUDPProxyClient )
		{
			fd_set fdSocket;        // 所有可用套节字集合
			FD_ZERO(&fdSocket);
			FD_SET( m_hUDPProxyClient, &fdSocket);
			timeval tv;
			tv.tv_sec   =   0; 
			tv.tv_usec   =   0; 
			int nHasData = select( 1,&fdSocket, NULL, NULL, &tv) ; 
			if ( 1==nHasData )
			{
				// 读取接收。
				char abyRcvBuf[ 1024 ] = { 0 };
				int nLen = recv( m_hUDPProxyClient, abyRcvBuf, sizeof( abyRcvBuf ), 0 );

				// 代理服务器发送过来的UDP数据包有数据头。
				/**
				+----+------+------+----------+----------+----------+
				|RSV | FRAG | ATYP | DST.ADDR | DST.PORT |   DATA   |
				+----+------+------+----------+----------+----------+
				| 2  |  1   |  1   | Variable |    2     | Variable |
				+----+------+------+----------+----------+----------+

				The fields in the UDP request header are:

				o  RSV  Reserved X'0000'
				o  FRAG    Current fragment number
				o  ATYP    address type of following addresses:
				o  IP V4 address: X'01'
				o  DOMAINNAME: X'03'
				o  IP V6 address: X'04'
				o  DST.ADDR       desired destination address
				o  DST.PORT       desired destination port
				o  DATA     user data
				*/
				const int OFFSET = 10;

				m_strClientRcvMsg += abyRcvBuf + OFFSET;
				m_strClientRcvMsg += "\r\n";

				UpdateData( FALSE );

			}
		}

		// 检测发送到服务器的数据。（也就是本程序做服务器的时候接收数据。）
		if ( INVALID_SOCKET != m_hUDPRemoteServer )
		{
			fd_set fdSocket;        // 所有可用套节字集合
			FD_ZERO(&fdSocket);
			FD_SET( m_hUDPRemoteServer, &fdSocket);
			timeval tv;
			tv.tv_sec   =   0; 
			tv.tv_usec   =   0; 
			int nHasData = select( 1,&fdSocket, NULL, NULL, &tv) ; 
			if ( 1==nHasData )
			{
				// 读取接收。
				sockaddr_in sin;
				int sinlen = sizeof (sin);
				memset( &sin, 0, sinlen );

				char abyRcvBuf[ 1024 ] = { 0 };
				recvfrom( m_hUDPRemoteServer, abyRcvBuf, sizeof( abyRcvBuf ), 0, (sockaddr *)&sin, &sinlen );

				this->m_strRemoteServerRcvMsg += abyRcvBuf;
				m_strRemoteServerRcvMsg += "\r\n";

				// 更新代理服务器地址和端口。
				this->m_strRemoteProxyIp = inet_ntoa( sin.sin_addr );
				this->m_nRemoteProxyPort = ntohs( sin.sin_port );

				UpdateData( FALSE );

			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CSocks5DemoDlg::OnBnClickedButtonSendBindRemoteServer()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();


	if ( INVALID_SOCKET != m_hUDPRemoteServer )
	{
		// 如果端口有改变，重新建立socket。
		sockaddr_in localAddr;
		memset( &localAddr, 0, sizeof( localAddr ) );
		int nSize = sizeof( localAddr );
		::getsockname( m_hUDPRemoteServer, (sockaddr*)&localAddr, &nSize );
		int nCurPort = ntohs( localAddr.sin_port );

		if ( m_nRemoteBindPort != nCurPort )
		{
			closesocket( m_hUDPRemoteServer );
			m_hUDPRemoteServer = INVALID_SOCKET;
		}		
	}

	if ( INVALID_SOCKET == m_hUDPRemoteServer )
	{
		this->m_hUDPRemoteServer = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	}

	if ( INVALID_SOCKET == m_hUDPRemoteServer )
	{
		MessageBox( "创建UDP socket都失败了，还混个P呀。" );
		int nLastError = WSAGetLastError();
		return;
	}

	// 绑定端口。
	sockaddr_in localAddr;
	memset( &localAddr, 0, sizeof( localAddr ) );
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons( this->m_nRemoteBindPort );
	::bind( m_hUDPRemoteServer, (sockaddr*)&localAddr, sizeof( localAddr ) );

	// 获取绑定的端口。
	int nSize = sizeof( localAddr );
	::getsockname( m_hUDPRemoteServer, (sockaddr*)&localAddr, &nSize );
	this->m_nRemoteBindPort = ntohs( localAddr.sin_port );

	this->UpdateData( FALSE );
}

void CSocks5DemoDlg::OnBnClickedButtonSendUdpIn()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( INVALID_SOCKET == m_hUDPRemoteServer )
	{
		MessageBox( "先监听端口！" );
		return;
	}

	UpdateData();

	char abySentBuf[1024] = {0};
	strcpy( abySentBuf, m_strRemoteSentMsg );

	sockaddr_in serverAddr;
	memset( &serverAddr, 0, sizeof( serverAddr ) );

	int nProxyIp = inet_addr( this->m_strRemoteProxyIp );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = nProxyIp;
	serverAddr.sin_port = htons( this->m_nRemoteProxyPort );

	int nSentLen = ::sendto( m_hUDPProxyClient, abySentBuf, strlen( abySentBuf ) + 1, 0, (sockaddr*)&serverAddr, sizeof( serverAddr ) );
}

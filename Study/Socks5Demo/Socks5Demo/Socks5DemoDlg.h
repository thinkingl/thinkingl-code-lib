
// Socks5DemoDlg.h : 头文件
//

#pragma once


// CSocks5DemoDlg 对话框
class CSocks5DemoDlg : public CDialog
{
// 构造
public:
	CSocks5DemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SOCKS5DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	/** 显示连接状态信息。*/
	void ShowConnectMsg( LPCTSTR strMsg );

	/** 显示UDP穿透信息。 */
void ShowAssociateMsg( LPCTSTR strMsg );

	/** 释放资源。 */
	void Release();

private:
	/** 连接SOCKES5代理服务器的socket句柄。 */
	SOCKET m_hProxyControl;

	/** UDP代理服务器客户，发送到代理服务器和接收代理服务器发来UDP的socket句柄。 */
	SOCKET m_hUDPProxyClient;

	/** 检测接收发到客户端的UDP数据的定时器。 */
	enum ETimer
	{
		TimerCheckRcvUDP = 12345,
	};
	UINT m_dwTimerCheckSendInUDP;

	/** UDP远端服务器，发送数据到代理服务器转给客户端。 */
	SOCKET m_hUDPRemoteServer;

public:
	afx_msg void OnBnClickedButton1();
	// 代理服务器ip。
	CString m_strProxyIp;
	// 代理服务器端口。
	int m_nProxyPort;
	// 连接信息输出。
	CString m_strConnectMsg;
	afx_msg void OnBnClickedButtonUdpAssociate();
	CString m_strAssociateStat;
	CString m_strIPProxyReply;
	int m_sPortProxyReply;
	CString m_strLocalIp;
	int m_nPortReq;
	afx_msg void OnBnClickedButtonSendUdpOut();
	CString m_strMsgSentToRemote;
	CString m_strRemoteIp;
	int m_nRemotePort;
	int m_nLocalInnerBindPort;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strClientRcvMsg;
	int m_nRemoteBindPort;
	CString m_strRemoteSentMsg;
	afx_msg void OnBnClickedButtonSendBindRemoteServer();
	CString m_strRemoteServerRcvMsg;
	CString m_strRemoteProxyIp;
	int m_nRemoteProxyPort;
};

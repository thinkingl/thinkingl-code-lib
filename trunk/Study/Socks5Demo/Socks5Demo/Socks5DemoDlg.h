
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
	
	/** 释放资源。 */
	void Release();

private:
	/** 连接SOCKES5代理服务器的socket句柄。 */
	SOCKET m_hProxyControl;
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
};

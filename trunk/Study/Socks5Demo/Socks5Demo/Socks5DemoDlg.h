
// Socks5DemoDlg.h : ͷ�ļ�
//

#pragma once


// CSocks5DemoDlg �Ի���
class CSocks5DemoDlg : public CDialog
{
// ����
public:
	CSocks5DemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SOCKS5DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	/** ��ʾ����״̬��Ϣ��*/
	void ShowConnectMsg( LPCTSTR strMsg );

	/** ��ʾUDP��͸��Ϣ�� */
void ShowAssociateMsg( LPCTSTR strMsg );

	/** �ͷ���Դ�� */
	void Release();

private:
	/** ����SOCKES5�����������socket����� */
	SOCKET m_hProxyControl;

	/** UDP����������ͻ������͵�����������ͽ��մ������������UDP��socket����� */
	SOCKET m_hUDPProxyClient;

public:
	afx_msg void OnBnClickedButton1();
	// ���������ip��
	CString m_strProxyIp;
	// ����������˿ڡ�
	int m_nProxyPort;
	// ������Ϣ�����
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
};

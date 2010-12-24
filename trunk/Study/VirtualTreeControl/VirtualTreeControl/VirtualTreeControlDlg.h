
// VirtualTreeControlDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "VirtualTreeCtrl.h"

// CVirtualTreeControlDlg �Ի���
class CVirtualTreeControlDlg : public CDialog
{
// ����
public:
	CVirtualTreeControlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VIRTUALTREECONTROL_DIALOG };

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
public:
	afx_msg void OnTvnSelchangedTreeTest(NMHDR *pNMHDR, LRESULT *pResult);
	CVirtualTreeCtrl m_treeTest;
	afx_msg void OnBnClickedTest();
};

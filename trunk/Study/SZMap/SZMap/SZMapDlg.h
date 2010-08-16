
// SZMapDlg.h : ͷ�ļ�
//

#pragma once

#include "MapCtrl.h"

// CSZMapDlg �Ի���
class CSZMapDlg : public CDialogEx
{
// ����
public:
	CSZMapDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SZMAP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	/** ��ͼ�ؼ��� */
	CMapCtrl m_mapCtrl;

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
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	double m_latitudeShow;
	double m_longitudeShow;
};

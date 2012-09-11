
// SZMapDlg.h : ͷ�ļ�
//

#pragma once

#include "MapCtrl.h"
#include "afxcmn.h"

#include "MapDownloadDialog.h"

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
	/** ���пؼ��� */
	void Updatelayer( CRect *prcClient = NULL );
private:
	/** ��ͼ�ؼ��� */
	CMapCtrl m_mapCtrl;

	/** ��ͼ���ش���. */
	CMapDownloadDialog m_mapDownloadDlg;

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

	CSliderCtrl m_sliderMapZlevel;
	afx_msg void OnTRBNThumbPosChangingSliderZLevle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderZLevle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonBackShanghai();
	afx_msg void OnNMThemeChangedSliderZLevle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonGoToKeda();
	double m_dbGoLatitude;
	double m_dbGoLongitude;
	afx_msg void OnBnClickedButtonGo();
	afx_msg void OnBnClickedButtonDownload();
	afx_msg void OnBnClickedButtonMarEarth();
	afx_msg void OnBnClickedButtonEarthMar();
};

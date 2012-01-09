#pragma once


// CCompressionDCTDialog �Ի���
#include "BaseDIPFunctionDialog.h"
#include "afxwin.h"
class CCompressionDCTDialog : public CBaseDIPFunctionDialog
{
	DECLARE_DYNAMIC(CCompressionDCTDialog)

public:
	CCompressionDCTDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCompressionDCTDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_COMPRESSION_DCT };
protected:
	virtual void OnDIP( CDigitalImage *pImg );
	bool m_DCT;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedApplyDct();
	afx_msg void OnBnClickedApplyIdct();
	int m_compressionRate;
	CComboBox m_cmbCompressionRate;
};

#pragma once


// CIntensityLogTransformationDialog �Ի���

class CIntensityLogTransformationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CIntensityLogTransformationDialog)

public:
	CIntensityLogTransformationDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIntensityLogTransformationDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_INTENSITY_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
	float m_logTransformationConstant;
};

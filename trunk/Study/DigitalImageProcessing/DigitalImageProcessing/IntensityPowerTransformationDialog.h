#pragma once


// CIntensityPowerTransformationDialog �Ի���

class CIntensityPowerTransformationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CIntensityPowerTransformationDialog)

public:
	CIntensityPowerTransformationDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIntensityPowerTransformationDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_INTENSITY_POWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
	double m_const;
	double m_v;
};

#pragma once

class CDigitalImage;
// CIntensityBitplaneSliceShowDialog 对话框

class CIntensityBitplaneSliceShowDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CIntensityBitplaneSliceShowDialog)

public:
	CIntensityBitplaneSliceShowDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIntensityBitplaneSliceShowDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_INTENSITY_BITPLANE_SLICING_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void TryShowOneBit( int bit );
	void Release();
private:
	CDigitalImage *m_pImage;
public:
	afx_msg void OnBnClickedRadioBit1();
	afx_msg void OnBnClickedRadioBit2();
	afx_msg void OnBnClickedRadioBit3();
	afx_msg void OnBnClickedRadioBit4();
	afx_msg void OnBnClickedRadioBit5();
	afx_msg void OnBnClickedRadioBit6();
	afx_msg void OnBnClickedRadioBit7();
	afx_msg void OnBnClickedRadioBit8();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedCancel();
};

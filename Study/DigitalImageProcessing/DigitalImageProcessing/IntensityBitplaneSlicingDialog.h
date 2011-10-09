#pragma once


// CIntensityBitplaneSlicingDialog 对话框
class CDigitalImage;
class CIntensityBitplaneSlicingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CIntensityBitplaneSlicingDialog)

public:
	CIntensityBitplaneSlicingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIntensityBitplaneSlicingDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_INTENSITY_BITPLANE_SLICING_RECONSTRUCT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void ReleaseImage();
private:
	CDigitalImage *m_pImage;
public:
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedCancel();
	BOOL m_bit1;
	BOOL m_bit2;
	BOOL m_bit3;
	BOOL m_bit4;
	BOOL m_bit5;
	BOOL m_bit6;
	BOOL m_bit7;
	BOOL m_bit8;
};

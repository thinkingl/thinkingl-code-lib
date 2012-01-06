#pragma once


// CSpatialTranslationDialog 对话框
class CDigitalImage;
class CSpatialTranslationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSpatialTranslationDialog)

public:
	CSpatialTranslationDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpatialTranslationDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_SPATIAL_TRANSLATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void ReleaseImage();
private:
	CDigitalImage *m_pImage;
public:
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
	int m_newCoordX;
	int m_newCoordY;
};

#pragma once


// CSpatialRotationDialog 对话框
class CDigitalImage;

class CSpatialRotationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSpatialRotationDialog)

public:
	CSpatialRotationDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpatialRotationDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_SPATIAL_ROTATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void ReleaseImage();
private:
	CDigitalImage *m_pImage;
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedRadioClockwise();
	BOOL m_bUnClockWise;
	double m_rotationAngle;
	int m_whichInterpolationFunction;
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	int m_rotationX;
	int m_rotationY;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

#pragma once


// CGrayscaleDialog 对话框
#include "resource.h"
#include "afxcmn.h"

#include "dipconst.h"

class CGrayscaleDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CGrayscaleDialog)

public:
	CGrayscaleDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGrayscaleDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FUNCTION_GRAYSCALE };

public:
	/** 设置功能. 
	*	多个功能共用这个对话框:
	*	1. 灰度调节. DIP_IntensityLevels 2. DIP_RGB2Gray_HSI 3. DIP_RGB2Gray_YUV
	*/
	void SetDIPFunction( EDIPFunction fun );

protected:
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
	int m_intensityLevel;
private:
	CSliderCtrl m_sliderIntensityLevel;

	EDIPFunction m_dipFunction;
public:
	afx_msg void OnTRBNThumbPosChangingSliderIntensityLevel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
};

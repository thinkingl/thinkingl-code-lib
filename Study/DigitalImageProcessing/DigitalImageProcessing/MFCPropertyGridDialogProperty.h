#pragma once

#include "stdafx.h"
#include "afxpropertygridctrl.h"


class CMFCPropertyGridDialogProperty :
	public CMFCPropertyGridProperty
{
//	DECLARE_DYNAMIC(CMFCPropertyGridDialogProperty)
public:
	CMFCPropertyGridDialogProperty(const CString& strName, CDialog *pDlg, UINT dlgResourceId, LPCTSTR lpszBtnText, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0
		);
	virtual ~CMFCPropertyGridDialogProperty(void);

	/** …Ë÷√∂‘ª∞øÚ. */
//	void SetDialog( CDialog *pDlg, DWORD dlgResourceId );
	virtual BOOL OnClickValue(
		UINT uiMsg,
		CPoint point 
		);
	virtual BOOL OnEdit(LPPOINT lptClick);
	virtual void OnDrawValue(CDC* pDC, CRect rect);
	virtual void OnClickButton(CPoint point);

private:
	CDialog *m_pDlg;
	UINT m_dlgResourceId;
	CString m_buttonText;
};


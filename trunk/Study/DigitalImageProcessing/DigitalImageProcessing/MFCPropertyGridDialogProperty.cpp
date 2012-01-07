#include "MFCPropertyGridDialogProperty.h"
#include "stdafx.h"

#define AFX_PROP_HAS_LIST 0x0001
#define AFX_PROP_HAS_BUTTON 0x0002
#define AFX_PROP_HAS_SPIN 0x0004

CMFCPropertyGridDialogProperty::CMFCPropertyGridDialogProperty(const CString& strName,  CDialog *pDlg, UINT dlgResourceId,  LPCTSTR lpszBtnText,  
	LPCTSTR lpszDescr /* = NULL */, DWORD_PTR dwData /* = 0 */)
	: CMFCPropertyGridProperty( strName, COleVariant(), lpszDescr, dwData )
{
	this->m_pDlg = pDlg;
	this->m_dlgResourceId = dlgResourceId;
	this->m_buttonText = lpszBtnText;

//	m_dwFlags = AFX_PROP_HAS_BUTTON;
}


CMFCPropertyGridDialogProperty::~CMFCPropertyGridDialogProperty(void)
{

}

void CMFCPropertyGridDialogProperty::OnClickButton(CPoint /*point*/)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	if ( m_pDlg && m_dlgResourceId )
	{
		if ( m_pDlg->GetSafeHwnd() )
		{
			// 窗口已经创建.

		}
		else
		{
			// 窗口还没有创建.
			m_pDlg->Create( this->m_dlgResourceId , this->m_pWndList );
		}

		m_pDlg->ShowWindow( SW_SHOW );
	}
}

BOOL CMFCPropertyGridDialogProperty::OnClickValue( UINT uiMsg, CPoint point )
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	if ( m_pDlg && m_dlgResourceId )
	{
		if ( m_pDlg->GetSafeHwnd() == NULL )
		{
			// 窗口还没有创建.
			m_pDlg->Create( this->m_dlgResourceId , this->m_pWndList );
		}

		m_pDlg->ShowWindow( SW_SHOW );
	}
	return TRUE;
}

BOOL CMFCPropertyGridDialogProperty::OnEdit(LPPOINT lptClick)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	// 创建.
	if ( m_pDlg && m_dlgResourceId )
	{
		if ( m_pDlg->GetSafeHwnd() )
		{
			// 窗口已经创建.

		}
		else
		{
			// 窗口还没有创建.
			m_pDlg->Create( this->m_dlgResourceId , this->m_pWndList );
		}

		this->m_pWndInPlace = m_pDlg;
//		m_pDlg->ShowWindow( SW_SHOW );
	}


	return TRUE;
}

void CMFCPropertyGridDialogProperty::OnDrawValue(CDC* pDC, CRect rect)
{
	pDC->DrawText( _T("点击查看直方图"), rect, 0 );
}
//
//void CMFCPropertyGridDialogProperty::SetDialog( CDialog *pDlg, DWORD dlgResourceId )
//{
//	this->m_pDlg = pDlg;
//	this->m_dlgResourceId = dlgResourceId;
//}
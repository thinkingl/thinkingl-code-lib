
// cashbookmfcView.h : CcashbookmfcView ��Ľӿ�
//

#pragma once


class CcashbookmfcView : public CListView
{
protected: // �������л�����
	CcashbookmfcView();
	DECLARE_DYNCREATE(CcashbookmfcView)

// ����
public:
	CcashbookmfcDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CcashbookmfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // cashbookmfcView.cpp �еĵ��԰汾
inline CcashbookmfcDoc* CcashbookmfcView::GetDocument() const
   { return reinterpret_cast<CcashbookmfcDoc*>(m_pDocument); }
#endif


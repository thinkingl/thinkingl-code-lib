
// cashbookmfcView.cpp : CcashbookmfcView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "cashbookmfc.h"
#endif

#include "cashbookmfcDoc.h"
#include "cashbookmfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CcashbookmfcView

IMPLEMENT_DYNCREATE(CcashbookmfcView, CListView)

BEGIN_MESSAGE_MAP(CcashbookmfcView, CListView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CcashbookmfcView ����/����

CcashbookmfcView::CcashbookmfcView()
{
	// TODO: �ڴ˴���ӹ������

}

CcashbookmfcView::~CcashbookmfcView()
{
}

BOOL CcashbookmfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CListView::PreCreateWindow(cs);
}

void CcashbookmfcView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: ���� GetListCtrl() ֱ�ӷ��� ListView ���б�ؼ���
	//  �Ӷ������������ ListView��
}

void CcashbookmfcView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CcashbookmfcView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CcashbookmfcView ���

#ifdef _DEBUG
void CcashbookmfcView::AssertValid() const
{
	CListView::AssertValid();
}

void CcashbookmfcView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CcashbookmfcDoc* CcashbookmfcView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcashbookmfcDoc)));
	return (CcashbookmfcDoc*)m_pDocument;
}
#endif //_DEBUG


// CcashbookmfcView ��Ϣ�������


// cashbookmfcView.cpp : CcashbookmfcView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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

// CcashbookmfcView 构造/析构

CcashbookmfcView::CcashbookmfcView()
{
	// TODO: 在此处添加构造代码

}

CcashbookmfcView::~CcashbookmfcView()
{
}

BOOL CcashbookmfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CListView::PreCreateWindow(cs);
}

void CcashbookmfcView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	//  从而可以用项填充 ListView。
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


// CcashbookmfcView 诊断

#ifdef _DEBUG
void CcashbookmfcView::AssertValid() const
{
	CListView::AssertValid();
}

void CcashbookmfcView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CcashbookmfcDoc* CcashbookmfcView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcashbookmfcDoc)));
	return (CcashbookmfcDoc*)m_pDocument;
}
#endif //_DEBUG


// CcashbookmfcView 消息处理程序

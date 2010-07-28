
// SZPEView.cpp : CSZPEView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "SZPE.h"
#endif

#include "SZPEDoc.h"
#include "SZPEView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSZPEView

IMPLEMENT_DYNCREATE(CSZPEView, CView)

BEGIN_MESSAGE_MAP(CSZPEView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSZPEView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSZPEView ����/����

CSZPEView::CSZPEView()
{
	// TODO: �ڴ˴���ӹ������

}

CSZPEView::~CSZPEView()
{
}

BOOL CSZPEView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CSZPEView ����

void CSZPEView::OnDraw(CDC* /*pDC*/)
{
	CSZPEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CSZPEView ��ӡ


void CSZPEView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSZPEView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSZPEView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CSZPEView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CSZPEView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSZPEView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSZPEView ���

#ifdef _DEBUG
void CSZPEView::AssertValid() const
{
	CView::AssertValid();
}

void CSZPEView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSZPEDoc* CSZPEView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSZPEDoc)));
	return (CSZPEDoc*)m_pDocument;
}
#endif //_DEBUG


// CSZPEView ��Ϣ�������

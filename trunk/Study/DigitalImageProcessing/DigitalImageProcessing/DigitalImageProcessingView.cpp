
// DigitalImageProcessingView.cpp : CDigitalImageProcessingView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DigitalImageProcessing.h"
#endif

#include "DigitalImageProcessingDoc.h"
#include "DigitalImageProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDigitalImageProcessingView

IMPLEMENT_DYNCREATE(CDigitalImageProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CDigitalImageProcessingView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDigitalImageProcessingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CDigitalImageProcessingView ����/����

CDigitalImageProcessingView::CDigitalImageProcessingView()
{
	// TODO: �ڴ˴���ӹ������
	m_pImageDrawer = NULL;
	m_pImage = NULL;
}

CDigitalImageProcessingView::~CDigitalImageProcessingView()
{
	ReleaseImageDrawer();
}

BOOL CDigitalImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CDigitalImageProcessingView ����

void CDigitalImageProcessingView::OnDraw(CDC* pDC)
{
	CDigitalImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
//	HRESULT hr = this->m_image.Load( pDoc->GetOpenFilePath() );
	if ( m_pImageDrawer && !m_pImageDrawer->IsNull() )
	{
		int width = this->m_pImageDrawer->GetWidth();
		int height = this->m_pImageDrawer->GetHeight();
		this->m_pImageDrawer->Draw( pDC->m_hDC, CRect( 0,0, width, height ) );
	}
	
}


// CDigitalImageProcessingView ��ӡ


void CDigitalImageProcessingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDigitalImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDigitalImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDigitalImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CDigitalImageProcessingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDigitalImageProcessingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDigitalImageProcessingView ���

#ifdef _DEBUG
void CDigitalImageProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDigitalImageProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDigitalImageProcessingDoc* CDigitalImageProcessingView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDigitalImageProcessingDoc)));
	return (CDigitalImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CDigitalImageProcessingView ��Ϣ�������


void CDigitalImageProcessingView::OnUpdate(CView* pSender, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: �ڴ����ר�ô����/����û���
	CDigitalImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDigitalImage *pImg = pDoc->CloneImage();
	if ( pImg )
	{
		this->ReleaseImageDrawer();

		this->m_pImage = pImg;
		m_pImageDrawer = pImg->GetImageDrawer();

		int width = m_pImageDrawer->GetWidth();
		int height = m_pImageDrawer->GetHeight();

		this->SetScrollSizes( MM_TEXT, CSize(width, height ) );
	}	
	else
	{
		CRect rcWnd;
		this->GetWindowRect( rcWnd );
		this->SetScaleToFitSize( rcWnd.Size() );
	}
	
}

void CDigitalImageProcessingView::ReleaseImageDrawer()
{
	if ( m_pImageDrawer )
	{
		delete m_pImageDrawer;
		m_pImageDrawer = NULL;
	}
	if ( m_pImage )
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}


//void CDigitalImageProcessingView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
//}


void CDigitalImageProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	
	// TODO: �ڴ����ר�ô����/����û���
}

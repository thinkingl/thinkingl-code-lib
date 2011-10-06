
// DigitalImageProcessingView.cpp : CDigitalImageProcessingView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDigitalImageProcessingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CDigitalImageProcessingView 构造/析构

CDigitalImageProcessingView::CDigitalImageProcessingView()
{
	// TODO: 在此处添加构造代码
	m_pImageDrawer = NULL;
	m_pImage = NULL;
}

CDigitalImageProcessingView::~CDigitalImageProcessingView()
{
	ReleaseImageDrawer();
}

BOOL CDigitalImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CDigitalImageProcessingView 绘制

void CDigitalImageProcessingView::OnDraw(CDC* pDC)
{
	CDigitalImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
//	HRESULT hr = this->m_image.Load( pDoc->GetOpenFilePath() );
	if ( m_pImageDrawer && !m_pImageDrawer->IsNull() )
	{
		int width = this->m_pImageDrawer->GetWidth();
		int height = this->m_pImageDrawer->GetHeight();
		this->m_pImageDrawer->Draw( pDC->m_hDC, CRect( 0,0, width, height ) );
	}
	
}


// CDigitalImageProcessingView 打印


void CDigitalImageProcessingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDigitalImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDigitalImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDigitalImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CDigitalImageProcessingView 诊断

#ifdef _DEBUG
void CDigitalImageProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDigitalImageProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDigitalImageProcessingDoc* CDigitalImageProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDigitalImageProcessingDoc)));
	return (CDigitalImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CDigitalImageProcessingView 消息处理程序


void CDigitalImageProcessingView::OnUpdate(CView* pSender, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类
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
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
//}


void CDigitalImageProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	
	// TODO: 在此添加专用代码和/或调用基类
}

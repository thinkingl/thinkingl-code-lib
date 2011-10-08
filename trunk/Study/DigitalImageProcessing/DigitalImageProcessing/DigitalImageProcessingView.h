
// DigitalImageProcessingView.h : CDigitalImageProcessingView 类的接口
//

#pragma once

class CDigitalImage;
class CDigitalImageProcessingDoc;

class CDigitalImageProcessingView : public CScrollView 
{
protected: // 仅从序列化创建
	CDigitalImageProcessingView();
	DECLARE_DYNCREATE(CDigitalImageProcessingView)

// 特性
public:
	CDigitalImageProcessingDoc* GetDocument() const;

	/** 开始图像处理. */
	CDigitalImage *StartDIP();
	/** 结束图像处理. */
	void EndDIP();


// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CDigitalImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	/** 销毁图片绘制对象. */
	void ReleaseImageDrawer();
	/** 销毁图片处理对象. */
	void ReleaseImage();
private:
	CDigitalImage *m_pImage;
	CImage *m_pImageDrawer;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
};

#ifndef _DEBUG  // DigitalImageProcessingView.cpp 中的调试版本
inline CDigitalImageProcessingDoc* CDigitalImageProcessingView::GetDocument() const
   { return reinterpret_cast<CDigitalImageProcessingDoc*>(m_pDocument); }
#endif


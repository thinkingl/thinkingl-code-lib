
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

	/** 开始图像处理. 
	*	返回一个图形对象进行图像处理.
	*	如果调用了 EndDIP, 使用者就不用delete 这个返回指针.
	*	如果没有调用 EndDIP, 那么这个指针必须要由使用者 delete.
	*/
	CDigitalImage *StartDIP();

	/** 结束图像处理. 
	*	将处理结果保存并更新显示.
	*/
	void EndDIP( CDigitalImage* pImg );

	/** 预览. 只显示. */
	void DIPPreview( CDigitalImage* pImg );
	/** 取消预览. 重新显示view自身的图像. */
	void CancelPreview();

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


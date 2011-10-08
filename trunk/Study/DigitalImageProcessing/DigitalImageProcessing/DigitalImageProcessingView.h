
// DigitalImageProcessingView.h : CDigitalImageProcessingView ��Ľӿ�
//

#pragma once

class CDigitalImage;
class CDigitalImageProcessingDoc;

class CDigitalImageProcessingView : public CScrollView 
{
protected: // �������л�����
	CDigitalImageProcessingView();
	DECLARE_DYNCREATE(CDigitalImageProcessingView)

// ����
public:
	CDigitalImageProcessingDoc* GetDocument() const;

	/** ��ʼͼ����. */
	CDigitalImage *StartDIP();
	/** ����ͼ����. */
	void EndDIP();


// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CDigitalImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	/** ����ͼƬ���ƶ���. */
	void ReleaseImageDrawer();
	/** ����ͼƬ�������. */
	void ReleaseImage();
private:
	CDigitalImage *m_pImage;
	CImage *m_pImageDrawer;

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // DigitalImageProcessingView.cpp �еĵ��԰汾
inline CDigitalImageProcessingDoc* CDigitalImageProcessingView::GetDocument() const
   { return reinterpret_cast<CDigitalImageProcessingDoc*>(m_pDocument); }
#endif


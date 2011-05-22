
// cashbookmfcView.h : CcashbookmfcView 类的接口
//

#pragma once


class CcashbookmfcView : public CListView
{
protected: // 仅从序列化创建
	CcashbookmfcView();
	DECLARE_DYNCREATE(CcashbookmfcView)

// 特性
public:
	CcashbookmfcDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CcashbookmfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // cashbookmfcView.cpp 中的调试版本
inline CcashbookmfcDoc* CcashbookmfcView::GetDocument() const
   { return reinterpret_cast<CcashbookmfcDoc*>(m_pDocument); }
#endif


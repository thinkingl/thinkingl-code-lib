#pragma once


// CMapCtrl

class CMapCtrl : public CWnd
{
	DECLARE_DYNAMIC(CMapCtrl)

public:
	CMapCtrl();
	virtual ~CMapCtrl();

private:
	/** ͼ����ơ� */
	CImage m_imgDraw;

	/** ͼƬ���Ͻ�λ�á� */
	CPoint m_imgPos;

	/** ����ϴ�λ�á� */
	CPoint m_lastMousePos;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};



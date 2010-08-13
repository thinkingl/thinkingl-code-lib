#pragma once


// CMapCtrl

class CMapCtrl : public CWnd
{
	DECLARE_DYNAMIC(CMapCtrl)

public:
	CMapCtrl();
	virtual ~CMapCtrl();

private:
	/** 图像绘制。 */
	CImage m_imgDraw;

	/** 图片左上角位置。 */
	CPoint m_imgPos;

	/** 鼠标上次位置。 */
	CPoint m_lastMousePos;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};



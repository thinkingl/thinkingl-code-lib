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

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();
};



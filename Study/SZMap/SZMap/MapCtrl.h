#pragma once

#include <map>
// CMapCtrl

class CMapCtrl : public CWnd
{
	DECLARE_DYNAMIC(CMapCtrl)

public:
	CMapCtrl();
	virtual ~CMapCtrl();

	enum
	{
		IMG_SIZE = 256,
	};

private:
	/** 刷新图片。 */
	void UpdateImageBuffer();
private:

	/** 鼠标上次位置。 */
	CPoint m_lastMousePos;

	/** 显示的图片序号。 */
	CRect m_imageIndexRect;

	/** 显示区域 */
	CRect m_rectShow;

	/** 图片区域左上角 */
	CPoint m_ptImage;

	/** 新生图片区域，如果图片区域切入此区域，则需要扩充图片区域，直到大于此区域。 */
	CRect m_rectNewImage;
	/** 删除图片区域，图片区域一旦超出此区域，则消减图片区域，直到小于此区域。 */
	CRect m_rectDead;

	/** 当前显示的图片和位置表。 */
//	typedef CPoint TImageIndex;
	class CImageIndex : public CPoint
	{
	public:
		bool operator<( const CImageIndex& another ) const
		{
			if( this->x == another.x )
			{
				return this->y  < another.y;
			}
			else
			{
				return this->x < another.x;
			}
		}
	};


	typedef std::auto_ptr< CImage > PTImage;
	typedef std::map< CImageIndex, PTImage> TImageBufferTable;

	TImageBufferTable m_tableImageBuffer;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};



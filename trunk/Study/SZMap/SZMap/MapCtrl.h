#pragma once

#include <map>
#include "Coord.h"
// CMapCtrl

/** ��ǰ��ʾ��ͼƬ��λ�ñ� */
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
	/** ˢ��ͼƬ�� */
	void UpdateImageBuffer();

	/** ����ͼƬ�� */
	BOOL LoadMapImage( const CImageIndex& imgIndex );

	/** ͨ��ͼƬ��ţ���λͼƬ�ļ��� */
	CString ImageIndex2ImagePath( int zLevel, const CImageIndex& imgIndex ) const;

	/** ͨ����ǰ�ͻ�����������㾭γ�ȡ� */
	CCoord ClientArea2Coord( const CPoint& point ) const;

	/** ��ͼ���ĵ�ľ�γ�ȡ� */
//	CCoord m_mapCoord;

private:

	/** ����ϴ�λ�á� */
	CPoint m_lastMousePos;

	/** ��ʾ��ͼƬ��š� */
	CRect m_imageIndexRect;

	/** ��ʾ���� */
	CRect m_rectShow;

	/** ͼƬ�������Ͻ� */
	CPoint m_ptImage;

	/** ����ͼƬ�������ͼƬ�����������������Ҫ����ͼƬ����ֱ�����ڴ����� */
	CRect m_rectNewImage;
	/** ɾ��ͼƬ����ͼƬ����һ������������������ͼƬ����ֱ��С�ڴ����� */
	CRect m_rectDead;




	typedef std::auto_ptr< CImage > PTImage;
	typedef std::map< CImageIndex, PTImage> TImageBufferTable;

	TImageBufferTable m_tableImageBuffer;

	/** ��ǰ�����ŵȼ� */
	int m_nZLevel;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};



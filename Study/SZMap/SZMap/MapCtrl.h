#pragma once

#include <map>
#include "Coord.h"
#include "ImageFactory.h"
// CMapCtrl

enum
{
	IMG_SIZE = 256,
	MAX_MAP_ZLEVEL = 17,
	MIN_MAP_ZLEVEL = -2,
};

enum EMapMessage
{
	WM_MAP_ON_LOAD_IMAGE = WM_USER + 1000, // 图片载入通知。
};



class CMapCtrl : public CWnd
{
	DECLARE_DYNAMIC(CMapCtrl)

public:
	CMapCtrl();
	virtual ~CMapCtrl();

public:
	/** 设置缩放比例。 */
	void SetZLevel( int nZLevel );

	/** 获取缩放比例。 */
	int GetZLevel()const;

	/** 中心定位。 */
	void Move2Center( const CCoord& centerCoord );

public:
// 	class CMapCtrlImageCallback : public IImageCallback
// 	{
// 	public:
// 		virtual void operator()()
// 		{
// 			if( this->m_hMapWnd )
// 			{
// 				// 发送消息通知地图控件，刷新某区域。
// 				::SendMessage( this->m_hMapWnd, WM_MAP_ON_LOAD_IMAGE, WPARAM( this ), 0 ); 
// 			}
// 
// 			// 自尽
// 			delete this;
// 		}
// 	public: 
// 		 HWND m_hMapWnd;
// 	};
	
private:
	/** 刷新图片。 */
	void UpdateImageBuffer();

	/** 载入图片。 */
	BOOL LoadMapImage( const CImageIndex& imgIndex );

	/** 通过图片序号，定位图片文件。 */
	CString ImageIndex2ImagePath( int zLevel, const CImageIndex& imgIndex ) const;

	/** 通过当前客户区域坐标计算经纬度。 */
	CCoord ClientArea2Coord( const CPoint& point ) const;

	/** 通过经纬度坐标获取对应的像素坐标。 */
	CPoint Coord2ImagePixel( const CCoord& coord ) const;

	/** 清理图片缓存。 */
	void ClearImageBuffer();

	/** 地图中心点的经纬度。 */
//	CCoord m_mapCoord;

private:
	/** 图片工厂，提供图片。 */
	CImageFactory m_imageFactory;

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

	/** 中心点的经纬度，用于缩放的时候消除误差。 */
	CCoord m_centerCoord;


	typedef std::auto_ptr< CImage > PTImage;
	typedef std::map< CImageIndex, PTImage> TImageBufferTable;

	TImageBufferTable m_tableImageBuffer;

	/** 当前的缩放等级 */
	int m_nZLevel;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};



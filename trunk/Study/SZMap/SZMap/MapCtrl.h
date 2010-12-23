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
	WM_MAP_ON_LOAD_IMAGE = WM_USER + 1000, // ͼƬ����֪ͨ��
};



class CMapCtrl : public CWnd
{
	DECLARE_DYNAMIC(CMapCtrl)

public:
	CMapCtrl();
	virtual ~CMapCtrl();

public:
	/** �������ű����� */
	void SetZLevel( int nZLevel );

	/** ��ȡ���ű����� */
	int GetZLevel()const;

	/** ���Ķ�λ�� */
	void Move2Center( const CCoord& centerCoord );

public:
// 	class CMapCtrlImageCallback : public IImageCallback
// 	{
// 	public:
// 		virtual void operator()()
// 		{
// 			if( this->m_hMapWnd )
// 			{
// 				// ������Ϣ֪ͨ��ͼ�ؼ���ˢ��ĳ����
// 				::SendMessage( this->m_hMapWnd, WM_MAP_ON_LOAD_IMAGE, WPARAM( this ), 0 ); 
// 			}
// 
// 			// �Ծ�
// 			delete this;
// 		}
// 	public: 
// 		 HWND m_hMapWnd;
// 	};
	
private:
	/** ˢ��ͼƬ�� */
	void UpdateImageBuffer();

	/** ����ͼƬ�� */
	BOOL LoadMapImage( const CImageIndex& imgIndex );

	/** ͨ��ͼƬ��ţ���λͼƬ�ļ��� */
	CString ImageIndex2ImagePath( int zLevel, const CImageIndex& imgIndex ) const;

	/** ͨ����ǰ�ͻ�����������㾭γ�ȡ� */
	CCoord ClientArea2Coord( const CPoint& point ) const;

	/** ͨ����γ�������ȡ��Ӧ���������ꡣ */
	CPoint Coord2ImagePixel( const CCoord& coord ) const;

	/** ����ͼƬ���档 */
	void ClearImageBuffer();

	/** ��ͼ���ĵ�ľ�γ�ȡ� */
//	CCoord m_mapCoord;

private:
	/** ͼƬ�������ṩͼƬ�� */
	CImageFactory m_imageFactory;

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

	/** ���ĵ�ľ�γ�ȣ��������ŵ�ʱ�������� */
	CCoord m_centerCoord;


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
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};



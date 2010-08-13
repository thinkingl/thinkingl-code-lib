// MapCtrl.cpp : 实现文件
//

#include "stdafx.h"

#include "MapCtrl.h"


// CMapCtrl

IMPLEMENT_DYNAMIC(CMapCtrl, CWnd)

CMapCtrl::CMapCtrl()
{

}

CMapCtrl::~CMapCtrl()
{
}


BEGIN_MESSAGE_MAP(CMapCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CMapCtrl 消息处理程序




void CMapCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CRect rcClient;
	this->GetClientRect( rcClient );

	CString strText = _T( "map control" );
	dc.TextOutW( 10, 10, strText );


	if( !this->m_imgDraw.IsNull() )
	{
		this->m_imgDraw.BitBlt( dc, this->m_imgPos.x, this->m_imgPos.y, 256, 256, 0, 0 );
	}
}


void CMapCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	// 载入图片.
	HRESULT hr = this->m_imgDraw.Load( _T( "J:\\GMapCatcher\\.googlemaps\\tiles\\17\\0\\0\\0\\0.png" ) );

	


	CWnd::PreSubclassWindow();
}


void CMapCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( MK_LBUTTON & nFlags )
	{
		// 图片随着鼠标运动。
		CSize move = point - this->m_lastMousePos;

		this->m_imgPos += move;

		this->Invalidate();
	}

	this->m_lastMousePos = point;

	CWnd::OnMouseMove(nFlags, point);
}

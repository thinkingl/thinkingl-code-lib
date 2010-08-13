// MapCtrl.cpp : ʵ���ļ�
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



// CMapCtrl ��Ϣ�������




void CMapCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
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
	// TODO: �ڴ����ר�ô����/����û���

	// ����ͼƬ.
	HRESULT hr = this->m_imgDraw.Load( _T( "J:\\GMapCatcher\\.googlemaps\\tiles\\17\\0\\0\\0\\0.png" ) );

	


	CWnd::PreSubclassWindow();
}


void CMapCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if( MK_LBUTTON & nFlags )
	{
		// ͼƬ��������˶���
		CSize move = point - this->m_lastMousePos;

		this->m_imgPos += move;

		this->Invalidate();
	}

	this->m_lastMousePos = point;

	CWnd::OnMouseMove(nFlags, point);
}

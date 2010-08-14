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
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CMapCtrl ��Ϣ�������




void CMapCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	CRect rcClient;
	this->GetClientRect( rcClient );

	//CString strText = _T( "map control" );
	//dc.TextOutW( 10, 10, strText );


	//if( !this->m_imgDraw.IsNull() )
	//{
	//	this->m_imgDraw.BitBlt( dc, this->m_imgPos.x, this->m_imgPos.y, 256, 256, 0, 0 );
	//}

	// ��ͼƬ���Ƶ����ڡ�
	for( int x=0; x<this->m_imageIndexRect.Width(); ++x )
	{
		for( int y=0; y<this->m_imageIndexRect.Height(); ++y )
		{
			CImageIndex imgIndex;
			imgIndex.x = this->m_imageIndexRect.left + x;
			imgIndex.y = this->m_imageIndexRect.top + y;

			CImage* pImg = this->m_tableImageBuffer[ imgIndex ].get();
			if( pImg )
			{
				int nDrawPosX = this->m_ptImage.x + x * IMG_SIZE;
				int nDrawPosY = this->m_ptImage.y + y * IMG_SIZE;
				pImg->BitBlt( dc, nDrawPosX, nDrawPosY, IMG_SIZE, IMG_SIZE, 0, 0 );
			}						
		}
	}
}


void CMapCtrl::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���


	CWnd::PreSubclassWindow();
}


void CMapCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if( MK_LBUTTON & nFlags )
	{
		// ͼƬ��������˶���
		CSize move = point - this->m_lastMousePos;

		this->m_ptImage += move;

		// ˢ��ͼƬ���档
		this->UpdateImageBuffer();

		this->Invalidate();
	}

	this->m_lastMousePos = point;

	CWnd::OnMouseMove(nFlags, point);
}


void CMapCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if( this->GetSafeHwnd() )
	{
		this->m_rectShow = CRect( 0,0,cx,cy );

		this->UpdateImageBuffer();
	}
}

void CMapCtrl::UpdateImageBuffer()
{
	// ������ʾ�����Сȷ����������Ĵ�С��
	

	this->m_rectNewImage = this->m_rectShow;
	this->m_rectNewImage.InflateRect( IMG_SIZE, IMG_SIZE );

	this->m_rectDead = this->m_rectNewImage;
	this->m_rectDead.InflateRect( 2*IMG_SIZE, 2*IMG_SIZE );

	CRect rcImg( m_ptImage.x, m_ptImage.y, 
		m_ptImage.x+this->m_imageIndexRect.Width() * IMG_SIZE, m_ptImage.y + this->m_imageIndexRect.Height() * IMG_SIZE );

	// �Ƿ���Ҫ�����µ�ͼƬ��
	while( rcImg.top > this->m_rectNewImage.top )
	{
		// �ϱ���Ҫ�µ�ͼƬ���롣
		this->m_imageIndexRect.InflateRect( 0, 1, 0, 0 );
		rcImg.InflateRect( 0, IMG_SIZE, 0, 0 );
		for( int i=this->m_imageIndexRect.left; i<this->m_imageIndexRect.right; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = i;
			imgIndex.y = this->m_imageIndexRect.top;
			
			PTImage pimg( new CImage() );
			pimg->Load( _T( "J:\\GMapCatcher\\.googlemaps\\tiles\\17\\0\\0\\0\\0.png" ) );

			this->m_tableImageBuffer[ imgIndex ] = pimg;
		}
	}
	while( rcImg.left > this->m_rectNewImage.left )
	{
		// �����Ҫ�µ�ͼƬ���롣
		this->m_imageIndexRect.InflateRect( 1, 0, 0, 0 );
		rcImg.InflateRect( IMG_SIZE, 0, 0, 0 );
		for( int i=this->m_imageIndexRect.top; i<this->m_imageIndexRect.bottom; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = this->m_imageIndexRect.left;
			imgIndex.y = i;
			
			PTImage pimg( new CImage() );
			pimg->Load( _T( "J:\\GMapCatcher\\.googlemaps\\tiles\\17\\0\\0\\0\\0.png" ) );

			this->m_tableImageBuffer[ imgIndex ] = pimg;
		}
	}
	while( rcImg.bottom < this->m_rectNewImage.bottom )
	{
		// �±���Ҫ�µ�ͼƬ���롣		
		for( int i=this->m_imageIndexRect.left; i<this->m_imageIndexRect.right; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = i;
			imgIndex.y = this->m_imageIndexRect.bottom;
			
			PTImage pimg( new CImage() );
			pimg->Load( _T( "J:\\GMapCatcher\\.googlemaps\\tiles\\17\\0\\0\\0\\0.png" ) );

			this->m_tableImageBuffer[ imgIndex ] = pimg;
		}
		this->m_imageIndexRect.InflateRect( 0, 0, 0, 1 );
		rcImg.InflateRect( 0, 0, 0, IMG_SIZE );
	}
	while( rcImg.right < this->m_rectNewImage.right )
	{
		// �ұ���Ҫ�µ�ͼƬ���롣
		for( int i=this->m_imageIndexRect.top; i<this->m_imageIndexRect.bottom; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = this->m_imageIndexRect.right;
			imgIndex.y = i;
			
			PTImage pimg( new CImage() );
			pimg->Load( _T( "J:\\GMapCatcher\\.googlemaps\\tiles\\17\\0\\0\\0\\0.png" ) );

			this->m_tableImageBuffer[ imgIndex ] = pimg;
		}
		this->m_imageIndexRect.InflateRect( 0, 0, 1, 0 );
		rcImg.InflateRect( 0, 0, IMG_SIZE, 0 );
	}

	// �����µ�ͼƬλ�á�
	this->m_ptImage = rcImg.TopLeft();


	// �Ƿ���Ҫ���پɵ�ͼƬ��
}

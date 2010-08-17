// MapCtrl.cpp : 实现文件
//

#include "stdafx.h"

#include "MapCtrl.h"
#include <math.h>

// CMapCtrl

IMPLEMENT_DYNAMIC(CMapCtrl, CWnd)

CMapCtrl::CMapCtrl()
{
	int x = 1*1024+671+10;
	int y = 0*1024+824+10;
	this->m_imageIndexRect = CRect( x, y, x, y );

	this->m_nZLevel = 6;
}

CMapCtrl::~CMapCtrl()
{
}


BEGIN_MESSAGE_MAP(CMapCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// CMapCtrl 消息处理程序




void CMapCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CRect rcClient;
	this->GetClientRect( rcClient );

	//CString strText = _T( "map control" );
	//dc.TextOutW( 10, 10, strText );


	//if( !this->m_imgDraw.IsNull() )
	//{
	//	this->m_imgDraw.BitBlt( dc, this->m_imgPos.x, this->m_imgPos.y, 256, 256, 0, 0 );
	//}

	// 将图片绘制到窗口。
	for( int x=0; x<this->m_imageIndexRect.Width(); ++x )
	{
		for( int y=0; y<this->m_imageIndexRect.Height(); ++y )
		{
			CImageIndex imgIndex;
			imgIndex.x = this->m_imageIndexRect.left + x;
			imgIndex.y = this->m_imageIndexRect.top + y;

			CImage* pImg = this->m_tableImageBuffer[ imgIndex ].get();
			int nDrawPosX = this->m_ptImage.x + x * IMG_SIZE;
			int nDrawPosY = this->m_ptImage.y + y * IMG_SIZE;
			if( pImg )
			{				
				pImg->BitBlt( dc, nDrawPosX, nDrawPosY, IMG_SIZE, IMG_SIZE, 0, 0 );
			}		
			else
			{
				dc.FillSolidRect( nDrawPosX, nDrawPosY, IMG_SIZE, IMG_SIZE, RGB( 200,200,200 ) );

				CString strText;
				strText.Format( _T( "囧 ，没这个图片！x: %d y: %d  " ), imgIndex.x, imgIndex.y );
				dc.TextOut( nDrawPosX + 10, nDrawPosY + 10, strText ); 
			}
		}
	}
}


void CMapCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类


	CWnd::PreSubclassWindow();
}


void CMapCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( MK_LBUTTON & nFlags )
	{
		// 图片随着鼠标运动。
		CSize move = point - this->m_lastMousePos;

		this->m_ptImage += move;

		// 刷新图片缓存。
		this->UpdateImageBuffer();

		this->Invalidate();
	}

	this->m_lastMousePos = point;

	CWnd::OnMouseMove(nFlags, point);
}


void CMapCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( this->GetSafeHwnd() )
	{
		// 备份旧的显示区域。
		CRect rectOldShow = this->m_rectShow;

		this->m_rectShow = CRect( 0,0,cx,cy );

		// 计算显示区域的中心偏移了多少位置。
		CSize offset = this->m_rectShow.CenterPoint() - rectOldShow.CenterPoint();

		// 图片区域跟着偏移，这样保证中心对齐。
		this->m_ptImage += offset;

		this->UpdateImageBuffer();
	}
}

void CMapCtrl::UpdateImageBuffer()
{
	// 根据显示区域大小确定其它区域的大小。
	

	this->m_rectNewImage = this->m_rectShow;
	this->m_rectNewImage.InflateRect( IMG_SIZE, IMG_SIZE );

	this->m_rectDead = this->m_rectNewImage;
	this->m_rectDead.InflateRect( 2*IMG_SIZE, 2*IMG_SIZE );

	CRect rcImg( m_ptImage.x, m_ptImage.y, 
		m_ptImage.x+this->m_imageIndexRect.Width() * IMG_SIZE, m_ptImage.y + this->m_imageIndexRect.Height() * IMG_SIZE );

	// 是否需要载入新的图片？
	while( rcImg.top > this->m_rectNewImage.top )
	{
		// 上边需要新的图片载入。
		this->m_imageIndexRect.InflateRect( 0, 1, 0, 0 );
		rcImg.InflateRect( 0, IMG_SIZE, 0, 0 );
		for( int i=this->m_imageIndexRect.left; i<this->m_imageIndexRect.right; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = i;
			imgIndex.y = this->m_imageIndexRect.top;

			this->LoadMapImage( imgIndex );				
		}
	}
	while( rcImg.left > this->m_rectNewImage.left )
	{
		// 左边需要新的图片载入。
		this->m_imageIndexRect.InflateRect( 1, 0, 0, 0 );
		rcImg.InflateRect( IMG_SIZE, 0, 0, 0 );
		for( int i=this->m_imageIndexRect.top; i<this->m_imageIndexRect.bottom; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = this->m_imageIndexRect.left;
			imgIndex.y = i;
			
			this->LoadMapImage( imgIndex );		
		}
	}
	while( rcImg.bottom < this->m_rectNewImage.bottom )
	{
		// 下边需要新的图片载入。		
		for( int i=this->m_imageIndexRect.left; i<this->m_imageIndexRect.right; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = i;
			imgIndex.y = this->m_imageIndexRect.bottom;
			
			this->LoadMapImage( imgIndex );		
		}
		this->m_imageIndexRect.InflateRect( 0, 0, 0, 1 );
		rcImg.InflateRect( 0, 0, 0, IMG_SIZE );
	}
	while( rcImg.right < this->m_rectNewImage.right )
	{
		// 右边需要新的图片载入。
		for( int i=this->m_imageIndexRect.top; i<this->m_imageIndexRect.bottom; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = this->m_imageIndexRect.right;
			imgIndex.y = i;
			
			this->LoadMapImage( imgIndex );		
		}
		this->m_imageIndexRect.InflateRect( 0, 0, 1, 0 );
		rcImg.InflateRect( 0, 0, IMG_SIZE, 0 );
	}
	

	// 是否需要销毁旧的图片？
	while( rcImg.top < this->m_rectDead.top )
	{
		// 削去上面的。
		for( int i=this->m_imageIndexRect.left; i<this->m_imageIndexRect.right; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = i;
			imgIndex.y = m_imageIndexRect.top;

			this->m_tableImageBuffer.erase( imgIndex );
		}

		this->m_imageIndexRect.DeflateRect( 0, 1, 0, 0 );
		rcImg.DeflateRect( 0, IMG_SIZE, 0, 0 );

	}
	while( rcImg.left < this->m_rectDead.left )
	{
		// 削去左边的。
		for( int i=this->m_imageIndexRect.top; i<this->m_imageIndexRect.bottom; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = m_imageIndexRect.left;
			imgIndex.y = i;

			this->m_tableImageBuffer.erase( imgIndex );
		}

		this->m_imageIndexRect.DeflateRect( 1, 0, 0, 0 );
		rcImg.DeflateRect( IMG_SIZE, 0, 0, 0 );
	}
	while( rcImg.right > this->m_rectDead.right )
	{
		// 削去右边

		this->m_imageIndexRect.DeflateRect( 0, 0, 1, 0 );
		rcImg.DeflateRect( 0, 0, IMG_SIZE, 0 );

		for( int i=this->m_imageIndexRect.top; i<this->m_imageIndexRect.bottom; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = m_imageIndexRect.right;
			imgIndex.y = i;

			this->m_tableImageBuffer.erase( imgIndex );
		}

		
	}
	while( rcImg.bottom > this->m_rectDead.bottom )
	{
		// 削去下边
		this->m_imageIndexRect.DeflateRect( 0, 0, 0, 1 );
		rcImg.DeflateRect( 0, 0, 0, IMG_SIZE );

		for( int i=this->m_imageIndexRect.left; i<this->m_imageIndexRect.right; ++i )
		{
			CImageIndex imgIndex;
			imgIndex.x = i;
			imgIndex.y = m_imageIndexRect.bottom;

			this->m_tableImageBuffer.erase( imgIndex );
		}

		
	}


	// 更新新的图片位置。
	this->m_ptImage = rcImg.TopLeft();
}

BOOL CMapCtrl::LoadMapImage( const CImageIndex& imgIndex )
{
	CString strImgPath = this->ImageIndex2ImagePath( this->m_nZLevel, imgIndex );

	PTImage pimg( new CImage() );
	HRESULT hr = pimg->Load( strImgPath );
	if( S_OK == hr )
	{
		this->m_tableImageBuffer[ imgIndex ] = pimg;
	}		

	return ( S_OK == hr );
}

CString CMapCtrl::ImageIndex2ImagePath( int zLevel, const CImageIndex& imgIndex ) const
{
	CString strPath;
	strPath.Format( _T( "J:\\GMapCatcher\\.googlemaps\\tiles\\%d\\%d\\%d\\%d\\%d.png" ), zLevel, ( imgIndex.x >> 10 ) % 1024, ( imgIndex.x % 1024 ), 
		( imgIndex.y >> 10 ) % 1024, imgIndex.y % 1024 );

	return strPath;
}

CCoord CMapCtrl::ClientArea2Coord( const CPoint& point ) const
{
	CCoord coord;
	coord.SetZLevel( this->m_nZLevel );

	// 先映射到图片上，属于哪个图片，偏移多少。定位到像素。
	// 相对图片左上角的位置。
	CPoint ptPos2Img = point - this->m_ptImage;

	// 相对世界地图左上角的像素距离。
	unsigned int nPixelPosX = this->m_imageIndexRect.left * IMG_SIZE + ptPos2Img.x;
	unsigned int nPixelPosY = this->m_imageIndexRect.top * IMG_SIZE + ptPos2Img.y;

	// 得到这个位置对应的图片序号。
//	int nImgIndexX = this->m_imageIndexRect.left + ptPos2Img.x / IMG_SIZE;
//	int nImgIndexY = this->m_imageIndexRect.top + ptPos2Img.y / IMG_SIZE;

	// 得到这个像素


	// 根据图片像素所在位置，计算经纬度。
//	coord.SetLatitude( nImgIndexX );
//	coord.SetLongitude( nImgIndexY );

	// 这个放大倍数下，整个世界一个方向上共有多少像素？
	unsigned int nWorldPixelNum = ( 1 << ( MAX_MAP_ZLEVEL - this->m_nZLevel ) ) * IMG_SIZE;

	// 经度是平均的，直接按比例求得。
	double longitude = ( ( double(nPixelPosX) / nWorldPixelNum ) * 2 - 1 ) * 180;

	// 纬度比较复杂，需要通过公式求得。 
	// Henry Bond 公式： F( lat ) = ln( tan( lat / 2 + pi / 4 ) )
	// 当前点到赤道的像素距离。
	unsigned int nPixel2Equator = nWorldPixelNum / 2 - nPixelPosY;

	// 比例
	double radioY = double( nPixel2Equator ) / ( nWorldPixelNum / 2 );

	double dexp = exp( radioY );

//	double latitude = ( atan( exp( radioY  ) ) - M_PI_4 ) * 2 * 180;

	//y = ( tile[0][1] + 1.0*tile[1][1]/TILES_HEIGHT) / (world_tiles/2.) - 1 # -1...1
 //   lon = x * 180.0
 //   y = math.exp(-y*2*math.pi)
 //   e = (y-1)/(y+1)
 //   lat = 180.0/math.pi * math.asin(e)
	double radioY2 = ( ( double(nPixelPosY) / nWorldPixelNum ) * 2 - 1 );
	double yy = exp( -radioY2 * 2 * M_PI );
	double ee = ( yy - 1 ) / ( yy + 1 );
	double latitude = 180.0 / M_PI * asin( ee );

//	latitude = lat;
	
	double checkAngle = ( latitude * M_PI / (180 * 2) + M_PI_4 );
	double checktan = tan( checkAngle );
	double latCheck = log( checktan );
	int test = ( nWorldPixelNum / 2 ) * latCheck;

	coord.SetLongitude( longitude );
	coord.SetLatitude( latitude );
	return coord;
}


void CMapCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CCoord coord = this->ClientArea2Coord( point );

	CString strMsg;
	strMsg.Format( _T( "latitude : %lf longitude : %lf " ), coord.GetLatitude(), coord.GetLongitude() );
	this->MessageBox( strMsg );

	CWnd::OnRButtonUp(nFlags, point);
}

void CMapCtrl::SetZLevel( int nZLevel )
{
	if( this->m_nZLevel != nZLevel )
	{
		// 获取旧的中心点经纬度，这个保持不变。
		CRect rcClient;
		this->GetClientRect( rcClient );
		CCoord oldCoord = this->ClientArea2Coord( rcClient.CenterPoint() );

		this->ClearImageBuffer();

		this->m_nZLevel = nZLevel;
		oldCoord.SetZLevel( nZLevel );
		// 重新按照经纬度中心定位。
		this->Move2Center( oldCoord );
	}
}

void CMapCtrl::ClearImageBuffer()
{
	this->m_tableImageBuffer.clear();
	this->m_ptImage = CPoint( 0, 0 );
	this->m_imageIndexRect = CRect( 0,0,0,0 );	
}

void CMapCtrl::Move2Center( const CCoord& center )
{
	if( center.GetZLevel() != this->m_nZLevel )
	{
		this->SetZLevel( center.GetZLevel() );
		return;
	}
	// 根据经纬度得到像素。
	CPoint ptPixel = this->Coord2ImagePixel( center );

	// 根据目标中心像素位置计算图片需要平移到什么地方。
	// 当前显示区域中心点对应的像素坐标
	CPoint ptImgLeftTop( this->m_imageIndexRect.left * IMG_SIZE, this->m_imageIndexRect.top * IMG_SIZE );
	CSize offset = this->m_rectShow.CenterPoint() - this->m_ptImage;
	CPoint ptClientCenterPixel = ptImgLeftTop + offset;

	// 如果图像移动不大，则直接移动。
	// 否则重新加载图片。
	CSize moveDistance = ptPixel - ptClientCenterPixel;

	if( abs( moveDistance.cx ) > this->m_imageIndexRect.Width() * IMG_SIZE
		|| abs( moveDistance.cy ) > this->m_imageIndexRect.Height() * IMG_SIZE )
	{
		// 重新加载图片。
		this->ClearImageBuffer();

		// 将图片序号调整到当前位置附近。
		this->m_imageIndexRect = CRect( ptPixel.x / IMG_SIZE, ptPixel.y / IMG_SIZE, ptPixel.x / IMG_SIZE, ptPixel.y / IMG_SIZE );

		ptImgLeftTop = CPoint( this->m_imageIndexRect.left * IMG_SIZE, this->m_imageIndexRect.top * IMG_SIZE );
		offset = this->m_rectShow.CenterPoint() - this->m_ptImage;
		ptClientCenterPixel = ptImgLeftTop + offset;

		// 如果图像移动不大，则直接移动。
		// 否则重新加载图片。
		moveDistance = ptPixel - ptClientCenterPixel;

		this->m_ptImage -= ( ptPixel - ptClientCenterPixel );		
		
	}
	else
	{
		this->m_ptImage -= ( ptPixel - ptClientCenterPixel );
	}	

	// 重新刷新图片缓存。
	this->UpdateImageBuffer();

	this->Invalidate();

}

CPoint CMapCtrl::Coord2ImagePixel( const CCoord& center ) const
{
	//world_tiles = tiles_on_level(coord[2])
 //   x = world_tiles / 360.0 * (coord[1] + 180.0)
 //   tiles_pre_radian = world_tiles / (2 * math.pi)
 //   e = math.sin(coord[0] * (1/180.*math.pi))
 //   y = world_tiles/2 + 0.5*math.log((1+e)/(1-e)) * (-tiles_pre_radian)
 //   offset = int((x - int(x)) * TILES_WIDTH), \
 //            int((y - int(y)) * TILES_HEIGHT)
 //   return (int(x) % world_tiles, int(y) % world_tiles), offset

	// 这个放大倍数下，整个世界一个方向上共有多少像素？
	unsigned int nWorldPixelNum = ( 1 << ( MAX_MAP_ZLEVEL - this->m_nZLevel ) ) * IMG_SIZE;

	int nX = double(nWorldPixelNum) / 360.0 * ( center.GetLongitude() + 180.0 );
	double pixelPerRadian = nWorldPixelNum / ( 2 * M_PI );
	double e = sin( center.GetLatitude() / 180.0 * M_PI );

	int nDistance2Equator = ( log( (1+e) / (1-e) ) ) / 2 * pixelPerRadian;
	int nY = nWorldPixelNum / 2 - nDistance2Equator;

	CPoint ptCenter( nX, nY );

	return ptCenter;
}
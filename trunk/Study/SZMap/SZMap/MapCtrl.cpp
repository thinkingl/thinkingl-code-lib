// MapCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "MapCtrl.h"
#include "szmapconfig.h"
#include <math.h>

#include "chinamapdeviationfix.h"
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
				strText.Format( _T( "�� ��û���ͼƬ��x: %d y: %d  " ), imgIndex.x, imgIndex.y );
				dc.TextOut( nDrawPosX + 10, nDrawPosY + 10, strText ); 
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

		CRect rcClient;
		this->GetClientRect( rcClient );
		this->m_centerCoord = this->ClientArea2Coord( rcClient.CenterPoint() );
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
		// ���ݾɵ���ʾ����
		CRect rectOldShow = this->m_rectShow;

		this->m_rectShow = CRect( 0,0,cx,cy );

		// ������ʾ���������ƫ���˶���λ�á�
		CSize offset = this->m_rectShow.CenterPoint() - rectOldShow.CenterPoint();

		// ͼƬ�������ƫ�ƣ�������֤���Ķ��롣
		this->m_ptImage += offset;

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

			this->LoadMapImage( imgIndex );				
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
			
			this->LoadMapImage( imgIndex );		
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
			
			this->LoadMapImage( imgIndex );		
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
			
			this->LoadMapImage( imgIndex );		
		}
		this->m_imageIndexRect.InflateRect( 0, 0, 1, 0 );
		rcImg.InflateRect( 0, 0, IMG_SIZE, 0 );
	}
	

	// �Ƿ���Ҫ���پɵ�ͼƬ��
	while( rcImg.top < this->m_rectDead.top )
	{
		// ��ȥ����ġ�
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
		// ��ȥ��ߵġ�
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
		// ��ȥ�ұ�

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
		// ��ȥ�±�
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


	// �����µ�ͼƬλ�á�
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
	CString strDefImgDir;
	::GetModuleFileName( NULL, strDefImgDir.GetBuffer( MAX_PATH ), MAX_PATH );
	strDefImgDir.ReleaseBuffer();
	int nPos = strDefImgDir.ReverseFind( '\\' );
	if ( nPos != -1 )
	{
		strDefImgDir = strDefImgDir.Left( nPos );
	}

	CString strImgDir = CSZMapConfig::GetMapImageDir().c_str();
	CString strImgSubDir = CSZMapConfig::GetMapSubDir().c_str();

	if ( strImgDir.Right( 1 ) != _T( "\\" ) )
	{
		strImgDir += _T( "\\" );
	}

	CString strPath;
	strPath.Format( _T( "%s\\%s\\%d\\%d\\%d\\%d\\%d.png" ), strImgDir, strImgSubDir, zLevel, ( imgIndex.x >> 10 ) % 1024, ( imgIndex.x % 1024 ), 
		( imgIndex.y >> 10 ) % 1024, imgIndex.y % 1024 );

	return strPath;
}

CCoord CMapCtrl::ClientArea2Coord( const CPoint& point ) const
{
	CCoord coord;
	coord.SetZLevel( this->m_nZLevel );

	// ��ӳ�䵽ͼƬ�ϣ������ĸ�ͼƬ��ƫ�ƶ��١���λ�����ء�
	// ���ͼƬ���Ͻǵ�λ�á�
	CPoint ptPos2Img = point - this->m_ptImage;

	// ��������ͼ���Ͻǵ����ؾ��롣
	unsigned int nPixelPosX = this->m_imageIndexRect.left * IMG_SIZE + ptPos2Img.x;
	unsigned int nPixelPosY = this->m_imageIndexRect.top * IMG_SIZE + ptPos2Img.y;

	// �õ����λ�ö�Ӧ��ͼƬ��š�
//	int nImgIndexX = this->m_imageIndexRect.left + ptPos2Img.x / IMG_SIZE;
//	int nImgIndexY = this->m_imageIndexRect.top + ptPos2Img.y / IMG_SIZE;

	// �õ��������


	// ����ͼƬ��������λ�ã����㾭γ�ȡ�
//	coord.SetLatitude( nImgIndexX );
//	coord.SetLongitude( nImgIndexY );

	// ����Ŵ����£���������һ�������Ϲ��ж������أ�
	unsigned int nWorldPixelNum = ( 1 << ( MAX_MAP_ZLEVEL - this->m_nZLevel ) ) * IMG_SIZE;

	// ������ƽ���ģ�ֱ�Ӱ�������á�
	double longitude = ( ( double(nPixelPosX) / nWorldPixelNum ) * 2 - 1 ) * 180;

	// γ�ȱȽϸ��ӣ���Ҫͨ����ʽ��á� 
	// Henry Bond ��ʽ�� F( lat ) = ln( tan( lat / 2 + pi / 4 ) )
	// ��ǰ�㵽��������ؾ��롣
	unsigned int nPixel2Equator = nWorldPixelNum / 2 - nPixelPosY;

	// ����
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CCoord coord = this->ClientArea2Coord( point );
	
	CChinaMapDeviationFix fixer;
	double earthLongitude, earthLatitude;
	fixer.MarsToEarth( coord.GetLongitude(), coord.GetLatitude(), earthLongitude, earthLatitude );

	// У��.
	double marsLongitudeTest, marsLatitudeTest;
	fixer.EarthToMars( earthLongitude, earthLatitude, marsLongitudeTest, marsLatitudeTest );
	double longitudeOffset = abs( marsLongitudeTest - coord.GetLongitude() );
	double latitudeOffset = abs( marsLatitudeTest - coord.GetLatitude() );



	CString strMsg;
	strMsg.Format( _T( "mars latitude : %lf mars longitude : %lf Earth : %lf, %lf \n\
			����������� : ��: %lf %d�� γ: %lf %d��" ), 
		coord.GetLatitude(), coord.GetLongitude() ,
		earthLatitude, earthLongitude,
		longitudeOffset, (int)longitudeOffset * 40000*1000 / 360,
		latitudeOffset, (int)latitudeOffset * 40000*1000 / 360
		);

	this->MessageBox( strMsg );

	CWnd::OnRButtonUp(nFlags, point);
}

void CMapCtrl::SetZLevel( int nZLevel )
{
	if( this->m_nZLevel != nZLevel )
	{

		this->ClearImageBuffer();

		this->m_nZLevel = nZLevel;
		this->m_centerCoord.SetZLevel( nZLevel );
		// ���°��վ�γ�����Ķ�λ��
		this->Move2Center( m_centerCoord );
	}
}

int CMapCtrl::GetZLevel() const
{
	return this->m_nZLevel;
}

void CMapCtrl::ClearImageBuffer()
{
	this->m_tableImageBuffer.clear();
	this->m_ptImage = CPoint( 0, 0 );
	this->m_imageIndexRect = CRect( 0,0,0,0 );	
}

void CMapCtrl::Move2Center( const CCoord& center )
{
	// ���澭γ�ȡ�
	this->m_centerCoord = center;

	if( center.GetZLevel() != this->m_nZLevel )
	{
		this->SetZLevel( center.GetZLevel() );
		return;
	}

	// ���ݾ�γ�ȵõ����ء�
	CPoint ptPixel = this->Coord2ImagePixel( center );

	// ����Ŀ����������λ�ü���ͼƬ��Ҫƽ�Ƶ�ʲô�ط���
	// ��ǰ��ʾ�������ĵ��Ӧ����������
	CPoint ptImgLeftTop( this->m_imageIndexRect.left * IMG_SIZE, this->m_imageIndexRect.top * IMG_SIZE );
	CSize offset = this->m_rectShow.CenterPoint() - this->m_ptImage;
	CPoint ptClientCenterPixel = ptImgLeftTop + offset;

	// ���ͼ���ƶ�������ֱ���ƶ���
	// �������¼���ͼƬ��
	CSize moveDistance = ptPixel - ptClientCenterPixel;

	if( abs( moveDistance.cx ) > this->m_imageIndexRect.Width() * IMG_SIZE
		|| abs( moveDistance.cy ) > this->m_imageIndexRect.Height() * IMG_SIZE )
	{
		// ���¼���ͼƬ��
		this->ClearImageBuffer();

		// ��ͼƬ��ŵ�������ǰλ�ø�����
		this->m_imageIndexRect = CRect( ptPixel.x / IMG_SIZE, ptPixel.y / IMG_SIZE, ptPixel.x / IMG_SIZE, ptPixel.y / IMG_SIZE );

		ptImgLeftTop = CPoint( this->m_imageIndexRect.left * IMG_SIZE, this->m_imageIndexRect.top * IMG_SIZE );
		this->m_ptImage = ptImgLeftTop - ptPixel + this->m_rectShow.CenterPoint();
		//offset = this->m_rectShow.CenterPoint() - this->m_ptImage;
		//ptClientCenterPixel = ptImgLeftTop + offset;

		//// ���ͼ���ƶ�������ֱ���ƶ���
		//// �������¼���ͼƬ��
		//moveDistance = ptPixel - ptClientCenterPixel;

		//this->m_ptImage -= ( ptPixel - ptClientCenterPixel );		
		
	}
	else
	{
		this->m_ptImage -= ( ptPixel - ptClientCenterPixel );
	}	

	// ����ˢ��ͼƬ���档
	this->UpdateImageBuffer();

	this->Invalidate();


}

CPoint CMapCtrl::Coord2ImagePixel( const CCoord& center ) const
{
	// ����Ŵ����£���������һ�������Ϲ��ж������أ�
	unsigned int nWorldPixelNum = ( 1 << ( MAX_MAP_ZLEVEL - this->m_nZLevel ) ) * IMG_SIZE;

	int nX = double(nWorldPixelNum) / 360.0 * ( center.GetLongitude() + 180.0 );
	double pixelPerRadian = nWorldPixelNum / ( 2 * M_PI );
	double sintmp = sin( center.GetLatitude() / 180.0 * M_PI );

	int nDistance2Equator = ( log( (1+sintmp) / (1-sintmp) ) ) / 2 * pixelPerRadian;
	int nY = nWorldPixelNum / 2 - nDistance2Equator;

	CPoint ptCenter( nX, nY );

	return ptCenter;
}
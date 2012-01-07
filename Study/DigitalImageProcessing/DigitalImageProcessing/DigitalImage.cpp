#include "DigitalImage.h"

#include "common.h"
#include "Matrix.h"
#include <assert.h>

CDigitalImage::CDigitalImage(void)
{
	this->m_intensityLevels = 0;
	this->m_width = 0;
	this->m_height = 0;
}


CDigitalImage::~CDigitalImage(void)
{
	this->Release();
}

bool CDigitalImage::Load( ctstring filePath )
{
	this->Release();

	CImage m_image;
	HRESULT hr = m_image.Load( filePath.c_str() );
	bool bOk = ( S_OK == hr );
	if ( bOk )
	{
		this->m_imagePath = filePath;

		void *pBuf = m_image.GetBits();
		int bpp = m_image.GetBPP();
		int pitch = m_image.GetPitch();
		int width = m_image.GetWidth();
		int height = m_image.GetHeight();
		
		this->m_imageDataBuf.resize( width*height, 0 );

		// 保存没像素字节数.
		// 这里先默认的认为载入的图片都是RGB表示的!
		// this->m_intensityLevels = bpp/3;
		// this->m_intensityLevels = min( 8, m_intensityLevels );	// 如果有透明通道的32bit图片, 亮度阶数仍然是8.
		switch( bpp )
		{
		case 8:
			this->m_imageType = DIT_Gray;
			this->m_intensityLevels = 8;
			break;
		case 24:
		case 32:
			this->m_imageType = DIT_RGB;
			this->m_intensityLevels = 8;
			break;
		default:
			assert( false );
			this->m_imageType = DIT_RGB;
			this->m_intensityLevels = 8;
			break;
		}

		bool needNetSequnce2HostSequence = ( htonl( 1 ) != 1 ) ;

		void *pData = m_image.GetBits();
		for ( int h=0; h<height; ++h )
		{
			void* lineBeginPos = (char*)pData + ( h * pitch );
			for ( int w=0; w<width; ++w )
			{
				unsigned int value = 0;
				int bytePP = bpp / 8;

				memcpy( &value, ( (char*)lineBeginPos + bytePP * w ), bytePP );		

				// 图片中是网络序的, 需要转字节序..
				if ( needNetSequnce2HostSequence )
				{
					unsigned int hostValue = 0;
					for( int i=0; i<bytePP; ++i )
					{
						hostValue += *( ( unsigned char *)&value + i );
						if( i<bytePP-1 )
						{
							hostValue <<= 8;
						}
					}
					
					value = hostValue;
				}
// 				int hValue = ntohl( value );
// 
// 				value = ( hValue >> (32-bpp) );	// 转序后需要再次位移....
// 
// 				int r= GetRValue( value );
// 				int g= GetGValue( value );
// 				int b= GetBValue( value );

// 				int kk = m_image.GetPixel( w, h );
// 
// 				int kkr= GetRValue( kk );
// 				int kkg= GetGValue( kk );
// 				int kkb= GetBValue( kk );

				this->m_imageDataBuf[ h*width + w ] = value;
			}
		}		
	
		// 保存宽度和高度.
		this->m_width = width;
		this->m_height = height;
		
	}
	return bOk;
}

CImage *CDigitalImage::GetImageDrawer() const
{
	if ( !this->IsLoaded() )
	{
		return NULL;
	}

	CImage *pImg = new CImage();

	int width = this->GetWidth();
	int height = this->GetHeight();
	int bpp = this->GetBitsPerPixel();

	// CImage 就是支持RGB的位图. 我们用支持透明色的32bit..
	const int DrawerIntensiveLevel = 8;
	const int DrawerBPP = DrawerIntensiveLevel * 4;
	BOOL bOk = pImg->Create( width, height, DrawerBPP );
	ASSERT( bOk );

	void *pBits = pImg->GetBits();
	int pitch = pImg->GetPitch();
	int drawerBpp = pImg->GetBPP();
	int bytePP = drawerBpp / 8;
	bool needNetSequnce2HostSequence = ( htonl( 1 ) != 1 ) ;

	for ( int h=0; h<height; ++h )
	{
		void *pLineBeginPos = (char*)pBits + h * pitch;
		for ( int w=0; w<width; ++w )
		{
			int value = this->m_imageDataBuf[ h * width + w];
			int pixel = 0;
			if ( DIT_RGB == this->m_imageType )
			{
				int r= GetRValue( value );
				int g= GetGValue( value );
				int b= GetBValue( value );

				// RGB模式时取单颜色的灰阶.
				r = this->IntensityTrans( r, m_intensityLevels, DrawerIntensiveLevel );
				g = this->IntensityTrans( g, m_intensityLevels, DrawerIntensiveLevel );
				b = this->IntensityTrans( b, m_intensityLevels, DrawerIntensiveLevel );

				pixel = RGB( r, g, b );
			}
			else if( DIT_Gray == this->m_imageType )
			{
				// 当前的灰度分辨率肯定小于等于8.
				int pixelIntensive = this->IntensityTrans( value, m_intensityLevels, DrawerIntensiveLevel );
				

				pixel = RGB( pixelIntensive, pixelIntensive, pixelIntensive );
			}

			// 图片里需要网络序的, 这里算法只能支持32bit图片.
			assert( 32 == drawerBpp );
			
			// 加上透明色, 直接唯一, 透明度都是0.
			pixel <<= 8;

			if ( needNetSequnce2HostSequence )
			{
				pixel = htonl( pixel );
			}

			((unsigned int*)pLineBeginPos)[ w ] = /*htonl*/( pixel );
		}
	}
	return pImg;
}

void CDigitalImage::Release()
{
	m_imageDataBuf.clear();
	m_intensityLevels = 0;
//	m_image.Destroy();
	m_imagePath.clear();
	this->m_width = 0;
	this->m_height = 0;
}

int CDigitalImage::GetWidth() const
{
	return this->m_width;
}

int CDigitalImage::GetHeight() const
{
	return this->m_height;
}

int CDigitalImage::GetBitsPerPixel() const
{
	int bpp = m_intensityLevels;
	if ( DIT_RGB == this->GetImageType() )
	{
		bpp = m_intensityLevels * 3;	// RGB三原色. 忽略透明通道.
	}

	return bpp;
}

bool CDigitalImage::IsLoaded() const
{
	return !this->m_imagePath.empty();
}

CDigitalImage *CDigitalImage::Clone() const
{
	// 新加的成员变量必须也要加在这里!!!
	CDigitalImage *pImg = new CDigitalImage();
	*pImg = *this;

	return pImg;
}

bool CDigitalImage::ToGrayscaleImageHSI( int intensityLevels )
{
	return this->RGB2Gray( R2G_HSI, intensityLevels );	
}

bool CDigitalImage::ToGrayscaleImageYUV( int intensityLevels )
{
	return this->RGB2Gray( R2G_YUV, intensityLevels );
}

int CDigitalImage::IntensityTrans( int oldIntensity, int oldGrayscaleLev, int newGrayscaleLev ) const
{
	if ( newGrayscaleLev > oldGrayscaleLev )
	{
		int newIntensity = ( oldIntensity * ((1<<newGrayscaleLev)-1) / ((1<<oldGrayscaleLev)-1) );
		return newIntensity;
	}
	else
	{
		return ( oldIntensity >> ( oldGrayscaleLev - newGrayscaleLev ) );
	}
	
}

tstring CDigitalImage::GetFilePath() const
{
	return this->m_imagePath;
}

int CDigitalImage::GetFileLength() const
{
	uint64 len = CCommon::GetFileLength( m_imagePath );
	ASSERT( len < MAXINT32 );
	return (int)len;
}

int CDigitalImage::GetIntensityLevel() const
{
	return m_intensityLevels;
}

CDigitalImage::EImageType CDigitalImage::GetImageType()const
{
	return this->m_imageType;
}

bool CDigitalImage::RGB2Gray( ERGB2GrayMode mod, int intensityLevels ) 
{
	if ( intensityLevels < 1 || intensityLevels > 8 )
	{
		TRACE( _T("ToGrayscale intensity level must between 1-8") );
		ASSERT( FALSE );
		return false;
	}

	if ( DIT_Gray == this->GetImageType() )
	{
		TRACE( _T( "RGB2Gray : image is gray ! " ) );
		return false;
	}

	for( int h=0; h<this->GetHeight(); ++h )
	{
		for( int w=0; w<this->GetWidth(); ++w )
		{
			int curValue = m_imageDataBuf[ h*this->GetWidth() + w ];
			int grayValue = 0;
			if ( this->m_imageType == DIT_RGB )
			{
				int R= GetRValue( curValue );
				int G= GetGValue( curValue );
				int B= GetBValue( curValue );
				if ( R2G_HSI == mod )
				{
					// RGB 灰度按照 HSI的方式转换 见书 P. 433
					grayValue = (  R+G+B )/3;
				}
				else
				{
					// YUV 的灰度加权不同. Y=0.212671*R + 0.715160*G + 0.072169*B
					grayValue = (int)( 0.212671*R + 0.715160*G + 0.072169*B );
				}			
			}
			else if( this->m_imageType == DIT_Gray )
			{
				grayValue = curValue;
			}

			// 当前灰度转换为目标灰度分辨率.
			grayValue = this->IntensityTrans( grayValue, m_intensityLevels, intensityLevels );			
			m_imageDataBuf[ h*this->GetWidth() + w ] = grayValue;
		}
	}
	this->m_imageType = DIT_Gray;
	this->m_intensityLevels = intensityLevels;
	return true;
}

bool CDigitalImage::SetIntensityLevel( int intensityLevels )
{
	if ( intensityLevels < 1 || intensityLevels > 8 )
	{
		TRACE( _T("SetIntensityLevel intensity level must between 1-8") );
		ASSERT( FALSE );
		return false;
	}
	
	for( int h=0; h<this->GetHeight(); ++h )
	{
		for( int w=0; w<this->GetWidth(); ++w )
		{
			int curValue = m_imageDataBuf[ h*this->GetWidth() + w ];
			int pixelValue = 0;
			if ( this->m_imageType == DIT_RGB )
			{
				int r= GetRValue( curValue );
				int g= GetGValue( curValue );
				int b= GetBValue( curValue );

				r = this->IntensityTrans( r, m_intensityLevels, intensityLevels );
				g = this->IntensityTrans( g, m_intensityLevels, intensityLevels );
				b = this->IntensityTrans( b, m_intensityLevels, intensityLevels );

				pixelValue = RGB( r, g, b );
			}
			else if( this->m_imageType == DIT_Gray )
			{
				pixelValue = curValue;

				pixelValue = this->IntensityTrans( pixelValue, m_intensityLevels, intensityLevels );		
			}

			m_imageDataBuf[ h*this->GetWidth() + w ] = pixelValue;
		}
	}

	// 修改每像素的字节数.
	this->m_intensityLevels = intensityLevels;
	return true;
}

bool CDigitalImage::ImageNegitive()
{
	// 公式: s = L-1-r 
	int L = ( 1 << this->m_intensityLevels );

	for( int h=0; h<this->GetHeight(); ++h )
	{
		for( int w=0; w<this->GetWidth(); ++w )
		{
			int curValue = m_imageDataBuf[ h*this->GetWidth() + w ];
			int pixelValue = 0;
			if ( this->m_imageType == DIT_RGB )
			{
				int r= GetRValue( curValue );
				int g= GetGValue( curValue );
				int b= GetBValue( curValue );

				r = L-1-r;
				g = L-1-g;
				b = L-1-b;

				pixelValue = RGB( r, g, b );
			}
			else if( this->m_imageType == DIT_Gray )
			{
				pixelValue = L-1-curValue;
			}
			m_imageDataBuf[ h*this->GetWidth() + w ] = pixelValue;
		}
	}
	return true;
}

bool CDigitalImage::IntensityLogTransform( double c )
{
	//  s = c * Log( 1+r )
	for( int h=0; h<this->GetHeight(); ++h )
	{
		for( int w=0; w<this->GetWidth(); ++w )
		{
			int curValue = m_imageDataBuf[ h*this->GetWidth() + w ];
			int pixelValue = 0;
			if ( this->m_imageType == DIT_RGB )
			{
				int r= GetRValue( curValue );
				int g= GetGValue( curValue );
				int b= GetBValue( curValue );

				r = (int)( c * log( (double)1+r ) );
				g = (int)( c * log( (double)1+g ) );
				b = (int)( c * log( (double)1+b ) );

				pixelValue = RGB( r, g, b );
			}
			else if( this->m_imageType == DIT_Gray )
			{
				pixelValue = (int)( c * log( (double)1+curValue ) );
			}
			m_imageDataBuf[ h*this->GetWidth() + w ] = pixelValue;
		}
	}
	return true;
}

bool CDigitalImage::IntensityPowerTransform( double c, double v )
{
	//   s = c * r ^ v
	int maxValue = ( 1 << m_intensityLevels )-1; 
	for( int h=0; h<this->GetHeight(); ++h )
	{
		for( int w=0; w<this->GetWidth(); ++w )
		{
			int curValue = m_imageDataBuf[ h*this->GetWidth() + w ];
			int pixelValue = 0;
			if ( this->m_imageType == DIT_RGB )
			{
				int r= GetRValue( curValue );
				int g= GetGValue( curValue );
				int b= GetBValue( curValue );

				r = (int)( c * pow( r, v ) );
				g = (int)( c * pow( g, v ) );
				b = (int)( c * pow( b, v ) );

				r = min( maxValue, r );
				g = min( maxValue, g );
				b = min( maxValue, b );
				pixelValue = RGB( r, g, b );
			}
			else if( this->m_imageType == DIT_Gray )
			{
				pixelValue = (int)( c * pow( curValue, v ) );				
				pixelValue = min( maxValue, pixelValue );
			}
			m_imageDataBuf[ h*this->GetWidth() + w ] = pixelValue;
		}
	}
	return true;
}

bool CDigitalImage::IntensityBitPlanesReconstruct( uint8 bitMask )
{
	for( int h=0; h<this->GetHeight(); ++h )
	{
		for( int w=0; w<this->GetWidth(); ++w )
		{
			int curValue = m_imageDataBuf[ h*this->GetWidth() + w ];
			int pixelValue = 0;
			if ( this->m_imageType == DIT_RGB )
			{
				int r= GetRValue( curValue );
				int g= GetGValue( curValue );
				int b= GetBValue( curValue );

				r &= bitMask;
				g &= bitMask;
				b &= bitMask;
								
				pixelValue = RGB( r, g, b );
			}
			else if( this->m_imageType == DIT_Gray )
			{
				pixelValue = curValue & bitMask;
			}
			m_imageDataBuf[ h*this->GetWidth() + w ] = pixelValue;
		}
	}
	return true;
}

bool CDigitalImage::IntensityBitPlanesOneBit( int bitIndex )
{
	int bitMask = 1 << bitIndex;
	for( int h=0; h<this->GetHeight(); ++h )
	{
		for( int w=0; w<this->GetWidth(); ++w )
		{
			int curValue = m_imageDataBuf[ h*this->GetWidth() + w ];
			int pixelValue = 0;
			if ( this->m_imageType == DIT_RGB )
			{
				int r= GetRValue( curValue );
				int g= GetGValue( curValue );
				int b= GetBValue( curValue );

				r = ( ( r&bitMask ) == 0 ? 0 : 1 );
				g = ( ( g&bitMask ) == 0 ? 0 : 1 );
				g = ( ( g&bitMask ) == 0 ? 0 : 1 );
				pixelValue = RGB( r, g, b );
			}
			else if( this->m_imageType == DIT_Gray )
			{
				pixelValue = ( ( curValue&bitMask ) == 0 ? 0:1 );
			}
			m_imageDataBuf[ h*this->GetWidth() + w ] = pixelValue;
		}
	}
	
	// 黑白双色. 只有0和1.
	this->m_intensityLevels = 1;

	return true;
}

bool CDigitalImage::GetHistogramData( THistogramData& grayHistoram, THistogramData& rh, THistogramData& gh, THistogramData& bh )const
{
	int lvl = this->GetIntensityLevel();
	int rs = 1 << lvl;

	grayHistoram.resize( rs, 0 );
	rh = gh = bh = grayHistoram;

	for( int h=0; h<this->GetHeight(); ++h )
	{
		for( int w=0; w<this->GetWidth(); ++w )
		{
			int curValue = m_imageDataBuf[ h*this->GetWidth() + w ];			
			if ( this->m_imageType == DIT_RGB )
			{
				int r= GetRValue( curValue );
				int g= GetGValue( curValue );
				int b= GetBValue( curValue );

				rh[ r ] ++;
				gh[ g ] ++;
				bh[ b ] ++;

				grayHistoram[ int((double)(r+g+b)/3 + 0.5) ] ++;
			}
			else if( this->m_imageType == DIT_Gray )
			{
				grayHistoram[ curValue ] ++;
			}
		}
	}
	return true;	
}

bool CDigitalImage::HistogramEqualization()
{
	// 先获取直方图数据.
	THistogramData grayHistogram, rh, gh, bh;
	this->GetHistogramData( grayHistogram, rh, gh, bh );
	// 总像素.
	int totalPixelNum = this->GetWidth() * this->GetHeight();
	// 最大亮度.
	int maxIntensity = grayHistogram.size() - 1;
	// 积分.
	for ( size_t i=1; i<grayHistogram.size(); ++i )
	{
		grayHistogram[i] += grayHistogram[i-1];
	}

	// 新的亮度表.
	typedef std::vector<int> TIntArray;
	TIntArray intensityTable(grayHistogram.size());
	typedef std::vector<double> TDbArray;
	TDbArray intensityRadioTable(grayHistogram.size() );
	for ( size_t i=0; i<grayHistogram.size(); ++i )
	{
		intensityTable[i] = (int)( ((double)grayHistogram[i] * maxIntensity / totalPixelNum ) + 0.5 ) ;
		intensityRadioTable[i] = (double)intensityTable[i] / max( 0.000001, i );
	}

	int aaaa = 0;
	int bbbb = 0;
	// 遍历所有像素.
	for( int h=0; h<this->GetHeight(); ++h )
	{
		for( int w=0; w<this->GetWidth(); ++w )
		{
			int curValue = m_imageDataBuf[ h*this->GetWidth() + w ];	
			int newValue = curValue;
			if ( this->m_imageType == DIT_RGB )
			{
				int r= GetRValue( curValue );
				int g= GetGValue( curValue );
				int b= GetBValue( curValue );

				int intensity = (int)((double)( r+g+b ) / 3 + 0.5);
				double radio = intensityRadioTable[ intensity ];

				r = intensityTable[ r ];
				g = intensityTable[ g ];
				b = intensityTable[ b ];

				newValue = RGB( r,g,b );	
				
				newValue = RGB( r*radio + 0.5, g*radio + 0.5, b*radio + 0.5 );
			}
			else if( this->m_imageType == DIT_Gray )
			{
				newValue = intensityTable[ curValue ];

			}
			m_imageDataBuf[  h*this->GetWidth() + w  ] = newValue;
		}
	}
	return true;
}

void CDigitalImage::Rotate( double angle, int rotationX, int rotationY, EInterpolateType interpolateType )
{
	// 将角度从0-360转换为弧度.
	angle = angle * 3.14159265 / 180;
	// 课本 P.110
	CMatrix<double> rotateMarix( 3, 3, 0, 1 );
	rotateMarix.Value( 0, 0 ) = cos( angle );
	rotateMarix.Value( 1, 0 ) = sin( angle );
	rotateMarix.Value( 0, 1 ) = -sin( angle );
	rotateMarix.Value( 1, 1 ) = cos( angle );
	rotateMarix.Value( 2, 2 ) = 1;

	this->InverseAffineTransform( rotateMarix, interpolateType, rotationX, rotationY, GetWidth(), GetHeight() );
	return;
	// 用逆矩阵, 求 仿射 的逆运算. 
	CMatrix<double> inverseRotateMatrix;
	rotateMarix.GetInverse( inverseRotateMatrix );

	TImageDataBuf newImg( this->GetWidth() * this->GetHeight(), 0 );

	// 确定新图片的像素点..
	for ( int xInNewImg = 0; xInNewImg<this->GetWidth(); ++xInNewImg )
	{
		for ( int y=0; y<this->GetHeight(); ++y )
		{
			// 先转换坐标原点.
			int transNewX = xInNewImg - rotationX;
			int transNewY = y - rotationY;

			CMatrix<double> originalCoordinateMatrix( 3, 1, 0, 1 );
			originalCoordinateMatrix.Value( 0, 0 ) = transNewX;
			originalCoordinateMatrix.Value( 1, 0 ) = transNewY;
			originalCoordinateMatrix.Value( 2, 0 ) = 1;

			CMatrix<double> transformedCoord = originalCoordinateMatrix * inverseRotateMatrix;

			// 精确的原始坐标点. 去掉旋转轴对坐标原点的偏移.
			double transX = transformedCoord.Value( 0, 0 ) + rotationX;
			double transY = transformedCoord.Value( 1, 0 ) + rotationY;

			// 采用不同的插值算法.
			int xOriginal = -1;
			int yOriginal = -1;
			if ( CDigitalImage::InterpolateNearestNeighbor == interpolateType )
			{
				// 最近的点, 四舍五入.
				xOriginal = int( transX + 0.5 );
				yOriginal = int( transY + 0.5 );

				int indexOriginal = yOriginal * this->GetWidth() + xOriginal;
				if ( xOriginal >= 0 && xOriginal < this->GetWidth() && yOriginal >= 0 && yOriginal < this->GetHeight() )
				{
					newImg[ xInNewImg + y*GetWidth() ] = m_imageDataBuf[ indexOriginal ];
				}	
			}
			else if( InterpolateBilinear == interpolateType )
			{
				// 双线性插值. http://en.wikipedia.org/wiki/Bilinear_interpolation
				int xLess = (int)transX;
				int yLess = (int)transY;
								
				if( xLess > 0 && xLess < GetWidth()-1 && yLess > 0 && yLess < GetHeight()-1 )
				{
					double topValueTmp = 0;
					double bottomValueTmp = 0;
					// 求上面的两个点 (xLess, yLess) 和 (xLess+1, yLess)之间的插值.
					topValueTmp = this->Pixel( xLess, yLess ) * ( xLess + 1 - transX ) + this->Pixel( xLess + 1, yLess ) * ( transX - xLess );
					// 求下面的两个点 (xLess, yLess+1) 和 ( xLess+1, yLess+1 )之间的插值.
					bottomValueTmp = this->Pixel( xLess, yLess+1 ) * ( xLess + 1 - transX ) + this->Pixel( xLess + 1, yLess + 1 ) * ( transX - xLess );

					// 将上面两个点之间插值.
					double finalValue = topValueTmp * ( yLess + 1 - transY ) + bottomValueTmp * ( transY - yLess );

					// 赋值.
					newImg[ xInNewImg+y*GetWidth() ] = (int)(finalValue+0.5);
				}

				
			}

					
		}
	}

	m_imageDataBuf = newImg;
}

void CDigitalImage::Translate( int coordX, int coordY )
{
	TImageDataBuf newImg( this->GetWidth() * this->GetHeight(), 0 );

	// 确定新图片的像素点..
	for ( int x = 0; x<this->GetWidth(); ++x )
	{
		for ( int y=0; y<this->GetHeight(); ++y )
		{
			// 先转换坐标原点.			
			int xTrans = x + coordX;
			int yTrans = y + coordY;

			int indexTrans = yTrans * this->GetWidth() + xTrans;
			if ( xTrans >= 0 && xTrans < this->GetWidth() && yTrans >= 0 && yTrans < this->GetHeight() )
			{
				newImg[ indexTrans ] = m_imageDataBuf[ y*this->GetWidth()+x ];
			}			
		}
	}

	m_imageDataBuf = newImg;
}

uint32& CDigitalImage::Pixel( int x, int y )
{
	return m_imageDataBuf[ x + GetWidth() * y ];
}

void CDigitalImage::Scale( double multipleX, double multipleY, EInterpolateType interpolateType )
{
	// affine 
	// 课本 P.110
	CMatrix<double> scaleMarix( 3, 3, 0, 1 );
	scaleMarix.Value( 0, 0 ) = (double)1/multipleX;	
	scaleMarix.Value( 1, 1 ) = (double)1/multipleY;
	scaleMarix.Value( 2, 2 ) = 1;

	this->InverseAffineTransform( scaleMarix, interpolateType, 0, 0		//, GetWidth(), GetHeight() );
		, (int)( GetWidth() * multipleX + 0.999 ), (int)( GetHeight() * multipleY + 0.999 ) );

}

void CDigitalImage::InverseAffineTransform( const CMatrix<double>& affineMatrix, EInterpolateType interpolateType
	, int transformOriginX, int transformOriginY, int newWidth, int newHeight )
{
	// 用逆矩阵, 求 仿射 的逆运算. 
	CMatrix<double> inverseAffineMatrix;
	affineMatrix.GetInverse( inverseAffineMatrix );

	TImageDataBuf newImg( newWidth * newHeight, 0 );

	// 确定新图片的像素点..
	for ( int xInNewImg = 0; xInNewImg<newWidth; ++xInNewImg )
	{
		for ( int yInNewImg=0; yInNewImg<newHeight; ++yInNewImg )
		{
			// 先转换坐标原点.
			int transNewX = xInNewImg - transformOriginX;
			int transNewY = yInNewImg - transformOriginY;

			CMatrix<double> originalCoordinateMatrix( 3, 1, 0, 1 );
			originalCoordinateMatrix.Value( 0, 0 ) = transNewX;
			originalCoordinateMatrix.Value( 1, 0 ) = transNewY;
			originalCoordinateMatrix.Value( 2, 0 ) = 1;

			CMatrix<double> transformedCoord = originalCoordinateMatrix * affineMatrix;

			// 精确的原始坐标点. 去掉旋转轴对坐标原点的偏移.
			double transX = transformedCoord.Value( 0, 0 ) + transformOriginX;
			double transY = transformedCoord.Value( 1, 0 ) + transformOriginY;

			// 采用不同的插值算法.
			int xOriginal = -1;
			int yOriginal = -1;
			if ( CDigitalImage::InterpolateNearestNeighbor == interpolateType )
			{
				// 最近的点, 四舍五入.
				xOriginal = int( transX + 0.5 );
				yOriginal = int( transY + 0.5 );

				int indexOriginal = yOriginal * this->GetWidth() + xOriginal;
				if ( xOriginal >= 0 && xOriginal < this->GetWidth() && yOriginal >= 0 && yOriginal < this->GetHeight() )
				{
					newImg[ xInNewImg + yInNewImg* newWidth ] = m_imageDataBuf[ indexOriginal ];
				}	
			}
			else if( InterpolateBilinear == interpolateType )
			{
				// 双线性插值. http://en.wikipedia.org/wiki/Bilinear_interpolation
				int xLess = (int)transX;
				int yLess = (int)transY;

				if( xLess > 0 && xLess < GetWidth()-1 && yLess > 0 && yLess < GetHeight()-1 )
				{
					double topValueTmp = 0;
					double bottomValueTmp = 0;
					// 求上面的两个点 (xLess, yLess) 和 (xLess+1, yLess)之间的插值.
					topValueTmp = this->Pixel( xLess, yLess ) * ( xLess + 1 - transX ) + this->Pixel( xLess + 1, yLess ) * ( transX - xLess );
					// 求下面的两个点 (xLess, yLess+1) 和 ( xLess+1, yLess+1 )之间的插值.
					bottomValueTmp = this->Pixel( xLess, yLess+1 ) * ( xLess + 1 - transX ) + this->Pixel( xLess + 1, yLess + 1 ) * ( transX - xLess );

					// 将上面两个点之间插值.
					double finalValue = topValueTmp * ( yLess + 1 - transY ) + bottomValueTmp * ( transY - yLess );

					// 赋值.
					newImg[ xInNewImg+yInNewImg* newWidth ] = (int)(finalValue+0.5);
				}


			}


		}
	}

	// 保存结果..
	m_imageDataBuf = newImg;
	this->m_width = newWidth;
	this->m_height = newHeight;
}

double CDigitalImage::GaussianRandom(double u,double g /*,double *r*/ ) 
{
	// 网上找的, 看不懂...
	double r = rand();	
	int i,m; 
	double s,w,v,t; 
	s=65536.0; w=2053.0; v=13849.0; 
	t=0.0; 
	for (i=1; i<=12; i++) 
	{
		r=(r)*w+v; m=(int)(r/s);
		r=r-m*s; t=t+(r)/s; 
	} 
	t=u+g*(t-6.0); 
	return(t); 
}

void CDigitalImage::AddGaussianNoise( double noiseMean, double noiseVariance )
{	
	for ( size_t i=0; i<m_imageDataBuf.size(); ++i )
	{
		int rs = (int)GaussianRandom( noiseMean, noiseVariance );

		int maxCor = (1 << this->GetIntensityLevel()) - 1;
		int value = m_imageDataBuf[i];
		if ( DIT_Gray == this->GetImageType() )
		{			
			value += rs;
			value = max( 0, value );						
			value = min( maxCor, value );			
		}
		else if( DIT_RGB == this->GetImageType() )
		{
			int r= GetRValue( value );
			int g= GetGValue( value );
			int b= GetBValue( value );

			r += rs;
			g += rs;
			b += rs;

			r = max( 0, r );
			g = max( 0, g );
			b = max( 0, b );

			r = min( maxCor, r );
			g = min( maxCor, g );
			b = min( maxCor, b );

			value = RGB( r,g,b );
		}
		m_imageDataBuf[i] = value;
	}
}

void CDigitalImage::AddSaltAndPepperNoise( int pixelNumPerSalt, int pixelNumPerPepper )
{
	for ( size_t i=0; i<m_imageDataBuf.size(); ++i )
	{
		int randomNum = rand();
		bool salt = ( randomNum % ( pixelNumPerSalt-1 ) ) == 0;

		randomNum = rand();
		bool pepper = ( randomNum % ( pixelNumPerPepper-1 ) ) == 0;

		int maxCor = (1 << this->GetIntensityLevel()) - 1;
		uint32 value = m_imageDataBuf[i];
		if ( DIT_Gray == this->GetImageType() )
		{			
			value = salt ? maxCor : value;
			value = pepper ? 0 : value;
		}
		else if( DIT_RGB == this->GetImageType() )
		{
			value = salt ? RGB( maxCor, maxCor, maxCor ) : value;
			value = pepper ? 0 : value;
		}
		m_imageDataBuf[i] = value;
	}
}

void CDigitalImage::SpatialMeanFilter( int maskSize )
{
	CMatrix<int> meanMask( maskSize, maskSize, 0, 1 );
	for ( int x=0; x<maskSize; ++x )
	{
		for( int y=0; y<maskSize; ++y )
		{
			meanMask.Value( x, y ) = 1;
		}
	}
	this->SpatialFilter( meanMask );
}

void CDigitalImage::SpatialFilter( const CMatrix<int>& filterMask )
{
	assert( filterMask.GetWidth() == filterMask.GetHeight() );

	int width = GetWidth();
	int height = GetHeight();
	int maskSize = filterMask.GetWidth();

	for( int x=0; x<width; ++x )
	{
		for( int y=0; y<height; ++y )
		{
			//  在这一点上应用滤镜, 求滤镜范围内的均值.
			int value = 0;
			int multipleCount = 0;
			for ( int filterX=0; filterX<maskSize; ++filterX )
			{
				for( int filterY=0; filterY<maskSize; ++filterY )
				{
					int imgX = x - maskSize/2 + filterX;
					int imgY = y - maskSize/2 + filterY;
					if( this->IsValidCoord( imgX, imgY, width, height ) )
					{
						int multiple = filterMask.Value( filterX, filterY );
						value +=  multiple * this->Pixel( imgX, imgY );
						multipleCount += multiple;
					}
				}
			}

			value /= multipleCount;
			this->Pixel( x, y ) = value;
		}
	}
}

bool CDigitalImage::IsValidCoord( int x, int y, int width, int height )
{
	return ( x >= 0 && x < width && y >= 0 && y < height );
}

void CDigitalImage::SpatialMedianFilter( int maskSize )
{
	int width = GetWidth();
	int height = GetHeight();

	for( int x=0; x<width; ++x )
	{
		for( int y=0; y<height; ++y )
		{
			//  在这一点上应用滤镜, 求滤镜范围内的中值
			
			int pixelNum = 0;
			TImageDataBuf sortBuf;
			for ( int filterX=0; filterX<maskSize; ++filterX )
			{
				for( int filterY=0; filterY<maskSize; ++filterY )
				{
					int imgX = x - maskSize/2 + filterX;
					int imgY = y - maskSize/2 + filterY;
					if( this->IsValidCoord( imgX, imgY, width, height ) )
					{
						sortBuf.push_back( this->Pixel( imgX, imgY ) );
					}
				}
			}

			std::sort( sortBuf.begin(), sortBuf.end() );
			int value = sortBuf[ sortBuf.size()/2 ];	// 中间值.
			this->Pixel( x, y ) = value;
		}
	}
}
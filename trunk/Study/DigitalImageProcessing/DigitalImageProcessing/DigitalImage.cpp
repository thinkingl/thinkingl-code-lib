#include "DigitalImage.h"

#include "common.h"
#include "Matrix.h"
#include <assert.h>
#include <complex>
#include "dipconst.h"

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

		// ����û�����ֽ���.
		// ������Ĭ�ϵ���Ϊ�����ͼƬ����RGB��ʾ��!
		// this->m_intensityLevels = bpp/3;
		// this->m_intensityLevels = min( 8, m_intensityLevels );	// �����͸��ͨ����32bitͼƬ, ���Ƚ�����Ȼ��8.
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

				// ͼƬ�����������, ��Ҫת�ֽ���..
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
// 				value = ( hValue >> (32-bpp) );	// ת�����Ҫ�ٴ�λ��....
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
	
		// �����Ⱥ͸߶�.
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

	// CImage ����֧��RGB��λͼ. ������֧��͸��ɫ��32bit..
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

				// RGBģʽʱȡ����ɫ�Ļҽ�.
				r = this->IntensityTrans( r, m_intensityLevels, DrawerIntensiveLevel );
				g = this->IntensityTrans( g, m_intensityLevels, DrawerIntensiveLevel );
				b = this->IntensityTrans( b, m_intensityLevels, DrawerIntensiveLevel );

				pixel = RGB( r, g, b );
			}
			else if( DIT_Gray == this->m_imageType )
			{
				// ��ǰ�ĻҶȷֱ��ʿ϶�С�ڵ���8.
				int pixelIntensive = this->IntensityTrans( value, m_intensityLevels, DrawerIntensiveLevel );
				

				pixel = RGB( pixelIntensive, pixelIntensive, pixelIntensive );
			}

			// ͼƬ����Ҫ�������, �����㷨ֻ��֧��32bitͼƬ.
			assert( 32 == drawerBpp );
			
			// ����͸��ɫ, ֱ��Ψһ, ͸���ȶ���0.
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
		bpp = m_intensityLevels * 3;	// RGB��ԭɫ. ����͸��ͨ��.
	}

	return bpp;
}

bool CDigitalImage::IsLoaded() const
{
	return !this->m_imagePath.empty();
}

CDigitalImage *CDigitalImage::Clone() const
{
	// �¼ӵĳ�Ա��������ҲҪ��������!!!
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
					// RGB �ҶȰ��� HSI�ķ�ʽת�� ���� P. 433
					grayValue = (  R+G+B )/3;
				}
				else
				{
					// YUV �ĻҶȼ�Ȩ��ͬ. Y=0.212671*R + 0.715160*G + 0.072169*B
					grayValue = (int)( 0.212671*R + 0.715160*G + 0.072169*B );
				}			
			}
			else if( this->m_imageType == DIT_Gray )
			{
				grayValue = curValue;
			}

			// ��ǰ�Ҷ�ת��ΪĿ��Ҷȷֱ���.
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

	// �޸�ÿ���ص��ֽ���.
	this->m_intensityLevels = intensityLevels;
	return true;
}

bool CDigitalImage::ImageNegitive()
{
	// ��ʽ: s = L-1-r 
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
	
	// �ڰ�˫ɫ. ֻ��0��1.
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
	// �Ȼ�ȡֱ��ͼ����.
	THistogramData grayHistogram, rh, gh, bh;
	this->GetHistogramData( grayHistogram, rh, gh, bh );
	// ������.
	int totalPixelNum = this->GetWidth() * this->GetHeight();
	// �������.
	int maxIntensity = grayHistogram.size() - 1;
	// ����.
	for ( size_t i=1; i<grayHistogram.size(); ++i )
	{
		grayHistogram[i] += grayHistogram[i-1];
	}

	// �µ����ȱ�.
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
	// ������������.
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
	// ���Ƕȴ�0-360ת��Ϊ����.
	angle = angle * PI / 180;
	// �α� P.110
	CMatrix<double> rotateMarix( 3, 3, 0, 1 );
	rotateMarix.Value( 0, 0 ) = cos( angle );
	rotateMarix.Value( 1, 0 ) = sin( angle );
	rotateMarix.Value( 0, 1 ) = -sin( angle );
	rotateMarix.Value( 1, 1 ) = cos( angle );
	rotateMarix.Value( 2, 2 ) = 1;

	this->InverseAffineTransform( rotateMarix, interpolateType, rotationX, rotationY, GetWidth(), GetHeight() );
	return;
	// �������, �� ���� ��������. 
	CMatrix<double> inverseRotateMatrix;
	rotateMarix.GetInverse( inverseRotateMatrix );

	TImageDataBuf newImg( this->GetWidth() * this->GetHeight(), 0 );

	// ȷ����ͼƬ�����ص�..
	for ( int xInNewImg = 0; xInNewImg<this->GetWidth(); ++xInNewImg )
	{
		for ( int y=0; y<this->GetHeight(); ++y )
		{
			// ��ת������ԭ��.
			int transNewX = xInNewImg - rotationX;
			int transNewY = y - rotationY;

			CMatrix<double> originalCoordinateMatrix( 3, 1, 0, 1 );
			originalCoordinateMatrix.Value( 0, 0 ) = transNewX;
			originalCoordinateMatrix.Value( 1, 0 ) = transNewY;
			originalCoordinateMatrix.Value( 2, 0 ) = 1;

			CMatrix<double> transformedCoord = originalCoordinateMatrix * inverseRotateMatrix;

			// ��ȷ��ԭʼ�����. ȥ����ת�������ԭ���ƫ��.
			double transX = transformedCoord.Value( 0, 0 ) + rotationX;
			double transY = transformedCoord.Value( 1, 0 ) + rotationY;

			// ���ò�ͬ�Ĳ�ֵ�㷨.
			int xOriginal = -1;
			int yOriginal = -1;
			if ( CDigitalImage::InterpolateNearestNeighbor == interpolateType )
			{
				// ����ĵ�, ��������.
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
				// ˫���Բ�ֵ. http://en.wikipedia.org/wiki/Bilinear_interpolation
				int xLess = (int)transX;
				int yLess = (int)transY;
								
				if( xLess > 0 && xLess < GetWidth()-1 && yLess > 0 && yLess < GetHeight()-1 )
				{
					double topValueTmp = 0;
					double bottomValueTmp = 0;
					// ������������� (xLess, yLess) �� (xLess+1, yLess)֮��Ĳ�ֵ.
					topValueTmp = this->Pixel( xLess, yLess ) * ( xLess + 1 - transX ) + this->Pixel( xLess + 1, yLess ) * ( transX - xLess );
					// ������������� (xLess, yLess+1) �� ( xLess+1, yLess+1 )֮��Ĳ�ֵ.
					bottomValueTmp = this->Pixel( xLess, yLess+1 ) * ( xLess + 1 - transX ) + this->Pixel( xLess + 1, yLess + 1 ) * ( transX - xLess );

					// ������������֮���ֵ.
					double finalValue = topValueTmp * ( yLess + 1 - transY ) + bottomValueTmp * ( transY - yLess );

					// ��ֵ.
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

	// ȷ����ͼƬ�����ص�..
	for ( int x = 0; x<this->GetWidth(); ++x )
	{
		for ( int y=0; y<this->GetHeight(); ++y )
		{
			// ��ת������ԭ��.			
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
	// �α� P.110
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
	// �������, �� ���� ��������. 
	CMatrix<double> inverseAffineMatrix;
	affineMatrix.GetInverse( inverseAffineMatrix );

	TImageDataBuf newImg( newWidth * newHeight, 0 );

	// ȷ����ͼƬ�����ص�..
	for ( int xInNewImg = 0; xInNewImg<newWidth; ++xInNewImg )
	{
		for ( int yInNewImg=0; yInNewImg<newHeight; ++yInNewImg )
		{
			// ��ת������ԭ��.
			int transNewX = xInNewImg - transformOriginX;
			int transNewY = yInNewImg - transformOriginY;

			CMatrix<double> originalCoordinateMatrix( 3, 1, 0, 1 );
			originalCoordinateMatrix.Value( 0, 0 ) = transNewX;
			originalCoordinateMatrix.Value( 1, 0 ) = transNewY;
			originalCoordinateMatrix.Value( 2, 0 ) = 1;

			CMatrix<double> transformedCoord = originalCoordinateMatrix * affineMatrix;

			// ��ȷ��ԭʼ�����. ȥ����ת�������ԭ���ƫ��.
			double transX = transformedCoord.Value( 0, 0 ) + transformOriginX;
			double transY = transformedCoord.Value( 1, 0 ) + transformOriginY;

			// ���ò�ͬ�Ĳ�ֵ�㷨.
			int xOriginal = -1;
			int yOriginal = -1;
			if ( CDigitalImage::InterpolateNearestNeighbor == interpolateType )
			{
				// ����ĵ�, ��������.
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
				// ˫���Բ�ֵ. http://en.wikipedia.org/wiki/Bilinear_interpolation
				int xLess = (int)transX;
				int yLess = (int)transY;

				if( xLess > 0 && xLess < GetWidth()-1 && yLess > 0 && yLess < GetHeight()-1 )
				{
					double topValueTmp = 0;
					double bottomValueTmp = 0;
					// ������������� (xLess, yLess) �� (xLess+1, yLess)֮��Ĳ�ֵ.
					topValueTmp = this->Pixel( xLess, yLess ) * ( xLess + 1 - transX ) + this->Pixel( xLess + 1, yLess ) * ( transX - xLess );
					// ������������� (xLess, yLess+1) �� ( xLess+1, yLess+1 )֮��Ĳ�ֵ.
					bottomValueTmp = this->Pixel( xLess, yLess+1 ) * ( xLess + 1 - transX ) + this->Pixel( xLess + 1, yLess + 1 ) * ( transX - xLess );

					// ������������֮���ֵ.
					double finalValue = topValueTmp * ( yLess + 1 - transY ) + bottomValueTmp * ( transY - yLess );

					// ��ֵ.
					newImg[ xInNewImg+yInNewImg* newWidth ] = (int)(finalValue+0.5);
				}


			}


		}
	}

	// ������..
	m_imageDataBuf = newImg;
	this->m_width = newWidth;
	this->m_height = newHeight;
}

double CDigitalImage::GaussianRandom(double u,double g /*,double *r*/ ) 
{
	// �����ҵ�, ������...
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
			//  ����һ����Ӧ���˾�, ���˾���Χ�ڵľ�ֵ.
			int value = 0;
			int red = 0;
			int green = 0;
			int blue = 0;
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

						if ( DIT_Gray == GetImageType() )
						{
							value +=  multiple * this->Pixel( imgX, imgY );
						}
						else if( DIT_RGB == GetImageType() )
						{							
							red += multiple * GetRValue( this->Pixel( imgX, imgY ) );
							green += multiple * GetGValue( this->Pixel( imgX, imgY ) );
							blue += multiple * GetBValue( this->Pixel( imgX, imgY ) );
						}
						
						multipleCount += multiple;
					}
				}
			}

			if ( DIT_Gray == GetImageType() )
			{
				value /= multipleCount;		// �Ҷ�ͼֱ�Ӵ���.
			}
			else if( DIT_RGB == GetImageType() )
			{						
				// ��ɫͼ�ֱ����ÿ��ɫ�ʽ��д���.
				red /= multipleCount;
				green /= multipleCount;
				blue /= multipleCount;
				value = RGB( red, green, blue );
			}
			
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
			//  ����һ����Ӧ���˾�, ���˾���Χ�ڵ���ֵ
			
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
			int value = sortBuf[ sortBuf.size()/2 ];	// �м�ֵ.
			this->Pixel( x, y ) = value;
		}
	}
}

void FFT2( complex<double> * TD, complex<double> * FD, int w, int wp, int h, int hp, int r, bool centerTransform )
{
// 	for( long i = 0; i < h; i++)
// 	{
// 		// ��y������п��ٸ���Ҷ�任
// 		FFT(&TD[w * i], &FD[w * i], wp);
// 	}
// 
// 	// ����任���
// 	for( long i = 0; i < h; i++)
// 	{
// 		for( long j = 0; j < w; j++)
// 		{
// 			TD[i + h * j] = FD[j + w * i];
// 		}
// 	}
// 
// 	for( long i = 0; i < w; i++)
// 	{
// 		// ��x������п��ٸ���Ҷ�任
// 		FFT(&TD[i * h], &FD[i * h], hp);
// 	}
// 
// 	for( long i = 0; i < h; i++)			// ��
// 	{		
// 		for( long j = 0; j < w; j++)		// ��
// 		{
// 			// ����Ƶ��
// 			double dTemp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + 
// 				FD[j * h + i].imag() * FD[j * h + i].imag()) / 100;
// 			if (dTemp > 255)
// 			{
// 				dTemp = 255;
// 			}
// 
// 			// ���ص�ָ�룬�˴���ֱ��ȡi��j����Ϊ�˽��任���ԭ���Ƶ�����
// 			// lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight-1-i) + j;
// 			int x = j;
// 			int y = i;
// 			if( centerTransform )
// 			{
// 				x = (j<w/2 ? j+w/2 : j-w/2);
// 				y = ( height - 1 - (i<h/2 ? i+h/2 : i-h/2));
// 			}
// 			// ����Դͼ��
// 			// this->Pixel( x,y ) = dTemp;
// 		}
// 	}
}

void CDigitalImage::FourierTransform( bool centerTransform )
{
	int width = GetWidth();
	int height = GetHeight();

	// ����ֵ
	long w = 1;	// ���и���Ҷ�任�Ŀ�ȣ�2�������η���	
	int wp = 0;
	

	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while( w * 2 <= width )
	{
		w *= 2;
		wp++;
	}	

	long h = 1;	// ���и���Ҷ�任�ĸ߶ȣ�2�������η���	
	int hp = 0;
	while(h * 2 <= height )
	{
		h *= 2;
		hp++;
	}

	
	TCoplexDoubleList TD( w*h );
	TCoplexDoubleList FD( w*h );

	TCoplexDoubleList TDRed( w*h );
	TCoplexDoubleList TDGreen( w*h );
	TCoplexDoubleList TDBlue( w*h );

	TCoplexDoubleList FDRed( w*h );
	TCoplexDoubleList FDGreen( w*h );
	TCoplexDoubleList FDBlue( w*h );

	for( long i = 0; i < h; i++)			// ��
	{		
		for( long j = 0; j < w; j++)		// ��
		{
			// ��ʱ��ֵ, ����Ҷ�任�����
			if( DIT_Gray == GetImageType() )
			{
				TD[j + w * i] = complex<double>( this->Pixel( j, i ), 0);
			}
			else
			{
				TDRed[j + w * i] = complex<double>( GetRValue( this->Pixel( j, i ) ), 0);
				TDGreen[j + w * i] = complex<double>( GetGValue( this->Pixel( j, i ) ), 0);
				TDBlue[j + w * i] = complex<double>( GetBValue( this->Pixel( j, i ) ), 0);
			}
		}
	}

	for( long i = 0; i < h; i++)
	{
		// ��y������п��ٸ���Ҷ�任
		if( DIT_Gray == GetImageType() )
		{
			FFT(&TD[w * i], &FD[w * i], wp);
		}
		else
		{
			FFT(&TDRed[w * i], &FDRed[w * i], wp);
			FFT(&TDGreen[w * i], &FDGreen[w * i], wp);
			FFT(&TDBlue[w * i], &FDBlue[w * i], wp);
		}
	}

	// ����任���
	for( long i = 0; i < h; i++)
	{
		for( long j = 0; j < w; j++)
		{			
			TD[i + h * j] = FD[j + w * i];
			TDRed[i + h * j] = FDRed[j + w * i];
			TDGreen[i + h * j] = FDGreen[j + w * i];
			TDBlue[i + h * j] = FDBlue[j + w * i];
		}
	}

	for( long i = 0; i < w; i++)
	{
		// ��x������п��ٸ���Ҷ�任		
		if( DIT_Gray == GetImageType() )
		{
			FFT(&TD[i * h], &FD[i * h], hp);
		}
		else
		{
			FFT(&TDRed[i * h], &FDRed[i * h], wp);
			FFT(&TDGreen[i * h], &FDGreen[i * h], wp);
			FFT(&TDBlue[i * h], &FDBlue[i * h], wp);
		}
	}

	// ������Ҷ�任����.
	this->m_fourierData = FD;
	this->m_fourierDataRed = FDRed;
	this->m_fourierDataGreen = FDGreen;
	this->m_fourierDataBlue = FDBlue;


	for( long i = 0; i < h; i++)			// ��
	{		
		for( long j = 0; j < w; j++)		// ��
		{
			// ����Ƶ��
			double dTemp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + 
				FD[j * h + i].imag() * FD[j * h + i].imag()) / 100;
			if (dTemp > 255)
			{
				dTemp = 255;
			}

			double dTempRed = sqrt(FDRed[j * h + i].real() * FDRed[j * h + i].real() + 
				FDRed[j * h + i].imag() * FDRed[j * h + i].imag()) / 100;
			if (dTempRed > 255)
			{
				dTempRed = 255;
			}

			double dTempGreen = sqrt(FDGreen[j * h + i].real() * FDGreen[j * h + i].real() + 
				FDGreen[j * h + i].imag() * FDGreen[j * h + i].imag()) / 100;
			if (dTempGreen > 255)
			{
				dTempGreen = 255;
			}

			double dTempBlue = sqrt(FDBlue[j * h + i].real() * FDBlue[j * h + i].real() + 
				FDBlue[j * h + i].imag() * FDBlue[j * h + i].imag()) / 100;
			if (dTempBlue > 255)
			{
				dTempBlue = 255;
			}
			// ���ص�ָ�룬�˴���ֱ��ȡi��j����Ϊ�˽��任���ԭ���Ƶ�����
			// lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight-1-i) + j;
			int x = j;
			int y = i;
			if( centerTransform )
			{
				x = (j<w/2 ? j+w/2 : j-w/2);
				y = ( height - 1 - (i<h/2 ? i+h/2 : i-h/2));
			}
			// ����Դͼ��
			if( DIT_Gray == GetImageType() )
			{
				this->Pixel( x,y ) = (int)dTemp;
			}
			else
			{
				this->Pixel( x,y ) = RGB( dTempRed, dTempGreen, dTempBlue );
			}
			
		}
	}

}

/*************************************************************************
 * �������ƣ�
 *   FFT()
 * ����:
 *   complex<double> * TD	- ָ��ʱ�������ָ��
 *   complex<double> * FD	- ָ��Ƶ�������ָ��
 *   r						��2������������������
 * ����ֵ:
 *   �ޡ�
 * ˵��:
 *   �ú�������ʵ�ֿ��ٸ���Ҷ�任��
 ************************************************************************/
VOID CDigitalImage::FFT(complex<double> * TD, complex<double> * FD, int r)
{	
	LONG	count;				// ����Ҷ�任����	
	int		i,j,k;				// ѭ������
	int		bfsize,p;	
	double	angle;				// �Ƕ�	
	complex<double> *W,*X1,*X2,*X;
	
	count = 1 << r;				// ���㸶��Ҷ�任����
	
	// ������������洢��
	W  = new complex<double>[count / 2];
	X1 = new complex<double>[count];
	X2 = new complex<double>[count];
	
	// �����Ȩϵ��
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	
	// ��ʱ���д��X1
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// ���õ����㷨���п��ٸ���Ҷ�任
	for(k = 0; k < r; k++)
	{
		for(j = 0; j < 1 << k; j++)
		{
			bfsize = 1 << (r-k);
			for(i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
				X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2]) 
					* W[i * (1<<k)];
			}
		}
		X  = X1;
		X1 = X2;
		X2 = X;
	}
	
	// ��������
	for(j = 0; j < count; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)
		{
			if (j&(1<<i))
			{
				p+=1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
	}
	
	delete W;
	delete X1;
	delete X2;
}

void CDigitalImage::InverseFourierTransform(  )
{
	if ( m_fourierData.empty() )
	{
		assert( false );
		return;
	}

	// �����㷨ֻ��֧��û�б��ü���ͼƬ.
	assert( this->GetWidth() * this->GetHeight() == m_fourierData.size() );

	long h = GetHeight();
	long w = GetWidth();

	int hp = 0;
	
	while( ( h >> hp ) != 1 )
	{
		hp ++;
	}

	int wp = 0;
	while( ( w >> wp ) != 1 )
	{
		wp ++;
	}

	TCoplexDoubleList& TD = m_fourierData;
	TCoplexDoubleList& TDRed = m_fourierDataRed;
	TCoplexDoubleList& TDGreen = m_fourierDataGreen;
	TCoplexDoubleList& TDBlue = m_fourierDataBlue;

	TCoplexDoubleList FD( w*h );
	TCoplexDoubleList FDRed( w*h );
	TCoplexDoubleList FDGreen( w*h );
	TCoplexDoubleList FDBlue( w*h );

	for( long i = 0; i < w; i++)
	{
		// ��x������п��ٸ���Ҷ�任		
		if( DIT_Gray == GetImageType() )
		{
			IFFT(&TD[i * h], &FD[i * h], hp);
		}
		else
		{
			IFFT(&TDRed[i * h], &FDRed[i * h], wp);
			IFFT(&TDGreen[i * h], &FDGreen[i * h], wp);
			IFFT(&TDBlue[i * h], &FDBlue[i * h], wp);
		}
	}
	

	// ����任���
	for( long i = 0; i < h; i++)
	{
		for( long j = 0; j < w; j++)
		{			
			TD[i + h * j] = FD[j + w * i];
			TDRed[i + h * j] = FDRed[j + w * i];
			TDGreen[i + h * j] = FDGreen[j + w * i];
			TDBlue[i + h * j] = FDBlue[j + w * i];
		}
	}

	for( long i = 0; i < h; i++)
	{
		// ��y������п��ٸ���Ҷ�任
		if( DIT_Gray == GetImageType() )
		{
			IFFT(&TD[w * i], &FD[w * i], wp);
		}
		else
		{
			IFFT(&TDRed[w * i], &FDRed[w * i], wp);
			IFFT(&TDGreen[w * i], &FDGreen[w * i], wp);
			IFFT(&TDBlue[w * i], &FDBlue[w * i], wp);
		}
	}

	// �����õ������ݻָ���ͼƬ��.
	for( long i = 0; i < h; i++)			// ��
	{		
		for( long j = 0; j < w; j++)		// ��
		{
			int value = (int)FD[j + w * i].real();
			value = max( 0, value );
			value = min( 255, value );

			int r = (int)FDRed[ j+w*i ].real();
			int g = (int)FDGreen[ j+w*i ].real();
			int b = (int)FDBlue[ j+w*i ].real();
						
			// ��ʱ��ֵ, ����Ҷ�任�����
			if( DIT_Gray == GetImageType() )
			{
				
				this->Pixel( j, i ) = value;				
			}
			else
			{
				this->Pixel( j, i ) = RGB( r,g,b );
			}
		}
	}
}

/*************************************************************************
 * �������ƣ�
 *   IFFT()
 * ����:
 *   complex<double> * FD	- ָ��Ƶ��ֵ��ָ��
 *   complex<double> * TD	- ָ��ʱ��ֵ��ָ��
 *   r						��2������
 * ����ֵ:
 *   �ޡ�
 * ˵��:
 *   �ú�������ʵ�ֿ��ٸ���Ҷ���任��
 ************************************************************************/
VOID CDigitalImage::IFFT(complex<double> * FD, complex<double> * TD, int r)
{	
	LONG	count;					// ����Ҷ�任����	
	int		i;						// ѭ������	
	complex<double> *X;	
	
	count = 1 << r;					// ���㸶��Ҷ�任����
	X = new complex<double>[count];	// ������������洢��
	memcpy(X, FD, sizeof(complex<double>) * count);	// ��Ƶ���д��X
	
	// ����
	for(i = 0; i < count; i++)
	{
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	}
	
	FFT(X, TD, r);					// ���ÿ��ٸ���Ҷ�任
	
	// ��ʱ���Ĺ���
	for(i = 0; i < count; i++)
	{
		TD[i] = complex<double> (TD[i].real() / count, -TD[i].imag() / count);
	}
	
	delete X;
}

void CDigitalImage::DiscreteCosineTransform()
{
	int wdith = GetWidth();
	int height = GetHeight();

	// ����ֵ
	long w = 1;
	long h = 1;
	long wp = 0;
	long hp = 0;

	// ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w * 2 <= wdith)
	{
		w *= 2;
		wp++;
	}

	while(h * 2 <= height)
	{
		h *= 2;
		hp++;
	}

	TDoubleVector f( w*h );
	TDoubleVector F( w*h );

	for( int i = 0; i < h; i++)			// ��
	{		
		for( int j = 0; j < w; j++)		// ��
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			// ��ʱ��ֵ
			f[j + i * w] = this->Pixel( j, i );
		}
	}

	for(int i = 0; i < h; i++)
	{
		// ��y���������ɢ���ұ任
		DCT(&f[w * i], &F[w * i], wp);
	}

	// ���������
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			f[j * h + i] = F[j + w * i];
		}
	}

	for(int j = 0; j < w; j++)
	{
		// ��x���������ɢ���ұ任
		DCT(&f[j * h], &F[j * h], hp);
	}	

	// ����. ���任��ʱ��ñ��ȥ.
	m_DCTData = F;

	for(int i = 0; i < h; i++)			// ��
	{		
		for(int j = 0; j < w; j++)		// ��
		{
			double dTemp = fabs(F[j*h+i]);	// ����Ƶ��

			// �ж��Ƿ񳬹�255
			if (dTemp > 255)
			{
				dTemp = 255;
			}

			// ����Դͼ��
			this->Pixel( j,i ) = (int)dTemp;
		}
	}
}

/*************************************************************************
 * �������ƣ�
 *   DCT()
 * ����:
 *   double * f				- ָ��ʱ��ֵ��ָ��
 *   double * F				- ָ��Ƶ��ֵ��ָ��
 *   r						��2������
 * ����ֵ:
 *   �ޡ�
 * ˵��:
 *   �ú�������ʵ�ֿ�����ɢ���ұ任���ú�������2N��Ŀ��ٸ���Ҷ�任
 * ��ʵ����ɢ���ұ任��
 ************************************************************************/
VOID CDigitalImage::DCT(double *f, double *F, int r)
{	
	LONG	count;			// ��ɢ���ұ任����	
	int		i;				// ѭ������	
	double	dTemp;	
	complex<double> *X;
		
	count = 1<<r;			// ������ɢ���ұ任����	

	X = new complex<double>[count*2];	
	memset(X, 0, sizeof(complex<double>) * count * 2);	// ����ֵΪ0
	
	// ��ʱ���д������X
	for(i=0;i<count;i++)
	{
		X[i] = complex<double> (f[i], 0);
	}
		
	FFT(X,X,r+1);				// ���ÿ��ٸ���Ҷ�任		
	dTemp = (double)1/sqrt((double)count);		// ����ϵ��		
	F[0] = X[0].real() * dTemp;	// ��F[0]	
	dTemp *= sqrt((double)2);
	
	// ��F[u]	
	for(i = 1; i < count; i++)
	{
		F[i]=(X[i].real() * cos(i*PI/(count*2)) + X[i].imag() * 
			sin(i*PI/(count*2))) * dTemp;
	}
	
	delete X;
}

void CDigitalImage::InverseDiscreteCosineTransform( double compressionRate /* = 0 */ )
{
	if ( m_DCTData.empty() )
	{
		assert( false );
		return;
	}

	// �����㷨ֻ��֧��û�б��ü���ͼƬ.
	assert( this->GetWidth() * this->GetHeight() == m_DCTData.size() );

	long h = GetHeight();
	long w = GetWidth();

	int hp = 0;

	while( ( h >> hp ) != 1 )
	{
		hp ++;
	}

	int wp = 0;
	while( ( w >> wp ) != 1 )
	{
		wp ++;
	}

	TDoubleVector f = m_DCTData;
	TDoubleVector F( w*h );

	// ����ѹ����������f�е�����. ��ȡ��ɢ���ұ任�����ϲ��ֵ�������.
	// ������֮��Ĳ��ֶ���0.
	double areaAll = w * h;
	double triangleSideLen = sqrt( areaAll * compressionRate * 2 );
	if ( triangleSideLen <= w )	// ������������һ��, �Ͳ�ѹ����.
	{
		for( int y=0; y<h; ++y )
		{
			for( int x=0; x<h; ++x )
			{
				double curTriangleWidth = triangleSideLen - y;
				double curTriangleHeight = triangleSideLen - x;	// ����ֱ��������.
				if( x > curTriangleWidth || y > curTriangleHeight )
				{
					f[ x + y*w ] = 0;
				}
			}
		}
	}
	




	for(int i = 0; i < h; i++)
	{
		// ��y���������ɢ���ұ任
		IDCT(&f[w * i], &F[w * i], wp);
	}

	// ���������
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			f[j * h + i] = F[j + w * i];
		}
	}

	for(int j = 0; j < w; j++)
	{
		// ��x���������ɢ���ұ任
		IDCT(&f[j * h], &F[j * h], hp);
	}	

	// ���õ�ͼƬ��.
	for( int i = 0; i < h; i++)			// ��
	{		
		for( int j = 0; j < w; j++)		// ��
		{
			int value = (int)F[ j + w*i ];
			value = max( 0, value );
			value = min( 255, value );
			// ��ʱ��ֵ
			this->Pixel( j, i ) = value;
		}
	}
}

/*************************************************************************
 * �������ƣ�
 *   IDCT()
 * ����:
 *   double * F				- ָ��Ƶ��ֵ��ָ��
 *   double * f				- ָ��ʱ��ֵ��ָ��
 *   r						��2������
 * ����ֵ:
 *   �ޡ�
 * ˵��:
 *   �ú�������ʵ�ֿ�����ɢ���ҷ��任���ú���Ҳ����2N��Ŀ��ٸ���Ҷ�任
 * ��ʵ����ɢ���ҷ��任��
 ************************************************************************/
VOID CDigitalImage::IDCT(double *F, double *f, int r)
{
	LONG	count;			// ��ɢ���ҷ��任����
	int		i;				// ѭ������
	double	dTemp, d0;	
	complex<double> *X;
		
	count = 1<<r;			// ������ɢ���ұ任����

	X = new complex<double>[count*2];	
	memset(X, 0, sizeof(complex<double>) * count * 2);	// ����ֵΪ0
	
	// ��Ƶ��任���д������X
	for(i=0;i<count;i++)
	{
		X[i] = complex<double> (F[i] * cos(i*PI/(count*2)), F[i] * 
			sin(i*PI/(count*2)));
	}
	
	IFFT(X,X,r+1);		// ���ÿ��ٸ���Ҷ���任
	
	// ����ϵ��
	dTemp = sqrt(2.0/count);
	d0 = (sqrt(1.0/count) - dTemp) * F[0];
	
	for(i = 0; i < count; i++)
	{
		f[i] = d0 + X[i].real()* dTemp * 2 * count;
	}
	
	delete X;
}

/*
 *  sobel_edge:  Converts the BW image in an Edged image.
 *  Inputs: 
 *      (image *) Org Black and White image struct.
 *		(image *) The Edge Detected image struct.
 *
 *  Returns:
 *      int: 0 is successful run. -1 is a bad malloc.
 *
 *  How this algorithm works:
 *      http://www.pages.drexel.edu/~weg22/edge.html
 */
void  CDigitalImage::SobelEdgeDetect( )
{
	#define NDIMS           2       //X * Y
	typedef unsigned char   uint8;

	typedef struct image{
		uint8	*im;
		int     dims[NDIMS];
	}image;

	image inImg, edgeImage;
	inImg.dims[0] = GetWidth();
	inImg.dims[1] = GetHeight();

	typedef std::vector<uint8> TImgDataBuf;
	TImgDataBuf imgData( GetWidth()*GetHeight() );
	for( int y=0; y<GetHeight(); ++y )
	{
		for( int x=0; x<GetWidth(); ++x )
		{
			imgData[ x + y*GetWidth() ] = (uint8)this->Pixel( x, y );
		}
	}
	inImg.im = imgData.data();

	image *bw_im = &inImg;
	image *ed_im = &edgeImage;

    int     X, Y, I, J, elements, im_offset, mask_offset;
    long sumX = 0;
	long sumY = 0;
	long SUM = 0;
    int     x_mask[]={-1,-2,-1,
                       0, 0, 0,
                       1, 2, 1};
    int     y_mask[]={ 1, 0,-1,
                       2, 0,-2,
                       1, 0,-1};    
    
    elements = bw_im->dims[0] * bw_im->dims[1];
	ed_im->dims[0] = bw_im->dims[0];
	ed_im->dims[1] = bw_im->dims[1];

	TImgDataBuf edgeImgDataBuf( ed_im->dims[0] * ed_im->dims[1], 0 );
    ed_im->im = edgeImgDataBuf.data();
	
     // Convolution starts here
    for(Y=0; Y<ed_im->dims[1]; Y++)  {
        for(X=0; X<ed_im->dims[0]; X++)  {
            // image boundaries 
            if( Y==0 || Y==ed_im->dims[1]-1){ /*rows*/
                SUM = 0;
            } else if( X==0 || X==ed_im->dims[0]-1) { /*cols*/
                SUM = 0;
            // Convolution starts here
            } else {
                sumX = 0; sumY = 0; SUM = 0;
                // X&Y GRADIENT APPROXIMATION
                for(I=-1; I<=1; I++)  {
                    for(J=-1; J<=1; J++)  {
                        im_offset = (Y+J)*ed_im->dims[0] + (X+I);
                        mask_offset = (I+1)+((J+1)*3);
						sumX += (int)
                            (*(bw_im->im+im_offset)) * 
                            x_mask[mask_offset];
                        sumY += (int)
                            (*(bw_im->im+im_offset)) * 
                            y_mask[mask_offset];
                    }
                }
             }
             SUM = labs(sumX) + labs(sumY);
			 if(SUM > 255)	SUM = 255;
			 else if(SUM < 0)	SUM = 0;
            *(ed_im->im+Y*ed_im->dims[0]+X) = 255 - (uint8) SUM;  

			this->Pixel( X, Y ) = 255 - (uint8) SUM;
	     
        }
    }
   
}

void  CDigitalImage::PrewittEdgeDetect( )
{
#define NDIMS           2       //X * Y
	typedef unsigned char   uint8;

	typedef struct image{
		uint8	*im;
		int     dims[NDIMS];
	}image;

	image inImg, edgeImage;
	inImg.dims[0] = GetWidth();
	inImg.dims[1] = GetHeight();

	typedef std::vector<uint8> TImgDataBuf;
	TImgDataBuf imgData( GetWidth()*GetHeight() );
	for( int y=0; y<GetHeight(); ++y )
	{
		for( int x=0; x<GetWidth(); ++x )
		{
			imgData[ x + y*GetWidth() ] = (uint8)this->Pixel( x, y );
		}
	}
	inImg.im = imgData.data();

	image *bw_im = &inImg;
	image *ed_im = &edgeImage;

	int     X, Y, I, J, elements, im_offset, mask_offset;
	long sumX = 0;
	long sumY = 0;
	long SUM = 0;
	int     x_mask[]
		={	-1,-1,-1,
			0, 0, 0,
			1, 1, 1};
	int     y_mask[]={ 
		1, 0,-1,
		1, 0,-1,
		1, 0,-1};    

	elements = bw_im->dims[0] * bw_im->dims[1];
	ed_im->dims[0] = bw_im->dims[0];
	ed_im->dims[1] = bw_im->dims[1];

	TImgDataBuf edgeImgDataBuf( ed_im->dims[0] * ed_im->dims[1], 0 );
	ed_im->im = edgeImgDataBuf.data();

	// Convolution starts here
	for(Y=0; Y<ed_im->dims[1]; Y++)  {
		for(X=0; X<ed_im->dims[0]; X++)  {
			// image boundaries 
			if( Y==0 || Y==ed_im->dims[1]-1){ /*rows*/
				SUM = 0;
			} else if( X==0 || X==ed_im->dims[0]-1) { /*cols*/
				SUM = 0;
				// Convolution starts here
			} else {
				sumX = 0; sumY = 0; SUM = 0;
				// X&Y GRADIENT APPROXIMATION
				for(I=-1; I<=1; I++)  {
					for(J=-1; J<=1; J++)  {
						im_offset = (Y+J)*ed_im->dims[0] + (X+I);
						mask_offset = (I+1)+((J+1)*3);
						sumX += (int)
							(*(bw_im->im+im_offset)) * 
							x_mask[mask_offset];
						sumY += (int)
							(*(bw_im->im+im_offset)) * 
							y_mask[mask_offset];
					}
				}
			}
			SUM = labs(sumX) + labs(sumY);
			if(SUM > 255)	SUM = 255;
			else if(SUM < 0)	SUM = 0;
			*(ed_im->im+Y*ed_im->dims[0]+X) = 255 - (uint8) SUM;  

			this->Pixel( X, Y ) = 255 - (uint8) SUM;

		}
	}

}

void CDigitalImage::RobertsEdgeDetect()
{
	int width = GetWidth();
	int height = GetHeight();

	CMatrix<unsigned char> xc( width, height, 0, 1 );
	for( int i=0; i<height; ++i )
	{
		for( int j=0; j<width; ++j )
		{
			xc.Value( j, i ) = this->Pixel( j, i );
		}
	}

	int nrows = height;
	int ncols = width;
	int thresh = 0;
	int thresh2 = 0;

	CMatrix<float> row( ncols, nrows, 0, 1 );
	CMatrix<float> col( ncols, nrows, 0, 1 );
	CMatrix<float> theta( ncols+2, nrows+2, 0, 1 );
	CMatrix<float> y( ncols+2, nrows+2, 0, 1 );
	CMatrix<unsigned char> edgemap( width, height, 0, 1 );



// 	if( row == NULL || col == NULL || theta == NULL || y == NULL || edgemap == NULL)
// 		error(" Allocation error of matrices in Robet's sub-function\n");
	for( int i = 0; i < nrows; i++)
	{
		for( int j = 0; j < ncols; j++)
		{
			double xcij = ( i<1 || j<1 ) ? 0 : xc[i-1][j-1];
			double xci = ( i < 1 ) ? 0 : xc[i-1][j];
			double xcj = (j<1) ? 0 : xc[i][j-1];
			row[i][j]= (float)(xc[i][j] - xcij )/sqrt( (double)2 );
			col[i][j]= (float)( xci - xcj )/sqrt( (double)2 );


			y[i][j] = row[i][j]*row[i][j]+ col[i][j]*col[i][j];
			theta[i][j]= atan2(col[i][j],row[i][j]);
		}
	}
	
	/* DO THE THRESHOLDING TO COMPUTE THE EDGE PIXELS */
	for( int i=0;i<nrows;i++)
	{
		for( int j=0;j<ncols;j++)
		{
			if(y[i][j] >= 255 )
			{
				edgemap[i][j] = 255;
//				edgepoints++;
			}
			else if( edgemap[i][j] < 0 )
			{
				edgemap[i][j] =0;
			}

			// ��ת
			this->Pixel( j, i ) = 255 - edgemap[i][j];
		}
	}
}


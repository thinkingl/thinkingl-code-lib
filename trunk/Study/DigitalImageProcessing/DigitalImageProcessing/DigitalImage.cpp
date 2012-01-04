#include "DigitalImage.h"

#include "common.h"

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

				value = *(int*)( (char*)lineBeginPos + bytePP * w );
				int sdfwdf =  ( ( 1 << bpp ) - 1 );
				value &= ( ( (int)1 << bpp ) - 1 );	// ��ȡ��bppλ.

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

				grayHistoram[ (double)(r+g+b)/3 + 0.5 ] ++;
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
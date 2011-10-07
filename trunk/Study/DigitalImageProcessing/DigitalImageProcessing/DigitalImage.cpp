#include "DigitalImage.h"


CDigitalImage::CDigitalImage(void)
{
	this->m_bitsPerPixel = 0;
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

		for ( int h=0; h<height; ++h )
		{
			for ( int w=0; w<width; ++w )
			{
				int value = m_image.GetPixel( w, h );
				this->m_imageDataBuf[ h*width + w ] = value;
			}
		}

		// ����û�����ֽ���.
		this->m_bitsPerPixel = bpp;
		// ����ͼƬ����. 
		// !��ʱ��Ϊ�����ͼƬ����RGBģʽ��ͼƬ.
		this->m_imageType = DIT_RGB;
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

	// CImage ����֧��RGB��λͼ.
	const int DrawerIntensiveLevel = 8;
	const int DrawerBPP = DrawerIntensiveLevel * 3;
	BOOL bOk = pImg->Create( width, height, DrawerBPP );
	ASSERT( bOk );

	for ( int h=0; h<height; ++h )
	{
		for ( int w=0; w<width; ++w )
		{
			int value = this->m_imageDataBuf[ h * width + w];
			int pixel = 0;
			if ( DIT_RGB == this->m_imageType )
			{
				if ( this->m_bitsPerPixel != DrawerBPP )
				{
					ASSERT( FALSE );	// ��ʱ����֧�ַ�24 bpp��RGB.
				}
				pixel = value;
			}
			else if( DIT_Grayscale == this->m_imageType )
			{
				// int pixelIntensive = ( value << ( DrawerIntensiveLevel - m_bitsPerPixel ) );
				// ��ǰ�ĻҶȷֱ��ʿ϶�С�ڵ���8.
				int pixelIntensive = this->IntensityTrans( value, m_bitsPerPixel, DrawerIntensiveLevel );
				

				pixel = RGB( pixelIntensive, pixelIntensive, pixelIntensive );
			}
			pImg->SetPixel( w, h, pixel );
		}
	}
	return pImg;
}

void CDigitalImage::Release()
{
	m_imageDataBuf.clear();
	m_bitsPerPixel = 0;
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
	return m_bitsPerPixel;
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
// 
// 	pImg->m_imagePath = this->m_imagePath;
// //	pImg->m_image.Load( pImg->m_imagePath.c_str() );
// 	pImg->m_bitsPerPixel = this->m_bitsPerPixel;
// 	pImg->m_imageDataBuf = this->m_imageDataBuf;
// 
// 	pImg->m_imageType = this->m_imageType;
// 	pImg->m_width = this->m_width;
// 	pImg->m_height = this->m_height;
	return pImg;
}

bool CDigitalImage::ToGrayscaleImage( int intensityLevels )
{
	if ( intensityLevels < 1 || intensityLevels > 8 )
	{
		TRACE( _T("ToGrayscale intensity level must between 1-8") );
		return false;
	}

	for( int h=0; h<this->GetHeight(); ++h )
	{
		for( int w=0; w<this->GetWidth(); ++w )
		{
			int curValue = m_imageDataBuf[ h*this->GetWidth() + w ];
			int grayValue = 0;
			int grayLevel = 0;
			if ( this->m_imageType == DIT_RGB )
			{
				// RGB �ҶȰ��� HSI�ķ�ʽת�� ���� P. 433
				grayValue = ( GetRValue( curValue ) + GetGValue( curValue ) + GetBValue( curValue ) )/3;
				// RGBģʽʱȡ����ɫ�Ļҽ�.
				grayLevel = m_bitsPerPixel/3;
				// �����32, ����һ��ͨ����͸��ͨ��.				
				grayLevel = min( grayLevel, 8 );				
			}
			else if( this->m_imageType == DIT_Grayscale )
			{
				grayValue = curValue;
				grayLevel = m_bitsPerPixel;
			}

			ASSERT( grayLevel <= 8 && grayLevel >= 1 );
			// ��ǰ�Ҷ�ת��ΪĿ��Ҷȷֱ���.
			grayValue = this->IntensityTrans( grayValue, grayLevel, intensityLevels );			
			m_imageDataBuf[ h*this->GetWidth() + w ] = grayValue;
		}
	}
	this->m_imageType = DIT_Grayscale;
	this->m_bitsPerPixel = intensityLevels;
	return true;
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
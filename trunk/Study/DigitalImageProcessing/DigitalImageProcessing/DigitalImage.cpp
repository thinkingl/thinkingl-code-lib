#include "DigitalImage.h"


CDigitalImage::CDigitalImage(void)
{
	this->m_bitsPerPixel = 0;
}


CDigitalImage::~CDigitalImage(void)
{
	this->Release();
}

bool CDigitalImage::Load( ctstring filePath )
{
	this->Release();

	HRESULT hr = m_image.Load( filePath.c_str() );
	bool bOk = ( S_OK == hr );
	if ( bOk )
	{
		this->m_imagePath = filePath;

		void *pBuf = m_image.GetBits();
		int bpp = m_image.GetBPP();
		int pitch = m_image.GetPitch();
		int width = this->GetWidth();
		int height = this->GetHeight();
		
		this->m_imageDataBuf.resize( width*height, 0 );

		for ( int h=0; h<height; ++h )
		{
			for ( int w=0; w<width; ++w )
			{
				int value = m_image.GetPixel( w, h );
				this->m_imageDataBuf[ h*width + w ] = value;
			}
		}
		this->m_bitsPerPixel = bpp;
		
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
	BOOL bOk = pImg->Create( width, height, bpp );
	ASSERT( bOk );

	for ( int h=0; h<height; ++h )
	{
		for ( int w=0; w<width; ++w )
		{
			int value = this->m_imageDataBuf[ h * width + w];
			pImg->SetPixel( w, h, value );
		}
	}
	return pImg;
}

void CDigitalImage::Release()
{
	m_imageDataBuf.clear();
	m_bitsPerPixel = 0;
	m_image.Destroy();
	m_imagePath.clear();
}

int CDigitalImage::GetWidth() const
{
	return m_image.GetWidth();
}

int CDigitalImage::GetHeight() const
{
	return m_image.GetHeight();
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
	// 新加的成员变量必须也要加在这里!!!
	CDigitalImage *pImg = new CDigitalImage();
	pImg->m_imagePath = this->m_imagePath;
	pImg->m_image.Load( pImg->m_imagePath.c_str() );
	pImg->m_bitsPerPixel = this->m_bitsPerPixel;
	pImg->m_imageDataBuf = this->m_imageDataBuf;
	return pImg;
}
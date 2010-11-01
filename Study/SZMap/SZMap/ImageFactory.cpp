#include "ImageFactory.h"


CImageFactory::CImageFactory(void)
{
	// 开始线程。
	this->m_hImageLoadThread = ::CreateThread( NULL, NULL, ImageLoadThread, this, NULL, NULL );

}


CImageFactory::~CImageFactory(void)
{
	// 结束线程。
	if ( m_hImageLoadThread )
	{
		::TerminateThread( this->m_hImageLoadThread, 0 );
	}
}

DWORD CImageFactory::ImageLoadThread( void * param )
{
	CImageFactory *pThis = ( CImageFactory* )param;
	if ( pThis )
	{
		while ( 1 )
		{
			while ( !pThis->m_imageMissionsStack.empty() )
			{
			}
			Sleep( 500 );
		}
	}
}

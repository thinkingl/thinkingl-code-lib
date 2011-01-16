#include "stdafx.h"
#include "kedagiftcommon.h"
#include <map>

CImage *GetImage( LPCTSTR strImg )
{
	typedef std::map< CString, CImage* > TImageTable;
	static TImageTable s_imageTable;

	CImage *pImg = s_imageTable[ strImg ];
	if ( 0 == pImg )
	{
		pImg = new CImage();
		pImg->Load( strImg );
		s_imageTable[ strImg ] = pImg;
	}
	return pImg;
}
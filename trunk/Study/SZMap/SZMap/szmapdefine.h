#pragma once

/** һЩ��������. */

enum EMapType
{
	MAP_NORMAL,		// ��ͨ��ͼ.
	MAP_SATELLITE,	// ���ǵ�ͼ.
};

#define DEF_STR static const TCHAR* 

enum EMapServer
{
	GoogleMaps,

};

namespace ServerUrl
{
	DEF_STR GoogleMaps = _T( "http://maps.google.com" );
}
// end of file



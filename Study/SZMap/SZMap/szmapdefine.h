#pragma once

/** 一些常量定义. */

enum EMapType
{
	MAP_NORMAL,		// 普通地图.
	MAP_SATELLITE,	// 卫星地图.
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



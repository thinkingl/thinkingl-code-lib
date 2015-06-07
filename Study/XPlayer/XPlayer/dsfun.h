#pragma once

/**
*	对一些DirectShow功能的函数封装.
*	+by lizhixing@2015-06-07
*/

#include <dshow.h>

// 支持GraphEdit调试程序.
HRESULT AddToRot( IUnknown * pUnknownGraph, DWORD* pdwRegister );

void RemoveFromRot( DWORD pdwRegister);
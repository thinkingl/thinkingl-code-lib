#pragma once

#include "portabledefine.h"

#ifdef  WIN32

class CDigitalImageProcessingView;
class CPropertiesWnd;
/** 获取当前的活动的图像视图. */
CDigitalImageProcessingView *GetActiveDIPView();
/** 获取属性窗口. */
CPropertiesWnd *GetPropertyWnd();
#endif

#ifndef RGB
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

#ifndef GetRValue
#define GetRValue(rgb)      (LOBYTE(rgb))
#endif 

#ifndef GetGValue
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#endif

#ifndef GetBValue
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))
#endif


// end of the file

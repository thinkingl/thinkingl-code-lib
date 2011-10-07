#pragma once

#ifdef  WIN32

class CDigitalImageProcessingView;
/** ��ȡ��ǰ�Ļ��ͼ����ͼ. */
CDigitalImageProcessingView *GetActiveDIPView();

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

#ifndef _CODING_TRANSFORM
#define _CODING_TRANSFORM
#include "kdvtype.h"
#include "stdio.h"

#ifdef WIN32
#include "windows.h"
#include <WINNT.H>
#include <WINNLS.H>
#elif _LINUX_
#include "iconv.h"
#include <string.h>
#endif

#define CP_GB2312 20936
class CodingTransformer
{
public: 
 //GB2312 תΪ UTF-8 
 static s32  UTF_8ToGB2312(s8* pOut, s32 iBufSize, s8 *pText, s32 nLen);
 //GB2312 תΪ UTF-8 
 static s32  GB2312ToUTF_8(s8* pOut, s32 iBufSize,s8 *pText, s32 nLen);
};
#endif//end _CODING_TRANSFORM
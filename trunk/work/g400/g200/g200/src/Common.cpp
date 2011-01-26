#include "Common.h"

extern u8 g_abyLogModuleLev[LOG_MOD_NUM];

void G200Log(u8 byLogModule, u8 byLogLevel, const char* szFormat, ...)
{
    char szMsg[256] = {0};
	
    if(g_abyLogModuleLev[byLogModule] < byLogLevel)
        return;
	
    va_list pvlist;
    va_start(pvlist, szFormat);
    s32 actLen = vsprintf((char *)szMsg, szFormat, pvlist);
	
	if( actLen <= (s32) 0 || actLen >= (s32) sizeof(szMsg) )
	{
		OspPrintf( TRUE, FALSE, "[error]: CmuLog vsprintf() failed.\n");
        va_end( pvlist );
		return;
	}
    va_end(pvlist);

    ::OspPrintf( TRUE, FALSE, szMsg);
}

BOOL32 IPtoString( u32 dwIP, char achIPStr[], u32 dwLen)
{
    struct   in_addr in;
    in.s_addr   = dwIP;
    
    if(dwLen <= 16)
    {
        return FALSE;
    }
	
    strcpy(achIPStr, inet_ntoa( in));
	
    return TRUE;
}

BOOL32 UnifyDir( s8 *pszNewDir, u32 dwDirLen)
{
/*
以后可能添加对字符结束符的判断（'\'，'/'）
    */
    s8 ch = '/';
    // 以 '/' 作为结束符
    if( pszNewDir[dwDirLen - 1] != ch)
    {
        if ( MAXLEN_FILEPATH < dwDirLen + 2 )
        {
            return FALSE;
        }
		
        pszNewDir[dwDirLen] = ch;
        pszNewDir[dwDirLen + 1] = '\0';
    }
	
    return TRUE;
}
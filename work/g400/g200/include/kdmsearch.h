#ifndef _KDMSEARCH_H_
#define _KDMSEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "kdvtype.h"

typedef struct
{
    u32 m_dwTargetIP;
    u32 m_dwNewIP;
    u32 m_dwNewMask;
    u32 m_dwNewGW;
} TSetNewIpInfo;

// 回调函数结构
typedef void (*FEvSetDevIp)(void* pUserHandle, TSetNewIpInfo tNewIpInfo);

/************************************************************************/
/* 函数功能：启动搜索模块                                               */
/* 参数说明：pfunSetIp —— 修改IP地址回调指针                          */
/* 函数返回值：0 —— 成功；非0 —— 失败                               */
/* 函数说明：搜索模块启动一个搜索线程                                   */
/************************************************************************/
int KdmSearchStartup(void* pUserHandle, FEvSetDevIp pfunSetIp, s8* pchConfigPath);

/************************************************************************/
/* 函数功能：停止搜索模块                                               */
/* 参数说明：无                                                         */
/* 函数返回值：无                                                       */
/* 函数说明：                                                           */
/************************************************************************/
void KdmSearchCleanup(void);

#ifdef __cplusplus
}
#endif

#endif // ifndef _KDMSEARCH_H_

/**************************** FILE END *******************************/


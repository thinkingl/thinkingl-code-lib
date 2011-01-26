#ifndef _REG_SRV_H
#define _REG_SRV_H

#include "osp.h"
#include "g200comm.h"
#include "kdmver.h"

#define G200MS_VERSION KDM_VER(G200ms, 0101)
#define G200MS_CONFVERSION  "KDM2800_G200MS V1R1B1"
/************************************************************************/
/* G200日志打印分类定义                                                 */
/************************************************************************/
#define LOG_MOD_NUM      5

#define LOG_DEVMGR_MOD     0
#define LOG_MAIN_MOD       1
#define LOG_CLIENTCONN_MOD 2
#define LOG_SDK3ASSN_MOD   3


#define AID_G200MS_CLIENTCONN      301
#define AID_G200MS_SDK3A           302

const u32 G200MS_INIT_3ASDK_INTV = 10 * 1000;
const u32 G200MS_LOAD_CFG_INTV = 10 * 60 * 1000;

#define EV_G200MS_EVENT_BEGIN      EV_G200MS_MSG_BGN

enum TInterMsg
{
	EV_G200MS_DEVREG_POWERON = EV_G200MS_EVENT_BEGIN,

	EV_G200MS_INIT_3ASDK_TIMER,

    EV_G200MS_DISCONN_WITH_MPLAT_NTF,

	EV_G200MS_LOAD_CFG_TIMER,

	EV_G200MS_PRINT_NEED_REG_DEV_REQ,
	
	EV_G200MS_PRINT_G200MS_STATUS_REQ

};


//G200MS打印函数
void G200MSLog(u8 byLogModule, u8 byLogLevel, const char* szFormat, ...);

//IP地址转换
BOOL32 IPtoString( u32 dwIP, char achIPStr[], u32 dwLen);

//路径规整函数
BOOL32 UnifyDir( s8 *pszNewDir, u32 dwDirLen);

#endif
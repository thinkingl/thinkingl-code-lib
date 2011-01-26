#ifndef _COMMON_090718_H_
#define _COMMON_090718_H_

#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include "osp.h"
#include "socketbase.h"
#include "kdvsys.h"
#include <time.h>

#ifdef WIN32
#include "io.h"
#endif


#define GETSTRIP(p) ((u8*)&(p))[0],((u8*)&(p))[1],((u8*)&(p))[2],((u8*)&(p))[3]//

//content length 0
#define EV_HS_SERVER_REG             5000
//content length 0
#define EV_SERVER_HS_REG_ACK				(EV_HS_SERVER_REG+1)//
//content length 0
#define EV_SERVER_HS_REG_NACK				(EV_HS_SERVER_REG+2)//
#define EV_HS_SERVER_UNREG					(EV_HS_SERVER_REG+3)//
#define EV_SERVER_HS_UNREG_ACK				(EV_HS_SERVER_REG+4)//
#define EV_SERVER_HS_UNREG_NACK				(EV_HS_SERVER_REG+5)//

#define EV_INTER_TIMER						(EV_HS_SERVER_REG+9)//
#define EV_HS_SERVER_KEDACOMXMLDATA			(EV_HS_SERVER_REG+10)//	
#define EV_HS_SERVER_KEDACOMFILE			(EV_HS_SERVER_REG+11)	
		
#define LEVEL_INFO 255//
#define LEVEL_ERR  0//
#define LEVEL_IMP  1//

#define HIINST  u32//
void DebugOut(u32 dwPrint, s8 * szFormat,...);
typedef void (* InterConnCallBack)(HIINST hInst, u8 *pBuf, s32 nLen);

#endif










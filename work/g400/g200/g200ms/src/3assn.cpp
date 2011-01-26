/*****************************************************************************
工程名      : G200MS
模块名      : 3assn
文件名      : 3assn.cpp
相关文件    :
文件实现功能:
作者        : lqp
版本        : V1.0  Copyright(C) 2010 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期        版本        修改人      修改内容
2010/05/21    1.0         lqp       创建
******************************************************************************/

#include "3assn.h"
#include "srvmain.h"
#include "devreg.h"
extern CDevMgrApp g_cDevMgrApp;
extern TG200CfgBase g_tCfg;

void VS3ASSdkDisconnMPlatCallBack()
{
    //与上级平台断链
    OspPost(MAKEIID(AID_G200MS_SDK3A,CInstance::DAEMON),EV_G200MS_DISCONN_WITH_MPLAT_NTF);
    g_cDevMgrApp.SetInit3ASdk(FALSE);
}

void CSdk3AIns::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{
	G200MSLog(LOG_SDK3ASSN_MOD,LOGLVL_DEBUG2,"[G200ms/SDK3ASSN-Daemon]:Receive Message[%s-%d]\n",
                    OspEventDesc(pcMsg->event), pcMsg->event);
	switch (pcMsg->event)
	{
	case EV_G200MS_INIT_3ASDK_TIMER:
		{
			Init3ASSdk();
		}
		break;
	case EV_G200MS_DISCONN_WITH_MPLAT_NTF:
		{
			//重新初始化3A
            SetTimer(EV_G200MS_INIT_3ASDK_TIMER, G200MS_INIT_3ASDK_INTV);
		}
		break;
	}
}

void CSdk3AIns::Init3ASSdk()
{
	u32 dwSrvIp = g_tCfg.Get3AIP();
	TKDMNO tKdmNo;
	tKdmNo.SetKdmNO(g_tCfg.GetG200Id());
	s8 achUser3a[MAXLEN_ALIAS + 1] = {0};
	s8 achPsw3a[MAXLEN_ALIAS + 1] = {0};
	s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_NAME);
	GetRegKeyString(achFileName,SECTION_MPLAT,KEY_3AUSER,"admin@fj",achUser3a,MAXLEN_ALIAS);
	GetRegKeyString(achFileName,SECTION_MPLAT,KEY_3APSW,"admin",achPsw3a,MAXLEN_ALIAS);
	VS_AAAINITEX tVs_AAAInitEx = {0};
	strncpy(tVs_AAAInitEx.abyInitName,achUser3a,VS_AAA_MAXLEN_WHOLE_TUSER_NAME);
	strncpy(tVs_AAAInitEx.abyInitPwd,achPsw3a,VS_AAA_MAXLEN_PWD);
	tVs_AAAInitEx.wPort = g_tCfg.Get3APort();
	G200MSLog(LOG_SDK3ASSN_MOD,LOGLVL_DEBUG2,"[G200MS/SDK3ASSN][Init3ASSdk]:用户名-%s,密码-%s,端口-%d",
		tVs_AAAInitEx.abyInitName,tVs_AAAInitEx.abyInitPwd,tVs_AAAInitEx.wPort);
    //退出3asdk库
	if (g_cDevMgrApp.IsHasInit3ALib())
	{
        u32 dwRet = VS_3ASSdkQuit();
        if (VS_AAAOK != dwRet)
        {
            G200MSLog(LOG_SDK3ASSN_MOD,LOGLVL_DEBUG1,"[G200MS/SDK3ASSN]:退出3asdk库失败,错误码[%d]!\n",dwRet);
            SetTimer(EV_G200MS_INIT_3ASDK_TIMER, G200MS_INIT_3ASDK_INTV);
            return;
        }
        G200MSLog(LOG_SDK3ASSN_MOD,LOGLVL_DEBUG1,"[G200MS/SDK3ASSN]:退出3asdk库成功!!\n");
        g_cDevMgrApp.SetInit3ALib(FALSE);
	}
	
	unsigned short nRet = VS_3ASSdkInit(dwSrvIp, (char *)tKdmNo.GetNOString(), 10000, &tVs_AAAInitEx);
	if (VS_AAAOK != nRet)
	{
		char achIp[18] = {0};
		IPtoString(dwSrvIp, achIp, 18);
		G200MSLog(LOG_SDK3ASSN_MOD, LOGLVL_DEBUG1, "[G200MS/SDK3ASSN] : 初始化3ASdk[IP:%s]失败, 错误码[%d]！\n", achIp, nRet);
		
		SetTimer(EV_G200MS_INIT_3ASDK_TIMER, G200MS_INIT_3ASDK_INTV);
		g_cDevMgrApp.SetInit3ASdk(FALSE);
		
		return;
	}
	G200MSLog(LOG_SDK3ASSN_MOD, LOGLVL_DEBUG1, "[G200MS/SDK3ASSN] : 初始化3ASdk成功！\n");
	g_cDevMgrApp.SetInit3ALib(true);
	
    VS_3ASCALLBACK tCallBack = {0};
    tCallBack.fp3ASSdkDisconnMPlatCB = VS3ASSdkDisconnMPlatCallBack;
    VS_3ASSdkSetCallBack(&tCallBack);
	
	g_cDevMgrApp.SetInit3ASdk(TRUE);
	
    if (g_cDevMgrApp.IsFirstConn3A())
    {
        post(MAKEIID(AID_G200MS_DEVMGR,CInstance::DAEMON),EV_G200MS_LOAD_CFG_TIMER);
        g_cDevMgrApp.SetFirstConn3AFlag(FALSE);
    }
}
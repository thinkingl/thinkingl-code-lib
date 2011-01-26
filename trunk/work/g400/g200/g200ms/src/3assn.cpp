/*****************************************************************************
������      : G200MS
ģ����      : 3assn
�ļ���      : 3assn.cpp
����ļ�    :
�ļ�ʵ�ֹ���:
����        : lqp
�汾        : V1.0  Copyright(C) 2010 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��        �汾        �޸���      �޸�����
2010/05/21    1.0         lqp       ����
******************************************************************************/

#include "3assn.h"
#include "srvmain.h"
#include "devreg.h"
extern CDevMgrApp g_cDevMgrApp;
extern TG200CfgBase g_tCfg;

void VS3ASSdkDisconnMPlatCallBack()
{
    //���ϼ�ƽ̨����
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
			//���³�ʼ��3A
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
	G200MSLog(LOG_SDK3ASSN_MOD,LOGLVL_DEBUG2,"[G200MS/SDK3ASSN][Init3ASSdk]:�û���-%s,����-%s,�˿�-%d",
		tVs_AAAInitEx.abyInitName,tVs_AAAInitEx.abyInitPwd,tVs_AAAInitEx.wPort);
    //�˳�3asdk��
	if (g_cDevMgrApp.IsHasInit3ALib())
	{
        u32 dwRet = VS_3ASSdkQuit();
        if (VS_AAAOK != dwRet)
        {
            G200MSLog(LOG_SDK3ASSN_MOD,LOGLVL_DEBUG1,"[G200MS/SDK3ASSN]:�˳�3asdk��ʧ��,������[%d]!\n",dwRet);
            SetTimer(EV_G200MS_INIT_3ASDK_TIMER, G200MS_INIT_3ASDK_INTV);
            return;
        }
        G200MSLog(LOG_SDK3ASSN_MOD,LOGLVL_DEBUG1,"[G200MS/SDK3ASSN]:�˳�3asdk��ɹ�!!\n");
        g_cDevMgrApp.SetInit3ALib(FALSE);
	}
	
	unsigned short nRet = VS_3ASSdkInit(dwSrvIp, (char *)tKdmNo.GetNOString(), 10000, &tVs_AAAInitEx);
	if (VS_AAAOK != nRet)
	{
		char achIp[18] = {0};
		IPtoString(dwSrvIp, achIp, 18);
		G200MSLog(LOG_SDK3ASSN_MOD, LOGLVL_DEBUG1, "[G200MS/SDK3ASSN] : ��ʼ��3ASdk[IP:%s]ʧ��, ������[%d]��\n", achIp, nRet);
		
		SetTimer(EV_G200MS_INIT_3ASDK_TIMER, G200MS_INIT_3ASDK_INTV);
		g_cDevMgrApp.SetInit3ASdk(FALSE);
		
		return;
	}
	G200MSLog(LOG_SDK3ASSN_MOD, LOGLVL_DEBUG1, "[G200MS/SDK3ASSN] : ��ʼ��3ASdk�ɹ���\n");
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
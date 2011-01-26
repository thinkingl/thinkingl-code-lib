#include "VcSsn.h"
#include "SdkSsn.h"
#include "PuiSsn.h"
#include "G200Data.h"
#include "CfgMgr.h"
#include "cmsevent.h"
#include "cmusdk.h"
#define RegisteEvent(x) OspAddEventDesc(#x,x)

CVcData     g_cVcData;

CVcSchedApp g_cVcSchedApp;
CPuiSsnApp  g_cPuiSsnApp;
CSdkSsnApp  g_cSdkSsnApp;
CCfgMgrApp  g_cCfgMgrApp;

extern BOOL32 PuiStart(LPCSTR lpszCfgFile, BOOL32 bIsIntegrated);
extern void PuiQuit();

u8 g_abyLogModuleLev[LOG_MOD_NUM];

API void lsdk(u8 byLogLev)
{
	g_abyLogModuleLev[LOG_SDK_MOD] = byLogLev;
}

API void lsched(u8 byLogLev)
{
	g_abyLogModuleLev[LOG_SCHED_MOD] = byLogLev;
}

API void lpui(u8 byLogLev)
{
	g_abyLogModuleLev[LOG_PUI_MOD] = byLogLev;
}

API void lcfg(u8 byLogLev)
{
	g_abyLogModuleLev[LOG_CFG_MOD] = byLogLev;
}

API void logallon()
{
	memset(g_abyLogModuleLev, 255, sizeof(g_abyLogModuleLev));
}

API void logalloff()
{
	memset(g_abyLogModuleLev, 0, sizeof(g_abyLogModuleLev));
}

API void pmatrix(u32 dwMatrixID = 0)
{
	::OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_PRINT_MATRIX_INFO_REQ, &dwMatrixID, sizeof(dwMatrixID));
}

API void poutrt(s32 nMatrixNo = 0,s32 nEncoderIndex = 0)
{
	CMonMsg cMsg;
	cMsg.SetMsgBody(&nMatrixNo,sizeof(s32));
	cMsg.CatMsgBody(&nEncoderIndex,sizeof(s32));
	::OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_PRINT_VIDEOOUT_RT_REQ, cMsg.GetMonMsg(),cMsg.GetMonMsgLen());
}

API void psi()
{
	::OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_PRINT_SWITCHINFO_REQ, NULL, 0);
}

API void g200st()
{
	::OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_PRINT_G200STATUS_REQ, NULL, 0);
}

API void g200help()
{
	//OspPrintf(TRUE, FALSE, "G200 Version: KDM2800 %s\tCompile Time: %s:%s\n", G200_VERSION, __DATE__, __TIME__);
	OspPrintf(TRUE, FALSE, "G200 Version: %s\tCompile Time: %s:%s\n", G200_CONFVERSION, __DATE__, __TIME__);
	OspPrintf(TRUE, FALSE, "pmatrix<matrixid> -- 打印矩阵信息\n");
	OspPrintf(TRUE, FALSE, "\tmatrixid : 0 -- 打印所有矩阵信息\n\t\tother -- 打印指定矩阵编号的矩阵信息\n");
	OspPrintf(TRUE, FALSE, "poutrt<matrixno,encoderIndex> -- 打印指定编码器的邦定关系\n");
    OspPrintf(TRUE, FALSE, "pcamstatus<puid>--显示摄像头状态\n");
	OspPrintf(TRUE,FALSE,"plns--显示license信息\n");
	OspPrintf(TRUE, FALSE, "psi -- 打印矩阵输入输出通道的对应关系\n");
	OspPrintf(TRUE, FALSE, "lsdk<loglev> -- 打开SDK模块的相应级别的打印\n");
	OspPrintf(TRUE, FALSE, "lpui<loglev> -- 打开PUI模块的相应级别的打印\n");
	OspPrintf(TRUE, FALSE, "lsched<loglev> -- 打开Sched模块的相应级别的打印\n");
	OspPrintf(TRUE, FALSE, "lcfg<loglev> -- 打开CfgMgr模块的相应级别的打印\n");
	OspPrintf(TRUE,FALSE,  "g200st -- 打印g200的当前状态\n");
	OspPrintf(TRUE, FALSE, "logallon -- 打开所有打印\n");
	OspPrintf(TRUE, FALSE, "logalloff -- 关闭所有打印\n");
}
/*
功能:显示上报的摄像头状态
*/
API void pcamstatus(LPCSTR lpszId = "")
{
    if(lpszId == NULL)
    {
        OspPrintf(TRUE, FALSE, "输入错误\n");
        return;
    }
    if(strlen(lpszId) != LEN_KDM_NO)
    {
        OspPrintf(TRUE, FALSE, "输入错误\n");
        return;
    }
    ::OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON),EV_G200_PRINT_CAMSTATUS_REQ,lpszId,strlen(lpszId));
}

//显示license信息
API void plns()
{
	::OspPost(MAKEIID(AID_G200_VCSCHED,CInstance::DAEMON),EV_G200_PRINT_LICENSEINFO_REQ);
}
#ifdef _LINUX_
void G200RegHelpCmd()
{
	OspRegCommand("g200help", (void*)g200help, "");
	OspRegCommand("pmatrix", (void*)pmatrix, "");
	OspRegCommand("poutrt", (void*)poutrt, "");
	OspRegCommand("psi", (void*)psi, "");
	OspRegCommand("lsdk", (void*)lsdk, "");
	OspRegCommand("lpui", (void*)lpui, "");
	OspRegCommand("lsched", (void*)lsched, "");
	OspRegCommand("lcfg", (void*)lcfg, "");
	OspRegCommand("logallon", (void*)logallon, "");
	OspRegCommand("logalloff", (void*)logalloff, "");
    OspRegCommand("pcamstatus",(void*)pcamstatus,"");
	OspRegCommand("plns",(void*)plns,"");
	OspRegCommand("g200st",(void*)g200st,"");
}
#endif

void G200Quit()
{
	OspSemDelete(g_cVcData.m_hMainSem);

    if(g_cVcData.m_tG200Cfg.IsRunPui())
    {
        PuiQuit();
    }

    OspQuit();
}

void InitEventDesc()
{
    RegisteEvent(EV_G200_VCSCHED_POWERON);
    RegisteEvent(EV_G200_SDKSSN_POWERON);
    RegisteEvent(EV_G200_CFGMGR_POWERON);


    RegisteEvent(EV_G200_CONN_MPLAT_TIMER);
    RegisteEvent(EV_G200_PUI_REG_TIMEOUT);
    RegisteEvent(EV_G200_PUI_DISCONNECT_NTY);
    RegisteEvent(EV_G200_FORCE_PUI_DISCONNECT_CMD);
    RegisteEvent(EV_G200_CFGMGR_GET_BINDRT_CMD);
    RegisteEvent(EV_G200_CFGMGR_GET_MATRIX_NTY);
    RegisteEvent(EV_G200_CFGMGR_GET_BINDINRT_NTY);
    RegisteEvent(EV_G200_CFGMGR_GET_BINDOUTRT_NTY);
    RegisteEvent(EV_G200_SDK_START_SEND_REQ);
    RegisteEvent(EV_G200_SDK_START_SEND_ACK);
    RegisteEvent(EV_G200_SDK_START_SEND_NACK);
    RegisteEvent(EV_G200_SDK_STOP_SEND_REQ);
    RegisteEvent(EV_G200_SDK_STOP_SEND_ACK);
    RegisteEvent(EV_G200_SDK_STOP_SEND_NACK);
    RegisteEvent(EV_G200_DISCONNECT_WITH_MPLAT_NTY);
    RegisteEvent(EV_G200_CONNECT_WITH_MPLAT_NTY);
    RegisteEvent(EV_G200_CONN_REGSRV_TIMER);
    RegisteEvent(EV_G200_REG_REGSRV_TIMER);
    RegisteEvent(EV_G200_CFGMGR_ADD_BINDINRT_NTY);
    RegisteEvent(EV_G200_SDK_STOP_RCV_DEV_STREAM_NTY);
    RegisteEvent(EV_G200_YJCTRL_CMD);
    RegisteEvent(PUI_CMU_PU_STATUS_NTY);
    RegisteEvent(PUI_CMU_REG_REQ);
    RegisteEvent(PUI_CMU_PU_REG_REQ);
    RegisteEvent(CMU_PUI_PU_REG_ACK);
    RegisteEvent(CMU_PUI_PU_REG_NACK);
    RegisteEvent(PUI_CMU_PU_CAPABILITY_NTY);
    RegisteEvent(PUI_CMU_PU_STATUS_NTY);
    RegisteEvent(CMU_PUI_REG_ACK);
    RegisteEvent(CMU_PUI_REG_NACK);
    RegisteEvent(EV_G200_PUI_REG_TIMEOUT);

    RegisteEvent(PUI_CMU_PU_ALARM_NTY);
    RegisteEvent(CMU_PUI_PU_SEND_TRANSCOM_CMD);
    RegisteEvent(CMU_PUI_PU_STARTSEND_CMD);
    RegisteEvent(CMU_PUI_PU_SENDKEYFRAME_CMD);
    RegisteEvent(CMU_PUI_PU_STOPSEND_CMD);

	RegisteEvent(G200_G200MS_REG_REQ);
	RegisteEvent(G200MS_G200_REG_ACK);
	RegisteEvent(G200MS_G200_REG_NACK);
	RegisteEvent(G200MS_G200_ADD_BINDINRT_NTY);
	
	RegisteEvent(BS_G200MS_G200_SYSMAIN_REQ);
	RegisteEvent(G200_G200MS_BS_SYSMAIN_ACK);
	RegisteEvent(G200_G200MS_BS_SYSMAIN_NACK);

	RegisteEvent(EV_HS_SERVER_KEDACOMXMLDATA);

	RegisteEvent(VS_MSG_CONFIG_VIDEOPARAM_REQ);
	RegisteEvent(VS_MSG_CONFIG_VIDEOPARAM_RSP);
	RegisteEvent(VS_MSG_CONFIG_VIDEOENCOSDPARAM_REQ);
	RegisteEvent(VS_MSG_CONFIG_VIDEOENCOSDPARAM_RSP);
	RegisteEvent(VS_MSG_CONFIG_VIDEOENCPARAM_REQ);
	RegisteEvent(VS_MSG_CONFIG_VIDEOENCPARAM_RSP);
	RegisteEvent(VS_MSG_CONFIG_SERIALPARAM_REQ);
	RegisteEvent(VS_MSG_CONFIG_SERIALPARAM_RSP);
	RegisteEvent(VS_MSG_CONFIG_DEV_SYSPARAM_REQ);
	RegisteEvent(VS_MSG_CONFIG_DEV_SYSPARAM_RSP);

	RegisteEvent(VS_MSG_GETCFG_VIDEOPARAM_REQ);
	RegisteEvent(VS_MSG_GETCFG_VIDEOENCOSDPARAM_REQ);
	RegisteEvent(VS_MSG_GETCFG_VIDEOENCPARAM_REQ);
	RegisteEvent(VS_MSG_GETCFG_SERIALPARAM_REQ);
}

void GetAbsPath(s8* pszPath,s32 nSize)
{
#ifdef _LINUX_
	s32 nRet = readlink("/proc/self/exe",pszPath,nSize);
	if (nRet <0 || nRet >= nSize)
	{
		strcpy(pszPath,"./");
	}
	else
	{
		s8* pszStr = strrchr(pszPath,'/');
		if (pszStr != NULL)
		{
			pszStr[0] = '\0';
		}
	}
#else
	strcat(pszPath,"./");
#endif
}


int main(int argc, char* argv[])
{
	InitEventDesc();
    s8 abyCfgFile[MAXLEN_FILEPATH+64] = {0};
    s8 achCfgPath[MAXLEN_FILEPATH] = {0}; 
	if (argc >= 2)
	{   
        strncpy(achCfgPath, argv[1], MAXLEN_FILEPATH);   
	}
    else
    {
		GetAbsPath(achCfgPath,MAXLEN_FILEPATH);
    }
    UnifyDir(achCfgPath, strlen(achCfgPath));
    strcat(achCfgPath,CONFIG_DIRECTORY);
    sprintf(abyCfgFile, "%s%s", achCfgPath, CONFIG_FILE_NAME);
	
	if(!IsOspInitd())
    {
        //Osp初始化
#ifdef _LINUX_
        OspInit(TRUE, 6000, "G200");
		G200RegHelpCmd();
#elif	_DEBUG
        OspInit(TRUE);
#else
        OspInit(FALSE);
#endif
    }
	OspSetPrompt("G200");
    g_cVcData.m_tG200Cfg.SetBindRtCfgPath(achCfgPath);

	//读取license文件
	g_cVcData.m_cLicenseHandler.InitLicense(achCfgPath);

    if(!g_cVcData.m_tG200Cfg.LoadCfg(abyCfgFile))
    {
        OspLog(LOGLVL_EXCEPTION, "Failed LoadG200Cfg\n");
        return FALSE;
    }

	time_t nTime;
	time(&nTime);
	g_cVcData.m_tG200Cfg.SetBootTime(nTime);

#ifdef _DEBUG
    memset(g_abyLogModuleLev, 255, sizeof(g_abyLogModuleLev));
#else
    memset(g_abyLogModuleLev, 1, sizeof(g_abyLogModuleLev));
#endif

	OspSemBCreate(&(g_cVcData.m_hMainSem));
	OspSemTake(g_cVcData.m_hMainSem);

    if(OspCreateTcpNode(INADDR_ANY, g_cVcData.m_tG200Cfg.GetSelfPlatPort(), TRUE) == INVALID_SOCKET)
	{
		G200Log(LOG_MAIN_MOD, LOGLVL_EXCEPTION, "创建TcpNode[%d]失败\n", g_cVcData.m_tG200Cfg.GetSelfPlatPort());
		return FALSE;
	}

	//创建APP
	g_cVcSchedApp.CreateApp("VcSched", AID_G200_VCSCHED, 80, 5000, 4<<20);
	g_cPuiSsnApp.CreateApp("G200PuiSsn", AID_CMU_PUI, 90, 5000);
	g_cSdkSsnApp.CreateApp("SdkSsn", AID_G200_SDK, 90, 5000);
	g_cCfgMgrApp.CreateApp("CfgMgr", AID_G200_CFG, 90, 2000);

	OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_SET_INST_ALIAS);
    OspPost(MAKEIID(AID_CMU_PUI, CInstance::DAEMON), EV_G200_SET_INST_ALIAS);
    OspPost(MAKEIID(AID_G200_SDK, CInstance::DAEMON), EV_G200_SET_INST_ALIAS);
    OspPost(MAKEIID(AID_G200_CFG, CInstance::DAEMON), EV_G200_SET_INST_ALIAS);

	OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_VCSCHED_POWERON);

    if(g_cVcData.m_tG200Cfg.IsRunPui())
    {
        if(!PuiStart(abyCfgFile, TRUE))
        {
            G200Log(LOG_MAIN_MOD, LOGLVL_EXCEPTION, "Failed to run pui\n");
        }
    }

	OspSemTake(g_cVcData.m_hMainSem);

    OspDelay(500);
	G200Quit();

	return TRUE;
}
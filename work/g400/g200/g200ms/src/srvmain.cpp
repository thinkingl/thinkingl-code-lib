#include "srvmain.h"
#include "devreg.h"
#include "cclientconn.h"
#include "kdvsys.h"
#include "cnetmanager.h"
#include "3assn.h"
#include "kdmsearch.h"


#define RegisteEvent(x) OspAddEventDesc(#x,x)
#define  CHECK_WRITE_FIEL_RET(data,errstring)\
if(!data) {G200MSLog(LOG_MAIN_MOD,LOGLVL_EXCEPTION,errstring);return FALSE;}
SEMHANDLE      g_hMainSem;
TG200CfgBase   g_tCfg;

CDevMgrApp g_cDevMgrApp;
CClientApp g_cClientApp;
CSdk3AApp g_cSdk3AApp;
u8 g_abyLogModuleLev[LOG_MOD_NUM];

API void lsdr(u8 byLogLev)
{
	g_abyLogModuleLev[LOG_DEVMGR_MOD] = byLogLev;
}
API void lc(u8 byLogLev)
{
	g_abyLogModuleLev[LOG_CLIENTCONN_MOD] = byLogLev;
}
API void mslao()
{
	memset(g_abyLogModuleLev, 255, sizeof(g_abyLogModuleLev));
}

API void mslof()
{
	memset(g_abyLogModuleLev, 0, sizeof(g_abyLogModuleLev));
}


API void pnr(u32 dwMatrixID=0)
{
	::OspPost(MAKEIID(AID_G200MS_DEVMGR, CInstance::DAEMON), EV_G200MS_PRINT_NEED_REG_DEV_REQ, &dwMatrixID, sizeof(dwMatrixID));
}

API void cc()
{
    LPCSTR pszXml = "<kedacomxmldata><command>4064</command><content></content></kedacomxmldata>";
    s32 nLength = strlen(pszXml) + 1;
    ::OspPost(MAKEIID(AID_G200MS_CLIENTCONN,CInstance::DAEMON),EV_HS_SERVER_KEDACOMXMLDATA,pszXml,nLength);
}

API void g200mshelp()
{
	OspPrintf(TRUE, FALSE, "G200ms Version: KDM2800 %s\tCompile Time: %s:%s\n", G200MS_CONFVERSION, __DATE__, __TIME__);	

	OspPrintf(TRUE, FALSE, "pnr<matrixid> -- 打印等待注册的设备信息\n");
	OspPrintf(TRUE, FALSE, "\t0 -- 打印所有矩阵上等待注册的设备\n");
	OspPrintf(TRUE, FALSE, "\t其他 -- 打印指定矩阵上等待注册的设备\n");

	OspPrintf(TRUE, FALSE, "lsdr<loglev> -- 打开设备管理模块的打印\n");
	OspPrintf(TRUE, FALSE, "lc<loglev> -- 打开客户端连接模块的打印\n");
	OspPrintf(TRUE, FALSE, "mslao -- 打开所有模块的打印\n");
	OspPrintf(TRUE, FALSE, "mslof -- 关闭所有模块的打印\n");
}


API void msst()
{
	::OspPost(MAKEIID(AID_G200MS_DEVMGR, CInstance::DAEMON), EV_G200MS_PRINT_G200MS_STATUS_REQ);
}

#ifdef _LINUX_
void G200MSHelpCmd()
{
	OspRegCommand("g200mshelp", (void*)g200mshelp, "");
	OspRegCommand("pnr", (void*)pnr, "");
	OspRegCommand("lsdr", (void*)lsdr, "");
	OspRegCommand("lc", (void*)lc, "");
	OspRegCommand("msst",(void*)msst,"");
	OspRegCommand("mslao", (void*)mslao, "");
	OspRegCommand("mslof", (void*)mslof, "");
}

/*========================================================================
    函 数 名：CBFEvSetDevIp
    功    能：vshunter修改ip回调函数
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人		修改内容
    2010/04/18  v1.0    liqingping  创建
========================================================================*/
void CBFEvSetDevIp(void *pUserHandle, TSetNewIpInfo tNewIpInfo)
{
	s8 achHostIp[20] = {0};
    s8 achIp[20] = {0};
    s8 achSubMask[20] = {0};
    s8 achDefaultGate[20] = {0};
	
	in_addr tInAddr;
	tInAddr.s_addr = tNewIpInfo.m_dwTargetIP;
	strcpy(achHostIp,inet_ntoa(tInAddr));
	
	tInAddr.s_addr = tNewIpInfo.m_dwNewIP;
	strcpy(achIp,inet_ntoa(tInAddr));
	
	tInAddr.s_addr =tNewIpInfo.m_dwNewMask;
	strcpy(achSubMask,inet_ntoa(tInAddr));
	
	tInAddr.s_addr = tNewIpInfo.m_dwNewGW;
	strcpy(achDefaultGate,inet_ntoa(tInAddr));
	
	BOOL32 bRet = g_cDevMgrApp.SetNicInfo(achHostIp,achIp,achSubMask,achDefaultGate);
	if (bRet)
	{
		OspSystem("reboot >/dev/null 2>&1");
	}

}
#endif

void G200MSLog(u8 byLogModule, u8 byLogLevel, const char* szFormat, ...)
{
    char szMsg[256] = {0};
	
    if(g_abyLogModuleLev[byLogModule] < byLogLevel)
        return;
	
    va_list pvlist;
    va_start(pvlist, szFormat);
    s32 actLen = vsprintf((char *)szMsg, szFormat, pvlist);
	
	if( actLen <= (s32) 0 || actLen >= (s32) sizeof(szMsg) )
	{
		OspPrintf( TRUE, FALSE, "[error]: G200MS vsprintf() failed.\n");
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

void G200MSQuit()
{
	OspSemDelete(g_hMainSem);
#ifdef _LINUX_
	KdmSearchCleanup();
#endif
    OspQuit();
}


void InitEventDesc()
{
	RegisteEvent(G200_G200MS_REG_REQ);
	RegisteEvent(G200MS_G200_REG_ACK);
	RegisteEvent(G200MS_G200_REG_NACK);
	RegisteEvent(G200MS_G200_ADD_BINDINRT_NTY);

	RegisteEvent(EV_HS_SERVER_KEDACOMXMLDATA);
	RegisteEvent(BS_G200MS_USER_LOGIN_REQ);
	RegisteEvent(G200MS_BS_USER_LOGIN_ACK);
	RegisteEvent(G200MS_BS_USER_LOGIN_NACK);

	RegisteEvent(EV_G200MS_INIT_3ASDK_TIMER);
	RegisteEvent(EV_G200MS_DISCONN_WITH_MPLAT_NTF);

	RegisteEvent(BS_G200MS_ADD_MATRIX_REQ);
	RegisteEvent(G200MS_BS_ADD_MATRIX_ACK);
	RegisteEvent(G200MS_BS_ADD_MATRIX_NACK);

	RegisteEvent(BS_G200MS_DELETE_MATRIX_REQ);
	RegisteEvent(G200MS_BS_DELETE_MATRIX_ACK);
	RegisteEvent(G200MS_BS_DELETE_MATRIX_NACK);

	RegisteEvent(BS_G200MS_UPDATE_MATRIX_REQ);
	RegisteEvent(G200MS_BS_UPDATE_MATRIX_ACK);
	RegisteEvent(G200MS_BS_UPDATE_MATRIX_NACK);

	RegisteEvent(BS_G200MS_ADD_ENCODER_REQ);
	RegisteEvent(G200_BS_ADD_ENCODER_ACK);
	RegisteEvent(G200_BS_ADD_ENCODER_NACK);

	RegisteEvent(BS_G200MS_DELETE_ENCODER_REQ);
	RegisteEvent(G200MS_BS_DELETE_ENCODER_ACK);
	RegisteEvent(G200MS_BS_DELETE_ENCODER_NACK);

	RegisteEvent(BS_G200MS_UPDATE_ENCODER_REQ);
	RegisteEvent(G200MS_BS_UPDATE_ENCODER_ACK);
	RegisteEvent(G200MS_BS_UPDATE_ENCODER_NACK);

	RegisteEvent(BS_G200MS_ADD_CAMERA_REQ);
	RegisteEvent(G200_BS_ADD_CAMERA_ACK);
	RegisteEvent(G200_BS_ADD_CAMERA_NACK);

	RegisteEvent(BS_G200MS_DELETE_CAMERA_REQ);
	RegisteEvent(G200MS_BS_DELETE_CAMERA_ACK);
	RegisteEvent(G200MS_BS_DELETE_CAMERA_NACK);

	RegisteEvent(BS_G200MS_UPDATE_CAMERA_REQ);
	RegisteEvent(G200MS_BS_UPDATE_CAMERA_ACK);
	RegisteEvent(G200MS_BS_UPDATE_CAMERA_NACK);

	RegisteEvent(BS_G200MS_ADDLINK_REQ);
	RegisteEvent(G200MS_BS_ADDLINK_ACK);
	RegisteEvent(G200MS_BS_ADDLINK_NACK);

	RegisteEvent(BS_G200MS_DELLINK_REQ);
	RegisteEvent(G200MS_BS_DELLINK_ACK);
	RegisteEvent(G200MS_BS_DELLINK_NACK);

	RegisteEvent(BS_G200MS_MATRIXSINFO_REQ);
	RegisteEvent(G200MS_BS_MATRIXSINFO_ACK);
	RegisteEvent(G200MS_BS_MATRIXSINFO_NACK);

	RegisteEvent(BS_G200MS_DEVICESLIST_REQ);
	RegisteEvent(G200MS_BS_DEVICESLIST_ACK);
	RegisteEvent(G200MS_BS_DEVICESLIST_NACK);

	RegisteEvent(BS_G200MS_UPDATE_USER_REQ);
	RegisteEvent(G200MS_BS_UPDATE_USER_ACK);
	RegisteEvent(G200MS_BS_UPDATE_USER_NACK);

	RegisteEvent(BS_G200MS_SYSCONF_REQ);
	RegisteEvent(G200MS_BS_SYSCONF_ACK);
	RegisteEvent(G200MS_BS_SYSCONF_NACK);

	RegisteEvent(BS_G200MS_SYSTEMREBOOT_REQ);
	RegisteEvent(G200MS_BS_SYSTEMREBOOT_ACK);
	RegisteEvent(G200MS_BS_SYSTEMREBOOT_NACK);

	RegisteEvent(BS_G200MS_HEARTBEAT_NTF);
	RegisteEvent(G200MS_BS_HEARTBEAT_NTF);

	RegisteEvent(BS_G200MS_SYSPARASET_REQ);
	RegisteEvent(G200MS_BS_SYSPARASET_ACK);
	RegisteEvent(G200MS_BS_SYSPARASET_NACK);

	RegisteEvent(BS_G200MS_SYSIPSET_REQ);
	RegisteEvent(G200MS_BS_SYSIPSET_ACK);
	RegisteEvent(G200MS_BS_SYSIPSET_NACK);

	RegisteEvent(BS_G200MS_G200_SYSMAIN_REQ);
	RegisteEvent(G200_G200MS_BS_SYSMAIN_ACK);
	RegisteEvent(G200_G200MS_BS_SYSMAIN_NACK);

	RegisteEvent(BS_G200MS_SYSQUIT_REQ);
	RegisteEvent(G200MS_BS_SYSQUIT_ACK);
	RegisteEvent(G200MS_BS_SYSQUIT_NACK);
	
	RegisteEvent(EV_G200MS_CLIENTCONN_TIMER);
}
/*========================================================================
    函 数 名：SpawnKdmSearch
    功    能：启动kdmsearch
    参    数：
    返 回 值：TRUE为启动成功 FALSE为启动失败
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2010/04/18  v1.0    liqingping  创建
========================================================================*/
BOOL32 SpawnKdmSearch(s8* pszPath)
{
#ifdef _LINUX_
	CNetManager cNetManager;
	BOOL32 bWriteFile = FALSE;
	s8 achFileFullName[128] = {0};
	sprintf(achFileFullName,"%s%s",pszPath,"mdcfg.ini");

	s32 nEthNum = cNetManager.GetNicNum();
	bWriteFile = SetRegKeyInt(achFileFullName,"NETINFO","EthNum",nEthNum);
	CHECK_WRITE_FIEL_RET(bWriteFile,"写入网卡数量时出错!");

	s8 achEthName[MAXLEN_ALIAS + 1];
	s8 achMacAddr[64];
	s8 achKeyName[128];
	s8 achKeyValue[128];
	NetConfigure tNetConfigure;
	for (s32 nCount = 0; nCount < nEthNum; nCount++ )
	{
		sprintf(achEthName,"eth%d",nCount);
		sprintf(achKeyName,"Eth%d",nCount);
		tNetConfigure = cNetManager.GetNicInfo(achEthName);
		BOOL32 bRet = cNetManager.GetRawMacAddr(achEthName,achMacAddr);
		if (bRet)
		{
			const s8* pszGateWay = tNetConfigure.GetNicGateWay();
			if (!strcmp(pszGateWay,""))
			{
				pszGateWay = "0.0.0.0";
			}
			sprintf(achKeyValue,"%s    %s    %s    %s",tNetConfigure.GetNicIp(),
				tNetConfigure.GetNicNetMask(),pszGateWay,achMacAddr);
			bWriteFile = SetRegKeyString(achFileFullName,"NETINFO",achKeyName,achKeyValue);
			CHECK_WRITE_FIEL_RET(bWriteFile,"写入网卡信息时出错!");
		}
		else
		{
			return FALSE;
		}
	}

	if (0 != KdmSearchStartup(NULL,CBFEvSetDevIp,pszPath))
	{
		G200MSLog(LOG_MAIN_MOD, LOGLVL_IMPORTANT,"Failed kdmsearch server.\n");
		return FALSE;
	}
	return TRUE;
#endif
	return FALSE;

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

void G200msLogFile(const s8* pszPath)
{
	s8 achFileName[128] = {0};
	s8 szLogText[256] = {0};
	sprintf(achFileName,"%s%s",pszPath,"g200ms.log");
	FILE* pFile = fopen(achFileName,"a+");
	time_t tBootTime = time(NULL);
	sprintf(szLogText,"%s:%s","g200ms is launched",ctime(&tBootTime));
	fwrite(szLogText,1,strlen(szLogText),pFile);
	fclose(pFile);
}

int main(int argc, char* argv[])
{
	//读取配置文件
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

	SpawnKdmSearch(achCfgPath);
	
	InitEventDesc();
	//G200msLogFile(achCfgPath);

	if(!IsOspInitd())
    {
        //Osp初始化
#ifdef _LINUX_
        OspInit(TRUE, 6001, "G200MS");
		G200MSHelpCmd();
#elif	_DEBUG
        OspInit(TRUE);
#else
        OspInit(FALSE);
#endif
    }
    OspSetPrompt("G200MS");
	if(!g_tCfg.LoadCfg(abyCfgFile))
    {
        G200MSLog(LOG_MAIN_MOD, LOGLVL_EXCEPTION, "Failed LoadG200Cfg\n");
        return FALSE;
    }
	g_tCfg.SetBindRtCfgPath(achCfgPath);

	time_t nTime;
	time(&nTime);
	g_tCfg.SetBootTime(nTime);

	OspSemBCreate(&g_hMainSem);
	OspSemTake(g_hMainSem);
	

#ifdef _DEBUG
    memset(g_abyLogModuleLev,255,sizeof(g_abyLogModuleLev));
#else
    memset(g_abyLogModuleLev,0,sizeof(g_abyLogModuleLev));
#endif

	if(OspCreateTcpNode(INADDR_ANY, g_tCfg.GetRegSrvListenPort(), TRUE) == INVALID_SOCKET)
	{
		G200MSLog(LOG_MAIN_MOD, LOGLVL_DEBUG1, "[G200MS] ：创建监听结点失败！\n");
		return FALSE;
	}
	
	//创建设备注册APP
	g_cDevMgrApp.CreateApp("DevMgr", AID_G200MS_DEVMGR, 80, 5000, 4<<20);
    g_cClientApp.CreateApp("ClientConn",AID_G200MS_CLIENTCONN,80,5000,4<<20);
	g_cSdk3AApp.CreateApp("Sdk3A",AID_G200MS_SDK3A,80,5000,4<<20);
	//启动设备注册APP
	OspPost(MAKEIID(AID_G200MS_DEVMGR, CInstance::DAEMON), EV_G200MS_DEVREG_POWERON);
		
	OspSemTake(g_hMainSem);
	
    OspDelay(500);
	G200MSQuit();
	
	return TRUE;
}
#include "SdkSsn.h"
#include "cmusdk.h"
#include "cmsevent.h"
#include "newcamera.h"
#include "cmserrcode.h"

BOOL32 g_bIsRegSucc = FALSE;

unsigned int g_dwG200Ip;

void VSSendStreamCallBack(VS_MESG_HEADER tMesgHeader, VS_SEND_STREAM tSendStream)
{
	CMonMsg cMonMsg;
	cMonMsg.SetReserved1(tMesgHeader.dwSerialNo);

	TSwitchInfo tSwitchInfo;
	TTransportAddr tVideoAddr;
	
	//构造源通道
	TPuChn tPuChn;
	tPuChn.SetNOString(tSendStream.tDevChn.tDevNo.abyDevNo);
	tPuChn.SetEdcChnId(tSendStream.tDevChn.byChnId);
	cMonMsg.SetMsgBody(&tPuChn, sizeof(TPuChn));
	
	tVideoAddr.SetIpAddr(tSendStream.tVideoAddr.dwIp);
	tVideoAddr.SetPort(tSendStream.tVideoAddr.wPort);
	cMonMsg.CatMsgBody(&tVideoAddr, sizeof(TTransportAddr));
	
	char achIp[18] = {0};
	IPtoString(tVideoAddr.GetIpAddr(), achIp, 18);
	printf("上级接收码流的vtdu的ip为 ：%s, port为 ：%d\n", achIp, tVideoAddr.GetPort());
	
	u8 byMode = tSendStream.byMode;
	u8 bySwitchType = tSendStream.bySwitchType;

	TTransportAddr tAudioAddr;
	tAudioAddr.SetIpAddr(tSendStream.tAudioAddr.dwIp);
	tAudioAddr.SetPort(tSendStream.tAudioAddr.wPort);

    IPtoString(tAudioAddr.GetIpAddr(),achIp,18);
	printf("上级接收码流音频的vtdu的ip为 ：%s, port为 ：%d\n", achIp, tAudioAddr.GetPort());

	cMonMsg.CatMsgBody(&byMode, sizeof(u8));
	cMonMsg.CatMsgBody(&bySwitchType, sizeof(u8));
	cMonMsg.CatMsgBody(&tAudioAddr, sizeof(TTransportAddr));

	OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_SDK_START_SEND_REQ, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
	
	return;
}

//停止发送码流回调
void VSStopStreamCallBack(VS_MESG_HEADER tMesgHeader, VS_STOP_STREAM tStopStream)
{
	CMonMsg cMonMsg;
	cMonMsg.SetReserved1(tMesgHeader.dwSerialNo);

	TChannel tSrcChn;
	TTransportAddr tTransportAddr;
	tTransportAddr.SetIpAddr(tStopStream.tVideoAddr.dwIp);
	tTransportAddr.SetPort(tStopStream.tVideoAddr.wPort);

	tSrcChn.SetNOString(tStopStream.tDevChn.tDevNo.abyDevNo);
	tSrcChn.SetEdcChnId(tStopStream.tDevChn.byChnId);

	cMonMsg.SetMsgBody(&tSrcChn, sizeof(TChannel));
	cMonMsg.CatMsgBody(&tStopStream.byMode, sizeof(u8));
	cMonMsg.CatMsgBody(&tTransportAddr,sizeof(TTransportAddr));


	OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_SDK_STOP_SEND_REQ, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
	
	return;
}

//参数配置回调
void VSDevParamCfgCallBack(VS_MESG_HEADER tMesgHeader, void *pBuf, unsigned short wBufLen)
{
	G200Log(LOG_SDK_MOD,LOGLVL_DEBUG2,"[G200][CMUSDK-CBF]:Receive Message[%s-%d]\n",OspEventDesc(tMesgHeader.wMesgType),tMesgHeader.wMesgType);
	switch(tMesgHeader.wMesgType)
	{
	case VS_MSG_CONFIG_VIDEOPARAM_REQ:	
	case VS_MSG_CONFIG_VIDEOENCOSDPARAM_REQ:	
	case VS_MSG_CONFIG_VIDEOENCPARAM_REQ:	
	case VS_MSG_CONFIG_SERIALPARAM_REQ:	
	case VS_MSG_CONFIG_DEV_SYSPARAM_REQ:
		{
			tMesgHeader.wMesgType = tMesgHeader.wMesgType+1;
			VS_CfgDevParamRsp(tMesgHeader, ERR_NOT_SUPPORT);
			G200Log(LOG_SDK_MOD,LOGLVL_DEBUG2,"[G200][CMUSDK-CBF]:Send Message[%s-%d]\n",OspEventDesc(tMesgHeader.wMesgType),tMesgHeader.wMesgType);
		}
		break;
	case VS_MSG_GETCFG_VIDEOPARAM_REQ:
		{
			VS_SRC_VIDEO_INFO tVideoInfo = {0};

			tMesgHeader.wMesgType  = tMesgHeader.wMesgType + 1;
			tMesgHeader.wMsgLen = sizeof(VS_SRC_VIDEO_INFO);
			VS_GetDevParamRsp(tMesgHeader, &tVideoInfo, ERR_NOT_SUPPORT);
			G200Log(LOG_SDK_MOD,LOGLVL_DEBUG2,"[G200][CMUSDK-CBF]:Send Message[%s-%d]\n",OspEventDesc(tMesgHeader.wMesgType),tMesgHeader.wMesgType);
		}
		break;
	case VS_MSG_GETCFG_VIDEOENCOSDPARAM_REQ:
		{
			VS_ENCODE_OSD_PARAM tEncoderOsdParam = {0};
			tMesgHeader.wMesgType = tMesgHeader.wMesgType + 1;
			tMesgHeader.wMsgLen = sizeof(VS_ENCODE_OSD_PARAM);
			VS_GetDevParamRsp(tMesgHeader,&tEncoderOsdParam,ERR_NOT_SUPPORT);
			G200Log(LOG_SDK_MOD,LOGLVL_DEBUG2,"[G200][CMUSDK-CBF]:Send Message[%s-%d]\n",OspEventDesc(tMesgHeader.wMesgType),tMesgHeader.wMesgType);
		}
		break;
	case VS_MSG_GETCFG_VIDEOENCPARAM_REQ:
		{
			VS_VIDEO_ENCODE_PARAM  tEncParam = {0};

			tMesgHeader.wMesgType = tMesgHeader.wMesgType + 1;
			tMesgHeader.wMsgLen = sizeof(VS_VIDEO_ENCODE_PARAM);
			VS_GetDevParamRsp(tMesgHeader, &tEncParam, ERR_NOT_SUPPORT);
			G200Log(LOG_SDK_MOD,LOGLVL_DEBUG2,"[G200][CMUSDK-CBF]:Send Message[%s-%d]\n",OspEventDesc(tMesgHeader.wMesgType),tMesgHeader.wMesgType);
		}
		break;
	case VS_MSG_GETCFG_SERIALPARAM_REQ:
		{
			VS_DEV_SERIAL_PARAM tSerialParam = {0};
			tMesgHeader.wMesgType = tMesgHeader.wMesgType + 1;
			tMesgHeader.wMsgLen = sizeof(VS_DEV_SERIAL_PARAM);
			VS_GetDevParamRsp(tMesgHeader, &tSerialParam, ERR_NOT_SUPPORT);
			G200Log(LOG_SDK_MOD,LOGLVL_DEBUG2,"[G200][CMUSDK-CBF]:Send Message[%s-%d]\n",OspEventDesc(tMesgHeader.wMesgType),tMesgHeader.wMesgType);
		}
		break;
	default:
		printf("[VSDevParamCfgCallBack] : 收到未知消息[%d]\n", tMesgHeader.wMesgType);
		return;
	}

	return;
}

//断链回调
void VSDisconnectPlatCallBack()
{
	//通知VcSched与上级平台断开连接
    ::OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_DISCONNECT_WITH_MPLAT_NTY);
    //通知sdk模块上级断开，准备重连
    ::OspPost(MAKEIID(AID_G200_SDK, CInstance::DAEMON), EV_G200_DISCONNECT_WITH_MPLAT_NTY);
}

//云镜控制
void VSYJCtrlCallBack(VS_DEV_NO tDevNo, unsigned char byVidChnId, VS_CAMERA_CTRL_DATA *ptCtrlData)
{
    TPuChn tCamID;
    tCamID.SetNOString(tDevNo.abyDevNo);
    tCamID.SetEdcChnId(byVidChnId);
    
    TCamInfo tCamInfo;
    tCamInfo.m_byCamType = ptCtrlData->m_byCamType;
    tCamInfo.m_byCommandId = ptCtrlData->m_byCommandId;
    tCamInfo.m_wCamId = ptCtrlData->m_wCamId;
    tCamInfo.m_byParam1 = ptCtrlData->m_byParam1;
    tCamInfo.m_byParam2 = ptCtrlData->m_byParam2;
    tCamInfo.m_byParam3 = ptCtrlData->m_byParam3;
    tCamInfo.m_byParam4 = ptCtrlData->m_byParam4;

    CMonMsg cMon;
    cMon.SetMsgBody(&tCamID,sizeof(TPuChn));
    cMon.CatMsgBody(&tCamInfo,sizeof(tCamInfo));
    ::OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON),EV_G200_YJCTRL_CMD,cMon.GetMonMsg(),cMon.GetMonMsgLen());
}


CSdkSsnIns::CSdkSsnIns()
{
    m_bInitSdk = FALSE;
}

/*====================================================================
函数名      : GetStrState
功能        : 获取状态字符串
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2009.11.5               linlifen       创建
====================================================================*/
LPCSTR CSdkSsnIns::GetStrState()
{
    switch(CurState())
    {
    case IDLE:
        return "Idle";
    case SERVICE:
        return "Service";
    default:
        return "未知状态";
    }
}

/*====================================================================
函数名      : DaemonInstanceEntry
功能        ：Daemon实例消息处理入口函数
算法实现    ：
引用全局变量：
输入参数说明：CMessage *const pcMsg,  传入的消息
              CApp* pcApp,            传入应用指针
返回值说明  ：无
----------------------------------------------------------------------
修改记录    ：
日  期      版本        修改人        修改内容
2009.11.5               linlifen       创建
====================================================================*/
void CSdkSsnIns::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{
    if(pcMsg->event == EV_G200_SET_INST_ALIAS)
    {
        char szAlias[MAX_ALIAS_LEN]={0};
        strcpy(szAlias, "[G200-SdkSsn-Daemon]");
        pcApp->SetInstAlias(CInstance::DAEMON, szAlias, (u8)strlen(szAlias));

		return;
    }

    G200Log(LOG_SDK_MOD,LOGLVL_DEBUG2,"[G200/SdkSsn-Daemon][%s]:Receive Message[%s]\n",GetStrState(),OspEventDesc(pcMsg->event));
    switch(CurState())
    {
    case IDLE:
        {
            OnIdle(pcMsg);
            break;
        }
    case SERVICE:
        {
            OnService(pcMsg);
            break;
        }
    default:
        {
            G200Log(LOG_SDK_MOD, LOGLVL_EXCEPTION, "%s[%s-%d]:未定义状态\n", m_alias, GetStrState(), CurState());
            break;
        }
    }
}


/*====================================================================
函数名      : OnIdle
功能        : 普通实例Idle状态消息处理函数
算法实现    :
引用全局变量:
输入参数说明: CMessage *const pcMsg--osp消息
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
06/03/16    3.9         陆雪忠        创建
====================================================================*/
void CSdkSsnIns::OnIdle(const CMessage *const pcMsg)
{
    switch(pcMsg->event)
    {
    case EV_G200_SDKSSN_POWERON:
        {
            post(MAKEIID(m_appId,m_instId),EV_G200_CONN_MPLAT_TIMER);
            break;
        }
    case EV_G200_CONN_MPLAT_TIMER:
        {
            //调用SDK向上级平台注册
            u32 dwIpAddr = g_cVcData.m_tG200Cfg.GetMPlatIP();
            TKDMNO tG200ID = g_cVcData.m_tG200Cfg.GetG200Id();

            //为了与上级平台断链之后能再次连接上,必须退出cmusdk库,然后再加载
			if (m_bInitSdk)
			{
                u32 wQuitRet =  VS_SdkQuit();
                if (VS_OK != wQuitRet)
                {
                    G200Log(LOG_SDK_MOD,LOGLVL_DEBUG1,"[G200/SdkSsn]: 退出cmusdk库失败 -- ErrCode:%d\n",wQuitRet);
                    SetTimer(EV_G200_CONN_MPLAT_TIMER,G200_CONN_MPLAT_INTV);
				    return;
                }  
                G200Log(LOG_SDK_MOD,LOGLVL_DEBUG1,"[G200/SdkSsn]: 退出cmusdk库成功\n");
                m_bInitSdk = FALSE;
            } 

            char szIP[MAX_IP_ADDR+1] = {0};
			IPtoString(dwIpAddr, szIP, sizeof(szIP));
			G200Log(LOG_SDK_MOD, LOGLVL_DEBUG1, "[G200/SdkSsn] : 注册的上级平台IP为 ：%s, G200编号为 ：%s\n", 
				                                szIP, tG200ID.GetNOString());

            u32 wRet = VS_SdkInit(dwIpAddr, (s8 *)tG200ID.GetNOString(), 10000, g_dwG200Ip);
			if (VS_OK != wRet)
			{
				G200Log(LOG_SDK_MOD, LOGLVL_DEBUG1, "[G200/SdkSsn] : 注册上级平台失败 -- ErrCode ：%d\n", wRet);
                SetTimer(EV_G200_CONN_MPLAT_TIMER,G200_CONN_MPLAT_INTV);
                //g_cVcSchedApp.AddRegFailNum();
				return;
			}

            m_bInitSdk = TRUE;//加载cmusdk库成功
			IPtoString(g_dwG200Ip, szIP, sizeof(szIP));
			G200Log(LOG_SDK_MOD, LOGLVL_DEBUG1, "[G200/SdkSsn] : 注册上级平台成功, LocalIp : %s！\n", szIP);

			VS_CALLBACK tCallBack = {0};
			tCallBack.fpSendStreamCB = VSSendStreamCallBack;
			tCallBack.fpStopStreamCB = VSStopStreamCallBack;
			tCallBack.fpDevParamCfgCB = VSDevParamCfgCallBack;

            tCallBack.fpYJCtrlCB = VSYJCtrlCallBack;//lqp
            tCallBack.fpDisconnPlatCB = VSDisconnectPlatCallBack;//lqp
			wRet = VS_SetCallBack(&tCallBack);

			g_bIsRegSucc = TRUE;
			
            //通知VcSched模块，向上级平台建链成功
			post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_CONNECT_WITH_MPLAT_NTY);

            NextState(SERVICE);

            break;
        }
    default:
        {
            //当前状态未定义处理消息
            G200Log(LOG_SDK_MOD, LOGLVL_IMPORTANT, "%s[%s]:收到无法处理的消息\n", m_alias, GetStrState());
            break;
        }
    }
}


/*====================================================================
函数名      : OnService
功能        : 普通实例Service状态消息处理函数
算法实现    :
引用全局变量:
输入参数说明: CMessage *const pcMsg--osp消息
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
06/03/16    3.9         陆雪忠        创建
====================================================================*/
void CSdkSsnIns::OnService(const CMessage *const pcMsg)
{
	unsigned short wRet = VS_OK;
    CMonMsg cMonMsg( pcMsg->content, pcMsg->length );
    switch(pcMsg->event)
    {
	case EV_G200_DISCONNECT_WITH_MPLAT_NTY:
		{
			//在空闲状态下，通知连接上级平台
            NextState(IDLE);
            SetTimer(EV_G200_CONN_MPLAT_TIMER,G200_CONN_MPLAT_INTV);
            //g_cVcSchedApp.ClearRegFailNum();//置上一次连接失败次数为零
		}
		break;
	case EV_G200_SDK_STOP_RCV_DEV_STREAM_NTY:
		{
			TPuChn *ptPuChn = (TPuChn *)cMonMsg.GetMsgBody();
			u32 dwIp = *(u32*)cMonMsg.GetMsgBody(sizeof(TPuChn));
			u16 wPort = *(u16*)cMonMsg.GetMsgBody(sizeof(TPuChn)+sizeof(u32));
			u32 dwAudioIp = *(u32*)cMonMsg.GetMsgBody(sizeof(TPuChn)+sizeof(u32)+sizeof(u16));
			u16 wAudioPort = *(u16*)cMonMsg.GetMsgBody(sizeof(TPuChn)+sizeof(u32)+sizeof(u16)+sizeof(u32));

			VS_DEV_CHN tDevChn = {0};
			strncpy(tDevChn.tDevNo.abyDevNo, ptPuChn->GetNOString(), VS_MAX_DEVNO_LEN);
			tDevChn.byChnId = ptPuChn->GetEdcChnId();

			VS_STOP_STREAM tVs_Stop_Stream = {0};
			tVs_Stop_Stream.tDevChn = tDevChn;
			tVs_Stop_Stream.tVideoAddr.dwIp = dwIp;
			tVs_Stop_Stream.tVideoAddr.wPort = wPort;
			tVs_Stop_Stream.tAudioAddr.dwIp = dwAudioIp;
			tVs_Stop_Stream.tAudioAddr.wPort = wAudioPort;
			tVs_Stop_Stream.dwPlayHander = VS_USELESS_U32;

			VS_StopRcvStreamNty(tVs_Stop_Stream);
		}
		break;
	case PUI_CMU_PU_STATUS_NTY:
		{
			TPuStatus *ptPuStatus = (TPuStatus *)cMonMsg.GetMsgBody();
			if (NULL != ptPuStatus)
			{
				VS_DEV_INFO vsDevInfo = {0};

				strncpy(vsDevInfo.tDevBaseInfo.tDevNo.abyDevNo, ptPuStatus->GetNOString(), LEN_KDM_NO);
				vsDevInfo.tDevBaseInfo.byCamNum = 1;
				vsDevInfo.tDevBaseInfo.byDevType = DEVTYPE_ENCODER;
				strcpy(vsDevInfo.tDevBaseInfo.m_szManuFac, "kedacom");
				
				vsDevInfo.tDevBaseCapInfo.byAlarmInNum = ptPuStatus->GetAlarmInNum();
				vsDevInfo.tDevBaseCapInfo.byAlarmOutNum = ptPuStatus->GetAlarmOutNum();
				vsDevInfo.tDevBaseCapInfo.byAudioDecNum = ptPuStatus->GetAudioDecNum();
				vsDevInfo.tDevBaseCapInfo.byAudioEncNum = ptPuStatus->GetAudioEncNum();
				vsDevInfo.tDevBaseCapInfo.bySerialPortNum = ptPuStatus->GetSerialPortNum();
				vsDevInfo.tDevBaseCapInfo.byVideoDecNum = ptPuStatus->GetVideoDecNum();
				vsDevInfo.tDevBaseCapInfo.byVideoEncNum = ptPuStatus->GetVideoEncNum();
				vsDevInfo.tDevBaseCapInfo.byVideoInPortNum = ptPuStatus->GetVideoInPortNum();

				vsDevInfo.tDevStatusInfo.abyVidInputPortNum[0] = 1;
				vsDevInfo.tDevStatusInfo.abyVidInputPort[0][0] = 0;
				vsDevInfo.tDevStatusInfo.abyMediaType[0] = VS_MEDIA_TYPE_MP4;
				vsDevInfo.tDevStatusInfo.abyVidSrcChn[0] = 0;
				vsDevInfo.tDevStatusInfo.abyDnyValue[0] = ptPuStatus->GetDynValue(0);
				vsDevInfo.tDevStatusInfo.byDirectTransNum = ptPuStatus->GetDirectTransNum();
				vsDevInfo.tDevStatusInfo.byIsDecCombined = ptPuStatus->IsDecCombined();
				vsDevInfo.tDevStatusInfo.byIsOnline = ptPuStatus->IsOnLine() ? 1 : 0;

				VS_DevInfoNty(VS_MSG_DEV_INFO_NTY, &vsDevInfo, sizeof(VS_DEV_INFO));
			}
		}
		break;
	case EV_G200_SDK_START_SEND_ACK:
		{
			VS_MESG_HEADER tMesgHeader = {0};
			tMesgHeader.dwSerialNo = cMonMsg.GetReserved1();
			tMesgHeader.wMesgType = VS_MSG_SEND_STREAM_RSP;

			TTransportAddr* ptVideoAddr = (TTransportAddr *)cMonMsg.GetMsgBody();
			TTransportAddr* ptAudioAddr = (TTransportAddr *)cMonMsg.GetMsgBody(sizeof(TTransportAddr));

			VS_RTCP_ADDR tRtcpAddr = {0};
			tRtcpAddr.tVideoRtcpAddr.dwIp = ptVideoAddr->GetIpAddr();
			tRtcpAddr.tVideoRtcpAddr.wPort = ptVideoAddr->GetPort();
			tRtcpAddr.tAudioRtcpAddr.dwIp = ptAudioAddr->GetIpAddr();
			tRtcpAddr.tAudioRtcpAddr.wPort = ptAudioAddr->GetPort();

			VS_SendStreamRsp(tMesgHeader, tRtcpAddr, 0);
		}
		break;
	case EV_G200_SDK_START_SEND_NACK:
		{
			VS_MESG_HEADER tMesgHeader = {0};
			tMesgHeader.dwSerialNo = cMonMsg.GetReserved1();
			tMesgHeader.wMesgType = VS_MSG_SEND_STREAM_RSP;
			
			VS_RTCP_ADDR tRtcpAddr = {0};
			VS_SendStreamRsp(tMesgHeader, tRtcpAddr, 1);
		}
		break;
	case EV_G200_SDK_STOP_SEND_ACK:
		{
			VS_MESG_HEADER tMesgHeader = {0};
			tMesgHeader.dwSerialNo = cMonMsg.GetReserved1();
			tMesgHeader.wMesgType = VS_MSG_STOP_STREAM_RSP;

			VS_StopStreamRsp(tMesgHeader, 0);
		}
		break;
	case EV_G200_SDK_STOP_SEND_NACK:
		{
			VS_MESG_HEADER tMesgHeader = {0};
			tMesgHeader.dwSerialNo = cMonMsg.GetReserved1();
			tMesgHeader.wMesgType = VS_MSG_STOP_STREAM_RSP;
			
			VS_StopStreamRsp(tMesgHeader, 1);
		}
		break;
    default:
        {
			G200Log(LOG_SDK_MOD, LOGLVL_IMPORTANT, "%s[%s]:无法处理的消息[%s-%d]\n", m_alias, GetStrState(),OspEventDesc(pcMsg->event),pcMsg->event);
            break;
        }
    }
}

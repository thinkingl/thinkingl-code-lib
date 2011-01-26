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
	
	//����Դͨ��
	TPuChn tPuChn;
	tPuChn.SetNOString(tSendStream.tDevChn.tDevNo.abyDevNo);
	tPuChn.SetEdcChnId(tSendStream.tDevChn.byChnId);
	cMonMsg.SetMsgBody(&tPuChn, sizeof(TPuChn));
	
	tVideoAddr.SetIpAddr(tSendStream.tVideoAddr.dwIp);
	tVideoAddr.SetPort(tSendStream.tVideoAddr.wPort);
	cMonMsg.CatMsgBody(&tVideoAddr, sizeof(TTransportAddr));
	
	char achIp[18] = {0};
	IPtoString(tVideoAddr.GetIpAddr(), achIp, 18);
	printf("�ϼ�����������vtdu��ipΪ ��%s, portΪ ��%d\n", achIp, tVideoAddr.GetPort());
	
	u8 byMode = tSendStream.byMode;
	u8 bySwitchType = tSendStream.bySwitchType;

	TTransportAddr tAudioAddr;
	tAudioAddr.SetIpAddr(tSendStream.tAudioAddr.dwIp);
	tAudioAddr.SetPort(tSendStream.tAudioAddr.wPort);

    IPtoString(tAudioAddr.GetIpAddr(),achIp,18);
	printf("�ϼ�����������Ƶ��vtdu��ipΪ ��%s, portΪ ��%d\n", achIp, tAudioAddr.GetPort());

	cMonMsg.CatMsgBody(&byMode, sizeof(u8));
	cMonMsg.CatMsgBody(&bySwitchType, sizeof(u8));
	cMonMsg.CatMsgBody(&tAudioAddr, sizeof(TTransportAddr));

	OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_SDK_START_SEND_REQ, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
	
	return;
}

//ֹͣ���������ص�
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

//�������ûص�
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
		printf("[VSDevParamCfgCallBack] : �յ�δ֪��Ϣ[%d]\n", tMesgHeader.wMesgType);
		return;
	}

	return;
}

//�����ص�
void VSDisconnectPlatCallBack()
{
	//֪ͨVcSched���ϼ�ƽ̨�Ͽ�����
    ::OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_DISCONNECT_WITH_MPLAT_NTY);
    //֪ͨsdkģ���ϼ��Ͽ���׼������
    ::OspPost(MAKEIID(AID_G200_SDK, CInstance::DAEMON), EV_G200_DISCONNECT_WITH_MPLAT_NTY);
}

//�ƾ�����
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
������      : GetStrState
����        : ��ȡ״̬�ַ���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2009.11.5               linlifen       ����
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
        return "δ֪״̬";
    }
}

/*====================================================================
������      : DaemonInstanceEntry
����        ��Daemonʵ����Ϣ������ں���
�㷨ʵ��    ��
����ȫ�ֱ�����
�������˵����CMessage *const pcMsg,  �������Ϣ
              CApp* pcApp,            ����Ӧ��ָ��
����ֵ˵��  ����
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
2009.11.5               linlifen       ����
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
            G200Log(LOG_SDK_MOD, LOGLVL_EXCEPTION, "%s[%s-%d]:δ����״̬\n", m_alias, GetStrState(), CurState());
            break;
        }
    }
}


/*====================================================================
������      : OnIdle
����        : ��ͨʵ��Idle״̬��Ϣ������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: CMessage *const pcMsg--osp��Ϣ
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
06/03/16    3.9         ½ѩ��        ����
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
            //����SDK���ϼ�ƽ̨ע��
            u32 dwIpAddr = g_cVcData.m_tG200Cfg.GetMPlatIP();
            TKDMNO tG200ID = g_cVcData.m_tG200Cfg.GetG200Id();

            //Ϊ�����ϼ�ƽ̨����֮�����ٴ�������,�����˳�cmusdk��,Ȼ���ټ���
			if (m_bInitSdk)
			{
                u32 wQuitRet =  VS_SdkQuit();
                if (VS_OK != wQuitRet)
                {
                    G200Log(LOG_SDK_MOD,LOGLVL_DEBUG1,"[G200/SdkSsn]: �˳�cmusdk��ʧ�� -- ErrCode:%d\n",wQuitRet);
                    SetTimer(EV_G200_CONN_MPLAT_TIMER,G200_CONN_MPLAT_INTV);
				    return;
                }  
                G200Log(LOG_SDK_MOD,LOGLVL_DEBUG1,"[G200/SdkSsn]: �˳�cmusdk��ɹ�\n");
                m_bInitSdk = FALSE;
            } 

            char szIP[MAX_IP_ADDR+1] = {0};
			IPtoString(dwIpAddr, szIP, sizeof(szIP));
			G200Log(LOG_SDK_MOD, LOGLVL_DEBUG1, "[G200/SdkSsn] : ע����ϼ�ƽ̨IPΪ ��%s, G200���Ϊ ��%s\n", 
				                                szIP, tG200ID.GetNOString());

            u32 wRet = VS_SdkInit(dwIpAddr, (s8 *)tG200ID.GetNOString(), 10000, g_dwG200Ip);
			if (VS_OK != wRet)
			{
				G200Log(LOG_SDK_MOD, LOGLVL_DEBUG1, "[G200/SdkSsn] : ע���ϼ�ƽ̨ʧ�� -- ErrCode ��%d\n", wRet);
                SetTimer(EV_G200_CONN_MPLAT_TIMER,G200_CONN_MPLAT_INTV);
                //g_cVcSchedApp.AddRegFailNum();
				return;
			}

            m_bInitSdk = TRUE;//����cmusdk��ɹ�
			IPtoString(g_dwG200Ip, szIP, sizeof(szIP));
			G200Log(LOG_SDK_MOD, LOGLVL_DEBUG1, "[G200/SdkSsn] : ע���ϼ�ƽ̨�ɹ�, LocalIp : %s��\n", szIP);

			VS_CALLBACK tCallBack = {0};
			tCallBack.fpSendStreamCB = VSSendStreamCallBack;
			tCallBack.fpStopStreamCB = VSStopStreamCallBack;
			tCallBack.fpDevParamCfgCB = VSDevParamCfgCallBack;

            tCallBack.fpYJCtrlCB = VSYJCtrlCallBack;//lqp
            tCallBack.fpDisconnPlatCB = VSDisconnectPlatCallBack;//lqp
			wRet = VS_SetCallBack(&tCallBack);

			g_bIsRegSucc = TRUE;
			
            //֪ͨVcSchedģ�飬���ϼ�ƽ̨�����ɹ�
			post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_CONNECT_WITH_MPLAT_NTY);

            NextState(SERVICE);

            break;
        }
    default:
        {
            //��ǰ״̬δ���崦����Ϣ
            G200Log(LOG_SDK_MOD, LOGLVL_IMPORTANT, "%s[%s]:�յ��޷��������Ϣ\n", m_alias, GetStrState());
            break;
        }
    }
}


/*====================================================================
������      : OnService
����        : ��ͨʵ��Service״̬��Ϣ������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: CMessage *const pcMsg--osp��Ϣ
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
06/03/16    3.9         ½ѩ��        ����
====================================================================*/
void CSdkSsnIns::OnService(const CMessage *const pcMsg)
{
	unsigned short wRet = VS_OK;
    CMonMsg cMonMsg( pcMsg->content, pcMsg->length );
    switch(pcMsg->event)
    {
	case EV_G200_DISCONNECT_WITH_MPLAT_NTY:
		{
			//�ڿ���״̬�£�֪ͨ�����ϼ�ƽ̨
            NextState(IDLE);
            SetTimer(EV_G200_CONN_MPLAT_TIMER,G200_CONN_MPLAT_INTV);
            //g_cVcSchedApp.ClearRegFailNum();//����һ������ʧ�ܴ���Ϊ��
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
			G200Log(LOG_SDK_MOD, LOGLVL_IMPORTANT, "%s[%s]:�޷��������Ϣ[%s-%d]\n", m_alias, GetStrState(),OspEventDesc(pcMsg->event),pcMsg->event);
            break;
        }
    }
}

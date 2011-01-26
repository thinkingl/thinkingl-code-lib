#include "VcSsn.h"
#include "G200Data.h"
#include "cmsevent.h"
#include "newcamera.h"

extern CVcData g_cVcData;

BOOL32 CVcSchedIns::SendMsgToSdkSsn(const CMonMsg &cMonMsg, u16 wEvent)
{
	post(MAKEIID(AID_G200_SDK, CInstance::DAEMON), wEvent, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
	return TRUE;
}

void CVcSchedIns::NotifyDstStopRcvStream(const TKDMNO& tKdmNo)
{
	TVideoOutRelation *ptOutRt = g_cVcData.GetVideoOutRt(tKdmNo);
    if(ptOutRt==NULL)
    {
        G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "%s[%s]:OnPuDisconn wrong PuID[%s] received!\n", m_alias, GetStrState(), tKdmNo.GetNOString());
        return;
    }

	CMonMsg cMonMsg;
	TPuChn tPuChn;
	u8 byEncChnId = 0;
	u8 byVidChnId = 0;
	u32 dwIp = 0;
	u16 wPort = 0;
	u32 dwAudioIp = 0;
	u16 wAudioPort = 0;
	
	TSwitchDataList *ptSwitchDataList  = g_cVcData.GetSwitchDataListPtr();
	if (ptSwitchDataList->GetItemCount() == 0)
	{
		return;
	}

	TSwitchData *ptSwitchData = ptSwitchDataList->PosFirst();
	while(NULL != ptSwitchData)
	{
		if (ptSwitchData->GetEncNO() == tKdmNo)
		{
			byEncChnId = ptSwitchData->GetEncChnId();
			byVidChnId = ptSwitchData->GetVidChnId();
			dwIp = ptSwitchData->GetVideoIp();
			wPort = ptSwitchData->GetVideoIpPort();
			dwAudioIp = ptSwitchData->GetAudioIp();
			wAudioPort = ptSwitchData->GetAudioIpPort();

			ptOutRt->ResetEncStatus(byEncChnId, byVidChnId);
			
			//通知上级平台停止接收对应摄像头的码流
			tPuChn.SetKdmNO(ptSwitchData->GetCamNO());
			tPuChn.SetEdcChnId(0);
			cMonMsg.SetMsgBody(&tPuChn, sizeof(TPuChn));
			cMonMsg.CatMsgBody(&dwIp,sizeof(dwIp));
			cMonMsg.CatMsgBody(&wPort,sizeof(wPort));
			cMonMsg.CatMsgBody(&dwAudioIp,sizeof(dwAudioIp));
			cMonMsg.CatMsgBody(&wAudioPort,sizeof(wAudioPort));

			SendMsgToSdkSsn(cMonMsg, EV_G200_SDK_STOP_RCV_DEV_STREAM_NTY);
			
			//删除交换数据
			ptSwitchDataList->DelItem(ptSwitchData);
			ptSwitchData = ptSwitchDataList->PosCur();
		}
		else
		{
			ptSwitchData = ptSwitchDataList->PosNext(ptSwitchData);
		}
	}
}

void CVcSchedIns::ProcMsgFromSdk(const CMessage * pcMsg)
{
	switch(pcMsg->event)
	{
	case EV_G200_SDK_START_SEND_REQ:
		{
			ProcSendStream(pcMsg);
		}
		break;
	case EV_G200_SDK_STOP_SEND_REQ:
		{
			ProcStopStream(pcMsg);
		}
		break;
    case EV_G200_YJCTRL_CMD:
        {
            //处理云镜控制
            CMonMsg* pcMon = (CMonMsg*)pcMsg->content;
            TPuChn tCamId;
            TCamInfo tCamInfo;
            pcMon->GetMsgBody(&tCamId,sizeof(tCamId));
            pcMon->GetMsgBody(&tCamInfo,sizeof(tCamInfo),sizeof(TPuChn));
            
            TVideoInRelation *ptCamVideo = g_cVcData.GetVideoInRT(tCamId.GetKdmNo());
            TSwitchData *ptSwitchData = g_cVcData.GetSwitchData(tCamId.GetKdmNo());
            if (ptCamVideo == NULL || ptSwitchData == NULL)
            {
                G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG1,"[G200/VcSdk][PTZ][%s]:不存在视频源",GetStrState());
				return;//摄像头不存在
            }

             //找到一个在线的，连接了矩阵的编码器
            TVideoOutRTList * pEncoderList = g_cVcData.GetVideoOutRTListPtr(ptCamVideo->GetMatrixID());
            if (pEncoderList == NULL || pEncoderList->GetItemCount() == 0)
            {
                G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG1,"[G200/VcSdk][PTZ][%s]:没有找到任何编码器!",GetStrState());
				return;
            }
            TVideoOutRelation *ptEncoder = pEncoderList->PosCur();
            while (ptEncoder)
            {
                if (ptEncoder->GetEncData().GetPuStatus().IsOnLine() && ptEncoder->IsConnMatrix())
                {
                    break;
                }
                ptEncoder = pEncoderList->PosNext(ptEncoder);
            }
            if (!ptEncoder)
            {
                G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG1,"[G200/VcSdk][PTZ][%s]:没有找到可以发送串口命令的编码器!",GetStrState());
				return;//没有找到编码器
            }

			u8 byMatrixType = ptCamVideo->GetMatrixType();
			if (MATRIX_TYPE_PECLO == byMatrixType || MATRIX_TYPE_AB == byMatrixType ||
				MATRIX_TYPE_AD == byMatrixType || MATRIX_TYPE_INFORNOVA == byMatrixType)
			{
				//在对摄像头控制之前，矩阵上必须切换到当前摄像头
				TChannel tChn;
				tChn.SetNOString(ptEncoder->GetEncData().GetPuStatus().GetNOString());
				tChn.SetVidChnId(0);
				
				TMatrixInfo tMatrixInfo;
				memset(&tMatrixInfo, 0, sizeof(tMatrixInfo));			
				tMatrixInfo.m_byCamType = byMatrixType;
				tMatrixInfo.m_byCommandID = G200_CAM_CMD_MATRIX_SWTICH;		
				tMatrixInfo.m_abyInputList[0] = (u8)ptSwitchData->GetVideoInPort();
				tMatrixInfo.m_byOutputPort = (u8)ptSwitchData->GetVideoOutPort();
				
				if (!MatrxiCtrl(tChn, tMatrixInfo))
				{
					G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG1,"[G200/VcSdk][PTZ][%s]:PTZ控制时,切换到当前摄像头失败!",GetStrState());
					return;
				}
			}

            //设置发送给pui的云镜控制
            TCamCtrl tCamCtrl;
            TCameraInfo tCameraInfo;
            tCamCtrl.SetCommandId(tCamInfo.m_byCommandId);
            tCamCtrl.SetParam1(tCamInfo.m_byParam1);
            tCamCtrl.SetParam2(tCamInfo.m_byParam2);
            tCamCtrl.SetParam3(tCamInfo.m_byParam3);
            tCamCtrl.SetParam4(tCamInfo.m_byParam4);

            tCameraInfo.SetCamId(ptCamVideo->GetVideoInPort());//设置摄像头的输入端口号
            tCameraInfo.SetType(byMatrixType);//设置矩阵的类型

            TPuChn tEncoderChn;
            tEncoderChn.SetKdmNO(ptEncoder->GetEncData().GetPuStatus().GetKdmNo());
			//ptSwitchData->GetVidChnId()
            tEncoderChn.SetVidChnId(0);
               
            CMonMsg cMon;
            cMon.SetDstID(ptEncoder->GetEncData().GetPuStatus().GetKdmNo());
            cMon.SetMsgBody(&tEncoderChn,sizeof(tEncoderChn));
            cMon.CatMsgBody(&tCamCtrl,sizeof(tCamCtrl));
            cMon.CatMsgBody(&tCameraInfo,sizeof(tCameraInfo));
            
            u16 wInstId = ptEncoder->GetEncData().GetInstId();

            SendMsgToPuiSsn(wInstId,CMU_PUI_PU_YJCTRL_CMD,cMon.GetMonMsg(),cMon.GetMonMsgLen());

            G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG1,"[G200/VcSdk][PTZ][%s]:选择编码器(%s)发送摄像头(%s)控制命令!\n",GetStrState(),
                ptEncoder->GetEncData().GetPuStatus().GetNOString(),tCamId.GetNOString());
        }
        break;
	default:
		{
			G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "[G200/VcSdk][%s] : 收到无法处理的消息[%s-%d]\n", GetStrState(), OspEventDesc(pcMsg->event),pcMsg->event);
			break;
		}
	}
}
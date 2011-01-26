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
			
			//֪ͨ�ϼ�ƽֹ̨ͣ���ն�Ӧ����ͷ������
			tPuChn.SetKdmNO(ptSwitchData->GetCamNO());
			tPuChn.SetEdcChnId(0);
			cMonMsg.SetMsgBody(&tPuChn, sizeof(TPuChn));
			cMonMsg.CatMsgBody(&dwIp,sizeof(dwIp));
			cMonMsg.CatMsgBody(&wPort,sizeof(wPort));
			cMonMsg.CatMsgBody(&dwAudioIp,sizeof(dwAudioIp));
			cMonMsg.CatMsgBody(&wAudioPort,sizeof(wAudioPort));

			SendMsgToSdkSsn(cMonMsg, EV_G200_SDK_STOP_RCV_DEV_STREAM_NTY);
			
			//ɾ����������
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
            //�����ƾ�����
            CMonMsg* pcMon = (CMonMsg*)pcMsg->content;
            TPuChn tCamId;
            TCamInfo tCamInfo;
            pcMon->GetMsgBody(&tCamId,sizeof(tCamId));
            pcMon->GetMsgBody(&tCamInfo,sizeof(tCamInfo),sizeof(TPuChn));
            
            TVideoInRelation *ptCamVideo = g_cVcData.GetVideoInRT(tCamId.GetKdmNo());
            TSwitchData *ptSwitchData = g_cVcData.GetSwitchData(tCamId.GetKdmNo());
            if (ptCamVideo == NULL || ptSwitchData == NULL)
            {
                G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG1,"[G200/VcSdk][PTZ][%s]:��������ƵԴ",GetStrState());
				return;//����ͷ������
            }

             //�ҵ�һ�����ߵģ������˾���ı�����
            TVideoOutRTList * pEncoderList = g_cVcData.GetVideoOutRTListPtr(ptCamVideo->GetMatrixID());
            if (pEncoderList == NULL || pEncoderList->GetItemCount() == 0)
            {
                G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG1,"[G200/VcSdk][PTZ][%s]:û���ҵ��κα�����!",GetStrState());
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
                G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG1,"[G200/VcSdk][PTZ][%s]:û���ҵ����Է��ʹ�������ı�����!",GetStrState());
				return;//û���ҵ�������
            }

			u8 byMatrixType = ptCamVideo->GetMatrixType();
			if (MATRIX_TYPE_PECLO == byMatrixType || MATRIX_TYPE_AB == byMatrixType ||
				MATRIX_TYPE_AD == byMatrixType || MATRIX_TYPE_INFORNOVA == byMatrixType)
			{
				//�ڶ�����ͷ����֮ǰ�������ϱ����л�����ǰ����ͷ
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
					G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG1,"[G200/VcSdk][PTZ][%s]:PTZ����ʱ,�л�����ǰ����ͷʧ��!",GetStrState());
					return;
				}
			}

            //���÷��͸�pui���ƾ�����
            TCamCtrl tCamCtrl;
            TCameraInfo tCameraInfo;
            tCamCtrl.SetCommandId(tCamInfo.m_byCommandId);
            tCamCtrl.SetParam1(tCamInfo.m_byParam1);
            tCamCtrl.SetParam2(tCamInfo.m_byParam2);
            tCamCtrl.SetParam3(tCamInfo.m_byParam3);
            tCamCtrl.SetParam4(tCamInfo.m_byParam4);

            tCameraInfo.SetCamId(ptCamVideo->GetVideoInPort());//��������ͷ������˿ں�
            tCameraInfo.SetType(byMatrixType);//���þ��������

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

            G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG1,"[G200/VcSdk][PTZ][%s]:ѡ�������(%s)��������ͷ(%s)��������!\n",GetStrState(),
                ptEncoder->GetEncData().GetPuStatus().GetNOString(),tCamId.GetNOString());
        }
        break;
	default:
		{
			G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "[G200/VcSdk][%s] : �յ��޷��������Ϣ[%s-%d]\n", GetStrState(), OspEventDesc(pcMsg->event),pcMsg->event);
			break;
		}
	}
}
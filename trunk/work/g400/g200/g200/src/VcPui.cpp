#include "VcSsn.h"
#include "G200Data.h"
#include "cmserrcode.h"
#include "cmsevent.h"

extern CVcData  g_cVcData;

BOOL32 CVcSchedIns::SendMsgToPuiSsn(const u16 wInstId, u16 wEvent, const u8 * const pbyMsg, u16 wLen)
{
	if( wInstId > G200_MAX_PUI_SSN_NUM|| wInstId==0)
    {
        ::G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "%s[%s]:SendMsgToPuiSsn[%s--%u] wrong instance id %u!\n", m_alias,
            GetStrState(), ::OspEventDesc(wEvent), wEvent, wInstId );
		
        return FALSE ;
    }
	
    TPuiData *ptPuiData=g_cVcData.GetPuiData(wInstId);
    if(ptPuiData == NULL)
    {
        G200Log(LOG_SCHED_MOD, LOGLVL_EXCEPTION, "%s[%s]:SendMsgToPuiSsn[%s--%u]--Can't find the pui from insid[%u]\n", m_alias,
            GetStrState(), ::OspEventDesc(wEvent), wEvent, wInstId);
		
        return FALSE;
    }
	
    if(!ptPuiData->IsConnected())
    {
        ::G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "%s[%s]:SendMsgToPuiSsn[%s--%u] wrong because of pui[%s] is not connected !\n",
            m_alias, GetStrState(), ::OspEventDesc(wEvent), wEvent, ptPuiData->GetBasePui().GetNOString());
		
        return FALSE ;
    }
    else
    {
        ::G200Log(LOG_SCHED_MOD,  LOGLVL_DEBUG1, "%s[%s]:Send Message[%s--%u] to pui[%s].\n", m_alias, GetStrState(),
                                                 ::OspEventDesc(wEvent), wEvent, ptPuiData->GetBasePui().GetNOString());
        post( MAKEIID( AID_CMU_PUI, wInstId ), wEvent, pbyMsg, wLen );
        return TRUE ;
    }
}

void CVcSchedIns::ProcPuiReg(const CMessage * pcMsg)
{
	CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;
	TPuiRegReq *pPuiRegMsg = (TPuiRegReq *)(pcMonMsg->GetMsgBody());

    if(CurState() != STATE_SERVICE)
    {
        G200Log(LOG_SCHED_MOD, LOGLVL_IMPORTANT, "[ERROR/VCPUI]CVcSchedIns:Pui Reg Failed(no enter service)\n");

        post(pcMsg->srcid, pcMsg->event + 2, pcMonMsg->GetMonMsg(), pcMonMsg->GetMonMsgLen());

        return;
    }

    if( pPuiRegMsg->GetNOType() != NOFIELD_TYPE_PLATFORMDEVICE ||
        pPuiRegMsg->GetDeviceGlobalType() != DEVICE_GLOBAL_TYPE_PUI)
    {
        G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "[ERROR/VCPUI]CVcSchedIns::ProcPuiReg pui[%s] is not type pui!\n", pPuiRegMsg->GetNOString());

        post(pcMsg->srcid, pcMsg->event + 2, pcMonMsg->GetMonMsg(), pcMonMsg->GetMonMsgLen());

        return;
    }

	TPuiData *ptPuiData = g_cVcData.GetPuiData(pPuiRegMsg->GetKdmNo());
	if(ptPuiData == NULL)
    {
		TBasePui tBasePui;
		tBasePui.SetNOString(pPuiRegMsg->GetNOString());
        ptPuiData = g_cVcData.AddPuiDev(tBasePui);

		if (NULL == ptPuiData)
		{
			G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "[ERROR/VCPUI]CVcSchedIns::ProcPuiReg Add Pui Data Failed!\n");
			post(pcMsg->srcid, pcMsg->event + 2, pcMonMsg->GetMonMsg(), pcMonMsg->GetMonMsgLen());
			return;
		}
    }

    if(ptPuiData->IsConnected())
    {
        G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "[ERROR/VCPUI]CVcSchedIns::ProcPuiReg the Same Pui[%s-%d] has registered!\n", pPuiRegMsg->GetNOString(), ptPuiData->GetInstId());

        post(pcMsg->srcid, pcMsg->event + 2, pcMonMsg->GetMonMsg(), pcMonMsg->GetMonMsgLen());

        return;
    }
    else
    {
        ptPuiData->SetConnected(TRUE);
        ptPuiData->SetInstId(GETINS(pcMsg->srcid));
		ptPuiData->SetMaxPuNum(pPuiRegMsg->GetMaxPuNum());

		ptPuiData->SetIpNum(1);
		ptPuiData->SetIpAddr(0, pPuiRegMsg->GetIpAddr(0));
		ptPuiData->SetCurPuNum(0);
    }

	post(pcMsg->srcid, pcMsg->event + 1, pcMonMsg->GetMonMsg(), pcMonMsg->GetMonMsgLen());
}

void CVcSchedIns::ProcPuReg(const CMessage * pcMsg)
{
	CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;
	TPuRegReq *ptPuRegMsg = (TPuRegReq *)(pcMonMsg->GetMsgBody());

    u32 dwPltIp;
	pcMonMsg->GetMsgBody(&dwPltIp, sizeof(u32), sizeof(TPuRegReq));
  
	OspPrintf(TRUE, FALSE, "收到编码器[%s]的注册请求信息！\n", ptPuRegMsg->GetNOString());

    if( ptPuRegMsg->GetNOType() != NOFIELD_TYPE_PRECEDINGDEVICE)
    {
        G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "[ERROR/VCPUI]CVcSchedIns::ProcPuReg pu %s is not type pu!\n", ptPuRegMsg->GetNOString());

        pcMonMsg->ReverseID();
        pcMonMsg->SetErrorCode(ERR_CMU_PU_INVALID);
        post(pcMsg->srcid, CMU_PUI_PU_REG_NACK, (u8* const)pcMonMsg, pcMonMsg->GetMonMsgLen());
        return;
    }

    TVideoOutRelation *ptOutRt = g_cVcData.GetVideoOutRt(ptPuRegMsg->GetKdmNo());

    if(ptOutRt == NULL)
    {
        G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "[ERROR/VCPUI]CVcSchedIns::ProcPuReg pu[%s] is not in topo!\n", ptPuRegMsg->GetNOString());

        pcMonMsg->ReverseID();
        pcMonMsg->SetErrorCode(ERR_CMU_PU_INVALID);
        post(pcMsg->srcid, CMU_PUI_PU_REG_NACK, (u8* const)pcMonMsg, pcMonMsg->GetMonMsgLen());
        return;
    }

	TEncData& tEncData = ptOutRt->GetEncData();
    if(tEncData.GetPuStatus().IsOnLine())
    {
        G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "[ERROR/VCPUI]CVcSchedIns::ProcPuReg the same pu %s has registered!\n", ptPuRegMsg->GetNOString());

        pcMonMsg->ReverseID();
        post(pcMsg->srcid, CMU_PUI_PU_REG_NACK, (u8* const)pcMonMsg, pcMonMsg->GetMonMsgLen());

        return;
    }
    else
    {
        time_t dwTime;
        time(&dwTime);

        tEncData.GetPuStatus().SetUsed(TRUE);

        TPuStatus& tPuStatus = tEncData.GetPuStatus();
        if(tEncData.IsHasGetCap() && tEncData.IsHasGetStatus())
        {
            tPuStatus.SetOnLine(TRUE);
        }

		g_cVcData.AddOnlineEncNum();

        tEncData.SetInstId(GETINS(pcMsg->srcid));
        tEncData.SetStartPort(ptPuRegMsg->GetPort()!=0?ptPuRegMsg->GetPort():KDM_RCVPORT_PU);
        tEncData.SetIpAddr(ptPuRegMsg->GetIpAddr());
		tEncData.SetPuOnline(tPuStatus.IsOnLine());

		tEncData.SetPuiIpAddr(dwPltIp);
    }

    tEncData.SetIpAddr(ptPuRegMsg->GetIpAddr());

    CMonMsg cMonMsg(pcMsg->content,pcMsg->length);
    TPuRegRsp tPuRegRsp;
	TBasePu tBasePu;
	tBasePu.SetNOString(ptPuRegMsg->GetNOString());
    tPuRegRsp.SetBasePu(tBasePu);
    TNatInfo tNatInfo;
    tNatInfo.SetNatIp(g_cVcData.m_tG200Cfg.GetSelfPlatIP());
    tNatInfo.SetNatPort(7000);

    s8 szIPAddr[32] = {0};
    if ( DEVICE_GLOBAL_TYPE_DECODER != tEncData.GetPuStatus().GetKdmNo().GetDeviceGlobalType() )
    {
        IPtoString(tNatInfo.GetNatIp(), szIPAddr, sizeof(szIPAddr));
        G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "[EVENT/VCPUI]CVcSchedIns::Pu 的nat探测包发送地址[%s--%d]\n", szIPAddr, tNatInfo.GetNatPort());
    }

    tPuRegRsp.SetNatInfo(tNatInfo);
    cMonMsg.SetMsgBody((u8*)&tPuRegRsp,sizeof(TPuRegRsp));
	cMonMsg.ReverseID();
    SendMsgToPuiSsn(tEncData.GetInstId(), CMU_PUI_PU_REG_ACK, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());

    u32 dwPuCount = g_cVcData.GetOnlineEncNum();
    if(dwPuCount >= G200_MAX_ENC_NUM)
    {
        G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "[ERROR/VCPUI]CVcSchedIns::Too many encoder[%d] connect!\n", dwPuCount);
	}
}


void CVcSchedIns::OnPuDisconn(const TKDMNO& tKdmNo)
{
    TVideoOutRelation *ptOutRt = g_cVcData.GetVideoOutRt(tKdmNo);
    if(ptOutRt==NULL)
    {
        G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "%s[%s]:OnPuDisconn wrong PuID[%s] received!\n", m_alias, GetStrState(), tKdmNo.GetNOString());
        return;
    }
	
    TPuStatus& tPuStatus = ptOutRt->GetEncData().GetPuStatus();
 
    ptOutRt->GetEncData().SetPuOnline(FALSE);
	ptOutRt->GetEncData().HasGetCap(FALSE);
	ptOutRt->GetEncData().HasGetStatus(FALSE);
    ptOutRt->GetEncData().SetInstId(0);
	tPuStatus.SetOnLine(FALSE);

	//通知上级平台停止接收该编码器上的所有视频源的码流，删除交换数据
	//并恢复该编码器的所有状态
	NotifyDstStopRcvStream(tKdmNo);

	u32 dwMatrixID = ptOutRt->GetMatrixID();
	TMatrixData* ptMatrixData = g_cVcData.GetMatrixData(dwMatrixID);
	if (NULL != ptMatrixData)
	{
		ptMatrixData->SubOnlineEncNum();

		if (ptMatrixData->GetOnlineEncNum() == 0)
		{
			//如果某个矩阵上所有的编码器都下线了，就向上级广播该矩阵上的所有设备下线
			NotifyCamStatus(dwMatrixID, FALSE);
		}
	}

    //设备退网处理
    if(ptOutRt->GetEncData().IsDel())
    {
        G200Log(LOG_SCHED_MOD, 64, "\t\t\tPuDisconn:pu[%s] is del\n", tPuStatus.GetNOString());
        
		g_cVcData.GetVideoOutRTListPtr(dwMatrixID)->DelItem(ptOutRt);
    }
}

void CVcSchedIns::ProcPuiDisconn(const CMessage * pcMsg)
{
    u16 wPuiInsId = GETINS(pcMsg->srcid);
    TPuiData *ptPuiData = g_cVcData.GetPuiData(wPuiInsId);
	
    if(ptPuiData == NULL)
    {
        G200Log(LOG_SCHED_MOD,  LOGLVL_IMPORTANT, "[ERROR]%s[%s]::ProcPuiDisconn wrong instance id %u received!\n", m_alias, GetStrState(), wPuiInsId);
        return;
    }
	
    G200Log(LOG_SCHED_MOD, 64, "ProcPuiDisConnet: Pui[%s--%d] Disconnect Pu\n", ptPuiData->GetBasePui().GetNOString(), ptPuiData->GetInstId());
	
    //处理与PUI相连的所有PU的断链
	TEncData tEncData;
	for(u32 dwIndex = 1; dwIndex <= G200_MAX_MATRIX_NUM; dwIndex++)
	{
		TVideoOutRTList* ptOutList = g_cVcData.GetVideoOutRTListPtr(dwIndex);
		if (NULL == ptOutList ||ptOutList->GetItemCount() == 0)
		{
			continue;
		}

		TVideoOutRelation* ptOutRt = ptOutList->PosFirst();
		while(ptOutRt != NULL)
		{
			tEncData = ptOutRt->GetEncData();
			if((wPuiInsId == tEncData.GetInstId()) && tEncData.GetPuStatus().IsOnLine())
			{
				OnPuDisconn(tEncData.GetPuStatus().GetKdmNo());
				ptOutRt = ptOutList->PosCur();
				continue;
			}
			ptOutRt = ptOutList->PosNext(ptOutRt);
		}
	}
	
    {
		ptPuiData->SetConnected(FALSE);
		ptPuiData->SetInstId(0);
		
		ptPuiData->SetMaxPuNum(0);
		
		ptPuiData->SetIpNum(1);
		TIpAddr tTmpIp;
		ptPuiData->SetIpAddr(0, tTmpIp);
		ptPuiData->SetCurPuNum(0);
	}
  
    if(ptPuiData->IsDel())
    {
        g_cVcData.GetPuiDataListPtr()->DelItem(ptPuiData);
    }
}


//刷新某个矩阵上的所有设备状态，该函数只有在矩阵上的一个编码器上线或者所有编码器下线的时候调用
void CVcSchedIns::NotifyCamStatus(const u32 dwMatrixID, BOOL32 bOnline)
{
	TPuStatus tCamPuSatus;
	
	CMonMsg cMonMsg;
	cMonMsg.SetSrcID(g_cVcData.m_tG200Cfg.GetG200Id());
	
	TVideoInRTList* ptInRtList = g_cVcData.GetVideoInRTListPtr();
	TVideoInRelation* ptInRt = ptInRtList->PosFirst();
	TKDMNO tKdmNo;
	for(; NULL != ptInRt; ptInRt = ptInRtList->PosNext(ptInRt))
	{
		if (ptInRt->GetMatrixID() == dwMatrixID && ptInRt->GetRegisterStatus())
		{

				tCamPuSatus = ptInRt->GetCameraData().GetPuStatus();
		
			tCamPuSatus.SetOnLine(bOnline);
			cMonMsg.SetMsgBody(&tCamPuSatus,sizeof(TPuStatus));
			SendMsgToSdkSsn(cMonMsg, PUI_CMU_PU_STATUS_NTY);
			
			ptInRt->GetCameraData().SetPuStatus(tCamPuSatus);
			//ptInRt->GetCameraData().SetPuOnline(bOnline);
			OspPrintf(TRUE, FALSE, "上报的设备信息 ： 矩阵编号[%d] -- 摄像头编号[%s]\n", dwMatrixID, tCamPuSatus.GetNOString());
		}
	}
}

void CVcSchedIns::NotifyOneCamStatus(const TKDMNO& tKdmNo, BOOL32 bOnline)
{
	TVideoInRelation* ptInRt = g_cVcData.GetVideoInRT(tKdmNo);
	if (NULL == ptInRt || !ptInRt->GetRegisterStatus())
	{
		return;
	}

	TPuStatus tCamPuSatus;

	tCamPuSatus = ptInRt->GetCameraData().GetPuStatus();


	tCamPuSatus.SetOnLine(bOnline);
	//ptInRt->GetCameraData().SetPuOnline(tCamPuSatus.IsOnLine());
	ptInRt->GetCameraData().SetPuStatus(tCamPuSatus);
	
	CMonMsg cMonMsg;
	cMonMsg.SetSrcID(g_cVcData.m_tG200Cfg.GetG200Id());
	
	cMonMsg.SetMsgBody(&tCamPuSatus,sizeof(TPuStatus));
	SendMsgToSdkSsn(cMonMsg, PUI_CMU_PU_STATUS_NTY);
	
	OspPrintf(TRUE, FALSE, "上报的设备信息 ： 摄像头编号[%s]\n", tCamPuSatus.GetNOString());
	
	return;
}

void CVcSchedIns::ProcMsgFromPui(const CMessage * pcMsg)
{
	switch(pcMsg->event)
	{
	case PUI_CMU_REG_REQ:
		{
			ProcPuiReg(pcMsg);
		}
		break;
	case PUI_CMU_PU_REG_REQ:
		{
			ProcPuReg(pcMsg);
		}
		break;
	case PUI_CMU_PU_CAPABILITY_NTY:
		{
			CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;
			TPuID *ptPuID=(TPuID*)pcMonMsg->GetMsgBody();
            TPuCapability *ptPuCapability=(TPuCapability*)((u8*)pcMonMsg->GetMsgBody()+sizeof(TPuID));
            u8 byVideoEncSlaveChnNum = *(u8*)((u8*)pcMonMsg->GetMsgBody()+sizeof(TPuID)+sizeof(TPuCapability));
            TVideoOutRelation *ptOutRt = g_cVcData.GetVideoOutRt(*ptPuID);
            if(NULL == ptOutRt)
            {
                G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "CVcSchedIns::ProcMsgFromPui capability notify wrong puid=%s.\n",ptPuID->GetNOString());
                return;
            }

            TPuStatus& tPuStatus = ptOutRt->GetEncData().GetPuStatus();
            tPuStatus.SetAlarmInNum(ptPuCapability->GetInputPinNum());
            tPuStatus.SetAlarmOutNum(ptPuCapability->GetOutputPinNum());
            tPuStatus.SetAudioDecNum(ptPuCapability->GetAudioDecNum());
            tPuStatus.SetAudioEncNum(ptPuCapability->GetAudioEncNum());
            tPuStatus.SetSerialPortNum(ptPuCapability->GetSerialPortNum());
            tPuStatus.SetVideoEncNum(ptPuCapability->GetVideoEncNum());
            tPuStatus.SetVideoDecNum(ptPuCapability->GetVideoDecNum());
            tPuStatus.SetVideoInPortNum(ptPuCapability->GetVideoInputPortNum());
            tPuStatus.SetKdmNO(ptOutRt->GetEncData().GetPuStatus().GetKdmNo());
			tPuStatus.SetSndAudio(tPuStatus.GetAudioEncNum()!=0);

			ptOutRt->GetEncData().SetCamNum((u16)ptPuCapability->GetVideoEncNum());
            ptOutRt->GetEncData().HasGetCap(TRUE);
		}
		break;
	case PUI_CMU_PU_STATUS_NTY:
		{
			CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;
			TPuStatus *ptPuStatus=(TPuStatus*)pcMonMsg->GetMsgBody();
            TVideoOutRelation *ptOutRt = g_cVcData.GetVideoOutRt(ptPuStatus->GetKdmNo());
            if(NULL == ptOutRt)
            {
                G200Log(LOG_SCHED_MOD,  LOGLVL_EXCEPTION, "CVcSchedIns::ProcMsgFromPui status notify wrong puid=%s.\n",ptPuStatus->GetNOString());
                return;
            }

            TPuStatus& tPuStatus = ptOutRt->GetEncData().GetPuStatus();
            if(ptPuStatus->GetDeviceGlobalType()==DEVICE_GLOBAL_TYPE_DVR||ptPuStatus->GetDeviceGlobalType()==DEVICE_GLOBAL_TYPE_ENCODER)
            {
                for(u8 byChn=0;byChn<min(MAX_PU_CHNNUM,tPuStatus.GetVideoEncNum());byChn++)
                {
                    tPuStatus.SetVideoType(byChn,ptPuStatus->GetVideoType(byChn));
					tPuStatus.SetDynValue(byChn, ptPuStatus->GetDynValue(byChn));
                    tPuStatus.SetVideoEncInputPortParam(byChn,ptPuStatus->GetVideoEncInputPortParam(byChn));
                    tPuStatus.SetVidSrcChn(byChn,ptPuStatus->GetVidSrcChn(byChn));
                }
                tPuStatus.SetDirectTransNum(ptPuStatus->GetDirectTransNum());
            }
            else if(ptPuStatus->GetDeviceGlobalType()==DEVICE_GLOBAL_TYPE_DECODER)
            {
                tPuStatus.SetDecCombined(ptPuStatus->IsDecCombined());

            }
            else
            {
            }

            tPuStatus.SetDevModel(ptPuStatus->GetDevModel());
            tPuStatus.SetKdmNO(ptOutRt->GetEncData().GetPuStatus().GetKdmNo());
            tPuStatus.SetAppType(ptPuStatus->GetAppType());

			tPuStatus.SetCapItemNum((u8)ptPuStatus->GetCapItemNum());
			tPuStatus.SetCap(ptPuStatus->GetCapType(), (u8)ptPuStatus->GetCapItemNum());
            ptOutRt->GetEncData().HasGetStatus(TRUE);

            if(ptOutRt->GetEncData().IsHasGetCap() && ptOutRt->GetEncData().IsHasGetStatus())
            {
                tPuStatus.SetOnLine(TRUE);
            }

			//ptOutRt->GetEncData().ParsePuVideoChnInfo();

			if (!tPuStatus.IsOnLine())
			{
				return;
			}

			//向上级广播与该编码器属于同一矩阵的所有摄像头设备的状态
			u32 dwMatrixID = ptOutRt->GetMatrixID();
			TMatrixData* ptMatrixData = g_cVcData.GetMatrixData(dwMatrixID);
			if (NULL != ptMatrixData)
			{
				ptMatrixData->AddOnlineEncNum();
				if (ptMatrixData->IsNeedNtyCamStatus())
				{
					NotifyCamStatus(dwMatrixID, TRUE);
					ptMatrixData->SetNeedNtyCamStatus(FALSE);
				}
				else if (ptMatrixData->GetOnlineEncNum() == 1)
				{
					//如果不需要上报标志为FALSE，且是该矩阵上的第一个编码器上线
					//则认为之前该矩阵上的所有编码器都掉线了，现在是重新再次上线
					NotifyCamStatus(dwMatrixID, TRUE);
				}
			}
		}
		break;
	case EV_G200_PUI_DISCONNECT_NTY:
		{
			ProcPuiDisconn(pcMsg);
		}
		break;
	case PUI_CMU_PU_DISCONNECT_NTY:
		{
			CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;
			OnPuDisconn(pcMonMsg->GetSrcKdmNo());
		}
		break;
	default:
		{
			G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "[ERROR/VCPUI] : 收到无法处理的消息[%s-%d]\n",OspEventDesc(pcMsg->event),pcMsg->event);
			break;
		}
	}
}

BOOL32 CVcSchedIns::MatrxiCtrl(const TChannel& tDevChn, TMatrixInfo& tMatrixInfo)
{
	TVideoOutRelation* ptOutRt = g_cVcData.GetVideoOutRt(tDevChn.GetKdmNo());
	if (NULL == ptOutRt)
	{
		return FALSE;
	}
	
	TCamPrtcl tCamPrtcl;
    printf("CVcSchedIns::MatrxiCtrl: sizeof(TCamPrtcl) = %d\n", sizeof(TCamPrtcl));

	//获取摄象头控制协议
	if(!GetCamProtocol(&tMatrixInfo, &tCamPrtcl))
	{
		OspPrintf(TRUE, FALSE,"[G200/VcSched][MatrxiCtrl] : Tran data to matrix protocol failed!\n");
		return FALSE;
	}
	
	G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "************矩阵控制信息************\n ");
	G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "矩阵类型 ：%d\n", tMatrixInfo.m_byCamType);
	G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "矩阵命令 ：%d\n", tMatrixInfo.m_byCommandID);
	G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "输入通道 ：%d\n", tMatrixInfo.m_abyInputList[0]);
	G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "输出通道 ：%d\n", tMatrixInfo.m_byOutputPort);
	G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "************************************\n");

	u16 wInst = ptOutRt->GetEncData().GetInstId();
	
	CMonMsg cMonMsg;
	cMonMsg.SetDstID(tDevChn.GetNOString());
	TPuTransData tPuTranData;
	for(u8 byNumCtrl=0; byNumCtrl<tCamPrtcl.m_byNumPrtcl; byNumCtrl++)
	{
		G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "Buffer[%d] : \n", byNumCtrl);
		for(u8 byNumOfData=0; byNumOfData<tCamPrtcl.m_atCamPrtcl[byNumCtrl].m_iNumOfData; byNumOfData++)
		{
			G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "%.2x.", tCamPrtcl.m_atCamPrtcl[byNumCtrl].m_abyCamData[byNumOfData]);
		}
		G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "\n");
		
		tPuTranData.SetCamDataLen(tCamPrtcl.m_atCamPrtcl[byNumCtrl].m_iNumOfData);
		tPuTranData.SetTransData(tCamPrtcl.m_atCamPrtcl[byNumCtrl].m_abyCamData, tPuTranData.GetCamDataLen());
		
		cMonMsg.SetMsgBody(&tDevChn, sizeof(TPuChn));
		cMonMsg.CatMsgBody(&tPuTranData, sizeof(TPuTransData));
		SendMsgToPuiSsn(wInst, CMU_PUI_PU_SEND_TRANSCOM_CMD, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
	}

	return TRUE;
}
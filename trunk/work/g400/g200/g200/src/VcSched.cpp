#include "G200Data.h"
#include "VcSsn.h"
#include "cmsevent.h"

CVcSchedAppData::CVcSchedAppData()
{
	memset(this, 0, sizeof(CVcSchedAppData));
}

u32 CVcSchedAppData::GetRegFailNum()
{
	return m_dwRegFailNum;
}

void CVcSchedAppData::AddRegFailNum()
{
	m_dwRegFailNum++;
}

void CVcSchedAppData::ClearRegFailNum()
{
	m_dwRegFailNum = 0;
}

void CVcSchedAppData::SetHasFinishGetVideoInRt(BOOL32 bFinish)
{
	m_bHasGetVideoInRt = bFinish;
}

void CVcSchedAppData::SetHasFinishGetVideoOutRt(BOOL32 bFinish)
{
	m_bHasGetVideoOutRt = bFinish;
}

BOOL32 CVcSchedAppData::HasFinishGetVideoInRt()
{
	return m_bHasGetVideoInRt;
}

BOOL32 CVcSchedAppData::HasFinishGetVideoOutRt()
{
	return m_bHasGetVideoOutRt;
}

CVcSchedIns::CVcSchedIns()
{
	
}

LPCSTR CVcSchedIns::GetStrState()
{
	switch(CurState())
    {
    case STATE_IDLE:
        return "Idle";
    case STATE_SERVICE:
        return "Service";
    default:
        return "未知状态";
    }
}

void CVcSchedIns::OnPrintEvent(CMessage *const pcMsg)
{
	switch(pcMsg->event)
	{
	case EV_G200_PRINT_MATRIX_INFO_REQ:
		{
			u32 dwMatrixID = 0;
			memcpy(&dwMatrixID, pcMsg->content, sizeof(u32));

			if (dwMatrixID == 0)
			{
				//打印所有矩阵信息
				OspPrintf(TRUE, FALSE, "\t矩阵名称\t矩阵编号\t矩阵类型\t当前在线编码器数目\n");
				TMatrixDataList* ptList = g_cVcData.GetMatrixDataListPtr();
				TMatrixData* ptMatrixInfo = ptList->PosFirst();
				for(; NULL != ptMatrixInfo; ptMatrixInfo = ptList->PosNext(ptMatrixInfo))
				{
					OspPrintf(TRUE, FALSE, "*************************************************************\n");
					OspPrintf(TRUE, FALSE, "\t  %s\t   %d\t\t    %d\t\t    %d\n", ptMatrixInfo->GetMatrixName(),
						ptMatrixInfo->GetMatrixID(), ptMatrixInfo->GetMatrixType(), ptMatrixInfo->GetOnlineEncNum());
				}
			}
			else
			{
				//打印指定的矩阵信息
				OspPrintf(TRUE, FALSE, "矩阵编号 : %d\n", dwMatrixID);
				TMatrixData *ptMatrixData = g_cVcData.GetMatrixData(dwMatrixID);
				if (NULL != ptMatrixData)
				{
					OspPrintf(TRUE, FALSE, "矩阵类型 : %d\n", ptMatrixData->GetMatrixType());
				}
				OspPrintf(TRUE, FALSE, "矩阵视频输入端口绑定关系 : \n");
				OspPrintf(TRUE, FALSE, "\t矩阵视频输入端口号\t摄像头\t\t编号\n");
				TVideoInRTList* ptList = g_cVcData.GetVideoInRTListPtr();
				TVideoInRelation* ptInRt = ptList->PosFirst();
				for(; NULL != ptInRt; ptInRt = ptList->PosNext(ptInRt))
				{
					if (ptInRt->GetMatrixID() == dwMatrixID)
					{
						OspPrintf(TRUE, FALSE, "\t\t%d\t\t%s\t\t%s\n", ptInRt->GetVideoInPort(), ptInRt->GetCameraData().GetDevName(),
							ptInRt->GetCameraData().GetPuStatus().GetNOString());
					}
				}

				OspPrintf(TRUE, FALSE, "\n矩阵包含的编码器 : \n");
				OspPrintf(TRUE, FALSE, "\t序号\t编码器\t是否连接矩阵\t\t编号\t\t\t\t是否在线\t编码器通道数\n");
				TVideoOutRTList* ptOutList = g_cVcData.GetVideoOutRTListPtr(dwMatrixID);
				if (NULL == ptOutList || ptOutList->GetItemCount() == 0)
				{
					break;
				}

				TVideoOutRelation* ptOutRt = ptOutList->PosFirst();
				TEncVideoInputPortInfo tInfo;
				u8 byIndex = 0;
				for(; NULL != ptOutRt; ptOutRt = ptOutList->PosNext(ptOutRt))
				{
					if (ptOutRt->GetMatrixID() != dwMatrixID)
					{
						continue;
					}
					
					TEncData& tEncdata = ptOutRt->GetEncData();
					OspPrintf(TRUE, FALSE, "\t%d\t%s\t\t%d\t%s\t%s\t\t%d\n", byIndex,tEncdata.GetDevName(), ptOutRt->IsConnMatrix(),
						tEncdata.GetPuStatus().GetNOString(),GetStrBool(tEncdata.GetPuStatus().IsOnLine()),
						tEncdata.GetPuStatus().GetVideoEncNum());
					byIndex++;
				}
			}
		}
		break;
	case EV_G200_PRINT_VIDEOOUT_RT_REQ:
		{
			CMonMsg* pcMonMsg = (CMonMsg*)pcMsg->content;
			s32 nMatrixNo = *(s32 *)pcMonMsg->GetMsgBody();
			s32 nEncoderIndex = *(s32 *)pcMonMsg->GetMsgBody(sizeof(s32));
			

			TEncVideoInputPortInfo tInfo;
			TVideoOutRelation* ptOutRt = g_cVcData.GetVideoOutRt(nMatrixNo,nEncoderIndex);
			if (NULL != ptOutRt)
			{
				OspPrintf(TRUE, FALSE, "编码器编号     : %s\n", ptOutRt->GetEncData().GetPuStatus().GetNOString());
				OspPrintf(TRUE, FALSE, "所属的矩阵编号 : %d\n", ptOutRt->GetMatrixID());
				OspPrintf(TRUE,FALSE,"编码器名称：%s\n",ptOutRt->GetEncData().GetDevName());
				OspPrintf(TRUE, FALSE, "是否连接了矩阵 : %d\n", ptOutRt->IsConnMatrix());
				OspPrintf(TRUE, FALSE, "   视频输入通道号   矩阵视频输出端口   当前对应的矩阵视频输入端口(0表示空闲)\n");
				
				for(u8 byIndex = 0; byIndex < MAX_PU_CHNNUM; byIndex++)
				{
					tInfo = ptOutRt->GetEncVideoInputPortInfo(byIndex);
					if (tInfo.IsConnVideoOutPort())
					{
						OspPrintf(TRUE, FALSE, "\t%d\t\t%d\t\t\t%d\n", byIndex, tInfo.GetVideoOutPort(), tInfo.GetCurMatrixInputPort());
					}
				}

				OspPrintf(TRUE, FALSE, "编号器状态：\n");
				ptOutRt->GetEncData().PrintData();
			}
		}
		break;
    case EV_G200_PRINT_CAMSTATUS_REQ:
        {
            TKDMNO tKdmNo;
            tKdmNo.SetNOString((const s8 *)pcMsg->content);
            
            TVideoInRelation* ptVideoInRelation = g_cVcData.GetVideoInRT(tKdmNo);
            if ( NULL != ptVideoInRelation)
            {
                OspPrintf(TRUE,FALSE,"摄像头名称       : %s\n",ptVideoInRelation->GetCameraData().GetDevName());
                OspPrintf(TRUE,FALSE,"摄像头编号       : %s\n",tKdmNo.GetNOString());
                OspPrintf(TRUE,FALSE,"所属矩阵编号     : %d\n",ptVideoInRelation->GetMatrixID());
                OspPrintf(TRUE,FALSE,"连接的矩阵端口号 : %d\n",ptVideoInRelation->GetVideoInPort());
                ptVideoInRelation->GetCameraData().PrintData();
            }
        }
        break;
	case EV_G200_PRINT_SWITCHINFO_REQ:
		{
			OspPrintf(TRUE, FALSE, "\n矩阵编号\t摄像头信息\t\t\t\t编码器信息\t\t\t\t发送视频的端口信息\n");
			OspPrintf(TRUE, FALSE, "\t   CamNo:VideoInPort\t\t\tEncNo:EncChnId:VidChnId:VideoOutPort  \t\tVideoIp:port\n");
			TSwitchDataList* ptSwitchDataList = g_cVcData.GetSwitchDataListPtr();
			TSwitchData* ptSwitchData = ptSwitchDataList->PosFirst();
			for(; NULL != ptSwitchData; ptSwitchData = ptSwitchDataList->PosNext(ptSwitchData))
			{
				s8 achIp[20] = {0};
				IPtoString(ptSwitchData->GetVideoIp(),achIp,sizeof(achIp));
				OspPrintf(TRUE, FALSE, "  %d\t%s:%d\t%s:%d:%d:%d\t\t%s:%d\n", ptSwitchData->GetMatrixID(),
					                                             ptSwitchData->GetCamNO().GetNOString(),
					                                             ptSwitchData->GetVideoInPort(),
																 ptSwitchData->GetEncNO().GetNOString(),
																 ptSwitchData->GetEncChnId(),
																 ptSwitchData->GetVidChnId(),
																 ptSwitchData->GetVideoOutPort(),achIp,
																 ptSwitchData->GetVideoIpPort());
			}
		}
		break;
	case EV_G200_PRINT_LICENSEINFO_REQ:
		{
			OspPrintf(TRUE,FALSE,"***********License Information********\n");
#if 1
			OspPrintf(TRUE,FALSE,"是否有效:%s\n",GetStrBool(g_cVcData.m_cLicenseHandler.IsLicenseValid()));
			OspPrintf(TRUE,FALSE,"许可证号:%s\n",g_cVcData.m_cLicenseHandler.GetApprovalNo());
			OspPrintf(TRUE,FALSE,"有效日期:%s\n",g_cVcData.m_cLicenseHandler.GetMaxDate());
			OspPrintf(TRUE,FALSE,"机器码:%s\n",g_cVcData.m_cLicenseHandler.GetMacInfo());
			OspPrintf(TRUE,FALSE,"支持最大视频数:%d\n",g_cVcData.m_cLicenseHandler.GetMaxChannel());
			OspPrintf(TRUE,FALSE,"支持编码器通道数:%d\n",g_cVcData.m_cLicenseHandler.GetMaxEncoderNum());
			s8 szlicenseType[20];
			if (g_cVcData.m_cLicenseHandler.GetLicenseType() == CLicenseHandler::TYPE_FILE)
			{
				sprintf(szlicenseType,"文件");
			}
			else if (g_cVcData.m_cLicenseHandler.GetLicenseType() == CLicenseHandler::TYPE_USB)
			{
				sprintf(szlicenseType,"USB");
			}
			else
			{
				sprintf(szlicenseType,"无");
			}
			OspPrintf(TRUE,FALSE,"license类型:%s\n",szlicenseType);
#else
			OspPrintf(TRUE,FALSE,"当前版本最大支持%d路视频!\n",G200_MAX_CAMERA_NUM);
#endif		
			
		}
		break;
	case EV_G200_PRINT_G200STATUS_REQ:
		{
			OspPrintf(TRUE,FALSE,"***********G200Status Information********\n");
			time_t tBootTime = g_cVcData.m_tG200Cfg.GetBootTime();
			OspPrintf(TRUE,FALSE,"程序启动时间:%s",ctime(&tBootTime));
			OspPrintf(TRUE,FALSE,"连接上级平台:%s\n",GetStrBool(g_cVcData.GetConnMplat()));
		}
		break;
	default:
		{
			G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "[G200/VcSched][OnPrintEvent] : 收到未知的消息[%d]\n", pcMsg->event);
		}
	}
}

void CVcSchedIns::ProcSendStream(const CMessage * pcMsg)
{
	CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;

	TChannel *ptSrcChn = (TChannel *)pcMonMsg->GetMsgBody();
	TTransportAddr *ptTransportAddr = (TTransportAddr *)pcMonMsg->GetMsgBody(sizeof(TChannel));
	TTransportAddr* ptAudioAddr = (TTransportAddr *)pcMonMsg->GetMsgBody(sizeof(TChannel) +
		sizeof(TTransportAddr) + sizeof(u8) + sizeof(u8));

	TVideoInRelation* ptInRt = g_cVcData.GetVideoInRT(ptSrcChn->GetKdmNo());
	if (NULL == ptInRt)
	{
		OspPrintf(TRUE, FALSE, "[G200/VcSched][ProcSendStream] : 设备[%s]没有注册！\n", ptSrcChn->GetNOString());
		SendMsgToSdkSsn(*pcMonMsg, pcMsg->event+2);
		return;
	}

	TSwitchData *ptSwitchData = g_cVcData.GetSwitchData(ptSrcChn->GetKdmNo(),ptTransportAddr->GetIpAddr(),
		ptTransportAddr->GetPort());
	if (NULL != ptSwitchData)
	{
		OspPrintf(TRUE, FALSE, "[G200/VcSched][ProcSendStream] : 设备[%s]的交换数据已经存在！\n", ptSrcChn->GetNOString());
		SendMsgToSdkSsn(*pcMonMsg, pcMsg->event+2);
		return;
	}

	u32 dwMatrixID = ptInRt->GetMatrixID();
	u32 dwMatrixType = ptInRt->GetMatrixType();

    TChannel tChn;
	u8 byEncChnId = 0xff;
	u8 byVidChnId = 0xff;
	u16 wVideoOutPort = 0;
	TVideoOutRelation tIdleEnc;
    {
	    TVideoOutRelation tConnMatrixEnc;

	    //选择一个可用的编码器接收摄像头的码流，选择一个连接了矩阵的编码器进行矩阵控制，完成视频源切换等操作
	    if (!g_cVcData.ChooseEnableEnc(dwMatrixID, ptInRt->GetVideoInPort(), tConnMatrixEnc, tIdleEnc, byEncChnId, byVidChnId, wVideoOutPort))
	    {
		    SendMsgToSdkSsn(*pcMonMsg, pcMsg->event+2);
		    return;
	    }

	    //根据选择的连接了矩阵的编码器发送矩阵控制命令
	    TPuStatus& tConnMatrixPuStatus = tConnMatrixEnc.GetEncData().GetPuStatus();
	    tChn.SetNOString(tConnMatrixPuStatus.GetNOString());
	    tChn.SetVidChnId(0);
    }
	
	TMatrixInfo tMatrixInfo;
	memset(&tMatrixInfo, 0, sizeof(tMatrixInfo));			
	tMatrixInfo.m_byCamType = (u8)dwMatrixType;
	tMatrixInfo.m_byCommandID = G200_CAM_CMD_MATRIX_SWTICH;		
	tMatrixInfo.m_abyInputList[0] = (u8)ptInRt->GetVideoInPort();
	tMatrixInfo.m_byOutputPort = (u8)wVideoOutPort;
	
	if (!MatrxiCtrl(tChn, tMatrixInfo))
	{
		TVideoOutRelation* ptIdleEnc = g_cVcData.GetVideoOutRt(tIdleEnc.GetEncData().GetPuStatus().GetKdmNo());
		if (ptIdleEnc != NULL)
		{
			ptIdleEnc->ResetEncStatus(byEncChnId, byVidChnId);
		}
		SendMsgToSdkSsn(*pcMonMsg, pcMsg->event+2);
		return;
	}

	OspDelay(500);

	TPuStatus& tIdelPuStatus = tIdleEnc.GetEncData().GetPuStatus();
	{
		//通知设备开始发送码流
		CMonMsg cMonMsg(pcMonMsg->GetMonMsg(), pcMonMsg->GetMonMsgLen());
		cMonMsg.SetDstID(tIdelPuStatus.GetNOString());
		
		TPuChn *ptPuChn = (TPuChn *)cMonMsg.GetMsgBody();
		ptPuChn->SetNOString(tIdelPuStatus.GetNOString());
		ptPuChn->SetEdcChnId(byEncChnId);
		ptPuChn->SetVidChnId(byVidChnId);
		
		u8* pbyMode = (u8 *)cMonMsg.GetMsgBody(sizeof(TChannel)+sizeof(TTransportAddr));
		if (!tIdleEnc.GetEncData().GetPuStatus().IsSndAudio())
		{
			*pbyMode = MODE_VIDEO;
		}
		
		if (FALSE == SendMsgToPuiSsn(tIdleEnc.GetEncData().GetInstId(), CMU_PUI_PU_STARTSEND_CMD, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen()))
		{
			tIdleEnc.ResetEncStatus(byEncChnId, byVidChnId);
			SendMsgToSdkSsn(*pcMonMsg, pcMsg->event+2);
			return;
		}

		//通知设备发送关键帧
		cMonMsg.SetEventID(CMU_PUI_PU_SENDKEYFRAME_CMD);
		cMonMsg.SetMsgBody(ptPuChn,sizeof(TPuChn));
		SendMsgToPuiSsn(tIdleEnc.GetEncData().GetInstId(),CMU_PUI_PU_SENDKEYFRAME_CMD, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
	}

	//添加交换数据
	TSwitchData tSwitchData;
	tSwitchData.SetMatrixID(ptInRt->GetMatrixID());
	tSwitchData.SetCamNO(ptSrcChn->GetKdmNo());
	tSwitchData.SetVideoInPort(ptInRt->GetVideoInPort());
	tSwitchData.SetEncNO(tIdelPuStatus.GetKdmNo());
	tSwitchData.SetEncChnId(byEncChnId);
	tSwitchData.SetVidChnId(byVidChnId);
	tSwitchData.SetVideoOutPort(wVideoOutPort);

	tSwitchData.SetVideoIp(ptTransportAddr->GetIpAddr());
	tSwitchData.SetVideoIpPort(ptTransportAddr->GetPort());

	tSwitchData.SetAudioIp(ptAudioAddr->GetIpAddr());
	tSwitchData.SetAudioIpPort(ptAudioAddr->GetPort());

	g_cVcData.AddSwitchData(tSwitchData);

	//构造rtcp地址
	TTransportAddr tVideoRtcpAddr;
	tVideoRtcpAddr.SetIpAddr(tIdleEnc.GetEncData().GetIpAddr());
	tVideoRtcpAddr.SetPort(tIdleEnc.GetEncData().GetStartPort() + byEncChnId * 4 + 1);
	TTransportAddr tAudioAddr;
	tAudioAddr.SetIpAddr(tIdleEnc.GetEncData().GetIpAddr());
	tAudioAddr.SetPort(tVideoRtcpAddr.GetPort()+2);

	CMonMsg cMonMsgRsp(pcMonMsg->GetMonMsg(), pcMonMsg->GetMonMsgLen());
	cMonMsgRsp.SetMsgBody(&tVideoRtcpAddr,sizeof(TTransportAddr));
	cMonMsgRsp.CatMsgBody(&tAudioAddr, sizeof(TTransportAddr));

	SendMsgToSdkSsn(cMonMsgRsp, pcMsg->event+1);

	return;
}

void CVcSchedIns::ProcStopStream(const CMessage * pcMsg)
{
	CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;
	
	TChannel *ptSrcChn = (TChannel *)pcMonMsg->GetMsgBody();
	TTransportAddr *ptTransportAddr = (TTransportAddr *)pcMonMsg->GetMsgBody(sizeof(TChannel) + sizeof(u8));
	TVideoInRelation* ptInRt  = g_cVcData.GetVideoInRT(ptSrcChn->GetKdmNo());
	if (NULL == ptInRt)
	{
		OspPrintf(TRUE, FALSE, "[G200/VcSched][ProcStopStream] : 找不到摄像头[%s]的绑定信息, 返回停止码流发送成功！\n", ptSrcChn->GetNOString());
		SendMsgToSdkSsn(*pcMonMsg, pcMsg->event+1);
		return;
	}
	
	TSwitchData* ptSwitchData = g_cVcData.GetSwitchData(ptSrcChn->GetKdmNo(),ptTransportAddr->GetIpAddr(),
		ptTransportAddr->GetPort());
	if (NULL == ptSwitchData)
	{
		OspPrintf(TRUE, FALSE, "[G200/VcSched][ProcStopStream] : 找不到摄像头[%s]的交换数据, 返回停止码流发送成功！\n", ptSrcChn->GetNOString());
		SendMsgToSdkSsn(*pcMonMsg, pcMsg->event+1);
		return;
	}

	TKDMNO& tEncNO = ptSwitchData->GetEncNO();
	TVideoOutRelation* ptOutRt = g_cVcData.GetVideoOutRt(tEncNO);
	if (NULL  == ptOutRt)
	{
		OspPrintf(TRUE, FALSE, "[G200/VcSched][ProcStopStream] : 编码器[%s]数据不存在, 返回停止码流发送成功！\n", ptSrcChn->GetNOString(), tEncNO.GetNOString());
		SendMsgToSdkSsn(*pcMonMsg, pcMsg->event+1);
		return;
	}

	u16 wInstId = ptOutRt->GetEncData().GetInstId();
	u8 byMode = *(u8 *)pcMonMsg->GetMsgBody(sizeof(TChannel));
	if (!ptOutRt->GetEncData().GetPuStatus().IsSndAudio())
	{
		byMode = MODE_VIDEO;
	}

	//通过编码器停止发送码流
	CMonMsg cMonMsg(pcMonMsg->GetMonMsg(), pcMonMsg->GetMonMsgLen());
	cMonMsg.SetDstID(tEncNO);

	u8 byEncChnId = ptSwitchData->GetEncChnId();
	u8 byVidChnId = ptSwitchData->GetVidChnId();

	TPuChn tPuChn;
	tPuChn.SetKdmNO(tEncNO);
	tPuChn.SetEdcChnId(byEncChnId);
	tPuChn.SetVidChnId(byVidChnId);

	cMonMsg.SetMsgBody(&tPuChn, sizeof(TPuChn));
	cMonMsg.CatMsgBody(&byMode, sizeof(byMode));
	SendMsgToPuiSsn(wInstId, CMU_PUI_PU_STOPSEND_CMD, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());

    SendMsgToSdkSsn(*pcMonMsg, pcMsg->event+1);

	//恢复编码器相关编码通道及视频输入通道的状态
	ptOutRt->ResetEncStatus(byEncChnId, byVidChnId);

	g_cVcData.DelSwitchDataByCamNo(ptSrcChn->GetKdmNo(),ptTransportAddr->GetIpAddr(),
		ptTransportAddr->GetPort());
}

void CVcSchedIns::ProcDisconnWithMplat()
{
	//将所有的交换数据清空，并恢复所有的设备通道状态信息
	TSwitchDataList *ptSwitchList = g_cVcData.GetSwitchDataListPtr();
	if (ptSwitchList->GetItemCount() != 0)
	{
		TKDMNO tEncNo;
		TKDMNO tCamNo;
		u8 byEncChnId = 0;
		u8 byVidChnId = 0;
		u16 wVidOutPort = 0;
		TVideoOutRelation *ptOutRt = NULL;
		
		TSwitchData *ptSwitchData = ptSwitchList->PosFirst();
		for(; NULL != ptSwitchData; ptSwitchData = ptSwitchList->PosNext(ptSwitchData))
		{
			tEncNo = ptSwitchData->GetEncNO();
			ptOutRt = g_cVcData.GetVideoOutRt(tEncNo);
			if (NULL != ptOutRt)
			{
				byEncChnId = ptSwitchData->GetEncChnId();
				byVidChnId = ptSwitchData->GetVidChnId();
				ptOutRt->ResetEncStatus(byEncChnId, byVidChnId);
			}
		}	
		ptSwitchList->Clear();
	}
	g_cVcData.SetConnMplat(FALSE);
}

/**********************************************
函数名:
功能:上级平台断链并重新连接时,向上级平台返回当前的设备状态,此时只要返回在线的设备即可
实现方式:
参数列表:
返回值列表:
修改记录:
-------------------------------------------------
修改日期      版本号      修改人         修改结果
20100205       v1.0       李清平           添加
***********************************************/
void CVcSchedIns::ProcConnWithMplat(void)
{
    TMatrixDataList *ptMatrixDataList = g_cVcData.GetMatrixDataListPtr();
    TMatrixData *ptMatrixData = ptMatrixDataList->PosFirst();
    while (NULL != ptMatrixData)
    {
        u16 wOnLineNum = ptMatrixData->GetOnlineEncNum();
        if (wOnLineNum > 0)
        {
            NotifyCamStatus(ptMatrixData->GetMatrixID(),TRUE);
        }
        ptMatrixData = ptMatrixDataList->PosNext(ptMatrixData);
    }
	g_cVcData.SetConnMplat(TRUE);
}

void CVcSchedIns::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{
    if( NULL == pcMsg )
    {
        G200Log(LOG_SCHED_MOD, LOGLVL_EXCEPTION, "%s[%s]: The received msg's pointer is NULL!", m_alias, GetStrState());

        return;
    }

    if(pcMsg->event == EV_G200_SET_INST_ALIAS)
    {
        //设置实例名
        u16 wInsCount = (u16) pcApp->GetInstanceNumber();

        s8 szAlias[MAX_ALIAS_LEN] = {0};
        strncpy(szAlias, "[G200/VcSched-Daemon]", sizeof(szAlias) - 1);

        pcApp->SetInstAlias(CInstance::DAEMON, szAlias, (u8)strlen(szAlias));

        return;
    }

    G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG2,"%s[%s]:Receive Message[%s]\n",m_alias,GetStrState(),OspEventDesc(pcMsg->event));
	if (pcMsg->event >= EV_G200_PRINT_MATRIX_INFO_REQ && pcMsg->event < END_PRINT_EVENT)
	{
		OnPrintEvent(pcMsg);
		return;
	}

	if (OnCommonMsg(pcMsg))
	{
		return;
	}

    switch(CurState())
    {
    case STATE_IDLE:
        {
            OnIdle(pcMsg);
        }
        break;
    case STATE_SERVICE:
        {
            OnService(pcMsg);
        }
        break;
    default:
        {
            G200Log(LOG_SCHED_MOD, LOGLVL_EXCEPTION, "%s[%s-%d]:未定义状态\n", m_alias, GetStrState(), CurState());
        }
        break;
    }
}

void CVcSchedIns::OnIdle(const CMessage *const pcMsg)
{
	switch(pcMsg->event)
	{
	case EV_G200_VCSCHED_POWERON:
		{
            //启动配置管理APP
			post(MAKEIID(AID_G200_CFG, CInstance::DAEMON), EV_G200_CFGMGR_POWERON);

			//向配置管理线程获取所有绑定关系信息
			s8 abyMatrixCfgPath[MAXLEN_FILEPATH+1] = {0};
			strncpy(abyMatrixCfgPath, g_cVcData.m_tG200Cfg.GetBindRtCfgPath(), MAXLEN_FILEPATH);
			
			CMonMsg cMonMsgSnd;
			cMonMsgSnd.SetMsgBody(abyMatrixCfgPath, sizeof(abyMatrixCfgPath));
				SendMsgToCfgMgr(cMonMsgSnd, EV_G200_CFGMGR_GET_BINDRT_CMD);

			//如果license有效
			if (g_cVcData.m_cLicenseHandler.IsLicenseValid())
			{
				//启动sdk模块
				post(MAKEIID(AID_G200_SDK, CInstance::DAEMON),EV_G200_SDKSSN_POWERON);
			}
			
		}
		break;
	case EV_G200_CFGMGR_GET_MATRIX_NTY:
	case EV_G200_CFGMGR_GET_BINDINRT_NTY:
	case EV_G200_CFGMGR_GET_BINDOUTRT_NTY:
		{
			ProcMsgFromCfgMgr(pcMsg);
		}
		break;
	default:
		{
			G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "[G200/VcSched][%s] : 收到未知消息[%s-%d]\n", GetStrState(), OspEventDesc(pcMsg->event),pcMsg->event);
		}
		break;
	}
}

void CVcSchedIns::OnService(const CMessage *const pcMsg)
{
	switch(pcMsg->event)
	{
	case PUI_CMU_REG_REQ:
	case PUI_CMU_PU_REG_REQ:
	case PUI_CMU_PU_CAPABILITY_NTY:
	case PUI_CMU_PU_STATUS_NTY:
	case EV_G200_PUI_DISCONNECT_NTY:
	case PUI_CMU_PU_DISCONNECT_NTY:
		{
			ProcMsgFromPui(pcMsg);
		}
		break;
	case EV_G200_SDK_START_SEND_REQ:
	case EV_G200_SDK_STOP_SEND_REQ:
    case EV_G200_YJCTRL_CMD:
		{
			ProcMsgFromSdk(pcMsg);
		}
		break;
	case EV_G200_DISCONNECT_WITH_MPLAT_NTY:
		{
			ProcDisconnWithMplat();
		}
		break;
    case EV_G200_CONNECT_WITH_MPLAT_NTY:
        {
            ProcConnWithMplat();            
        }
        break;
    case EV_HS_SERVER_KEDACOMXMLDATA:
        {
            ProcMsgFromG200MS(pcMsg);
        }
        break;
	default:
		{
			G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "[G200/VcSched][%s] : 收到无法处理的消息[%s-%d]\n",GetStrState(),OspEventDesc(pcMsg->event),pcMsg->event);
			break;
		}
	}

}

BOOL32 CVcSchedIns::OnCommonMsg(const CMessage *const pcMsg)
{
	switch(pcMsg->event)
	{
	case EV_G200_CFGMGR_ADD_BINDINRT_NTY:
		{
			ProcAddBindInRtNty(pcMsg);
		}
		break;
	default:
		{
			return FALSE;
		}
	}

	return TRUE;
}
